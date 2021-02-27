#include <tcp_server_service.h>
#include <tcp_listen.h>
#include <tcp_server.h>
#include <tcp_socket.h>
#include <tcp_session.h>
#include <tcp_thread_pool.h>
#include <tcp_thread.h>
#include <QtCore/QThread>
#include <windows.h>
namespace Shata
{
    namespace Socket
    {
        TcpServerService::TcpServerService() :
            tcp_server_thread_pool(std::make_unique<TcpThreadPool>()),
            tcp_session_thread_pool(std::make_unique<TcpThreadPool>()),
            tcp_index(0)
        {
            qRegisterMetaType<u8>("u8");
            qRegisterMetaType<u16>("u16");
            qRegisterMetaType<u32>("u32");
            qRegisterMetaType<u64>("u64");
            qRegisterMetaType<u96>("u96");

            qRegisterMetaType<i8>("i8");
            qRegisterMetaType<i16>("i16");
            qRegisterMetaType<i32>("i32");
            qRegisterMetaType<i64>("i64");
            qRegisterMetaType<i96>("i96");

            qRegisterMetaType<std::shared_ptr<TcpSession>>("std::shared_ptr<TcpSession>");
        }

        TcpServerService::~TcpServerService()
        {

        }

        bool TcpServerService::AddListenPort(const u96 index, const QHostAddress& address, u16 port)
        {
            if (tcp_socket_pool.find(index) == tcp_socket_pool.end())
            {
                return tcp_socket_pool.emplace(index, std::make_unique<TcpSocket>(address, port)).second;
            }
            return false; // 已经存在
        }

        bool TcpServerService::CreaterServer()
        {
            if (tcp_socket_pool.empty())
            {
                return false;
            }

            if (tcp_session_thread_pool->CreaterThreadPool(8) && tcp_server_thread_pool->CreaterThreadPool(tcp_socket_pool.size()))
            {
                for (u96 i = 0; i < tcp_socket_pool.size(); ++i)
                {
                    if (tcp_server_pool.emplace(i, std::make_unique<TcpServer>()).second)
                    {
                        if (connect(tcp_server_pool[i].get(), &TcpServer::SendConnectionNotify, this, &TcpServerService::OnConnection, Qt::QueuedConnection))
                        {
                            if (tcp_server_pool[i]->CreaterServer(tcp_socket_pool[i]->GetHost(), tcp_socket_pool[i]->GetPort()))
                            {
                                // 为了不影响主线程消息队列，启动新的线程执行 OnConnection，此外所有动作将会以异步的方式投递到主线程中。 
                                tcp_server_pool[i]->moveToThread(tcp_server_thread_pool->GetEventThread(i));
                                {
                                    continue;
                                }
                            }

                            if (disconnect(tcp_server_pool[i].get(), &TcpServer::SendConnectionNotify, this, &TcpServerService::OnConnection))
                            {
                                continue;
                            }
                        }
                    }
                }
                return true;
            }
            return false;
        }

        bool TcpServerService::DestroyServer()
        {
            for (u96 i = 0; i < tcp_socket_session.size(); ++i)
            {
                emit tcp_socket_session[i]->SendCloseNotify();
            }

            // 理论上可以按照逻辑销毁，没有经过测试的代码。
            for (u96 i = 0; i < tcp_server_pool.size(); ++i)
            {
                if (tcp_server_pool[i]->DestroyServer())
                {
                    if (disconnect(tcp_server_pool[i].get(), &TcpServer::SendConnectionNotify, this, &TcpServerService::OnConnection))
                    {
                        continue;
                    }
                }
                return false;
            }

            return tcp_server_thread_pool->DestroyThreadPool() && tcp_session_thread_pool->DestroyThreadPool();
        }

        void TcpServerService::SetConnectionCallback(const InterfaceConnection& callback)
        {
            if (nullptr == tcp_connection_callback)
            {
                tcp_connection_callback = callback;
            }
        }

        void TcpServerService::SetDisconnectCallback(const InterfaceDisconnect& callback)
        {
            if (nullptr == tcp_disconnect_callback)
            {
                tcp_disconnect_callback = callback;
            }
        }

        void TcpServerService::SetMessageCallback(const InterfaceMessage& callback)
        {
            if (nullptr == tcp_message_callback)
            {
                tcp_message_callback = callback;
            }
        }

        void TcpServerService::SetSendReceiptCallback(const InterfaceSendReceipt& callback)
        {
            if (nullptr == tcp_sendreceipt_callback)
            {
                tcp_sendreceipt_callback = callback;
            }
        }

        void TcpServerService::SetDisplayErrorCallback(const InterfaceDisplayError& callback)
        {
            if (nullptr == tcp_error_callback)
            {
                tcp_error_callback = callback;
            }
        }

        void TcpServerService::AddSession(const u96 index, qintptr handler)
        {
            if (tcp_socket_session.find(index) == tcp_socket_session.end())
            {
                if (tcp_socket_session.emplace(index, std::make_shared<TcpSession>(index, handler)).second)
                {
                    // 将会话投递到线程中运行，某一个线程暂时阻塞不影响其它线程消息执行顺序（投递是均衡分配的）。

                    if (nullptr != tcp_connection_callback)
                    {
                        tcp_connection_callback(GetSession(index), index);
                    }

                    return CreaterSession(GetSession(index).get(), tcp_session_thread_pool->GetEventThread(index), index);
                }
            }
        }

        void TcpServerService::DelSession(const u96 index)
        {
            if (tcp_socket_session.find(index) != tcp_socket_session.end())
            {
                tcp_socket_session.erase(index);
                tcp_index_multiplexing.emplace(index);
            }
        }

        const std::shared_ptr<TcpSession>& TcpServerService::GetSession(const u96 index)
        {
            return std::cref(tcp_socket_session[index]);
        }

        void TcpServerService::CreaterSession(TcpSession* session, TcpThread* thread, const u96 index)
        {
            if (nullptr != session)
            {
                if (   connect(session, &TcpSession::SendDisconsNotify,         this, &TcpServerService::OnDisconnect, Qt::QueuedConnection)
                    && connect(session, &TcpSession::SendMessageNotify,         this, &TcpServerService::OnMessage, Qt::DirectConnection)
                    && connect(session, &TcpSession::SendReceiptNotify,         this, &TcpServerService::OnSendReceipt, Qt::DirectConnection)
                    && connect(session, &TcpSession::SendDisplayErrorNotify,    this, &TcpServerService::OnDisplayError, Qt::QueuedConnection))
                {
                    // 将会话任务送到线程中运行
                    return session->moveToThread(thread);
                }
            }
        }

        bool TcpServerService::DestroySession(TcpSession* session)
        {
            if (nullptr != session)
            {
                return
                   disconnect(session, &TcpSession::SendDisconsNotify,          this, &TcpServerService::OnDisconnect)
                && disconnect(session, &TcpSession::SendMessageNotify,          this, &TcpServerService::OnMessage)
                && disconnect(session, &TcpSession::SendReceiptNotify,          this, &TcpServerService::OnSendReceipt)
                && disconnect(session, &TcpSession::SendDisplayErrorNotify,     this, &TcpServerService::OnDisplayError);
            }
            return false;
        }

        const u96 TcpServerService::GetPlexingIndex(u96 index)
        {
            if (index == 0)
            {
                return GetPlexingIndex(tcp_index_multiplexing.empty() ? tcp_index.fetch_add(1) : tcp_index_multiplexing.top());
            }

            if (tcp_index_multiplexing.size())
            {
                tcp_index_multiplexing.pop();
            }
            return index;
        }

        void TcpServerService::OnConnection(qintptr handler)
        {
            // 不用锁，因为信号投递到主线程，会排队依次执行。这个函数是一个槽。
            return AddSession(GetPlexingIndex(), handler);
        }

        void TcpServerService::OnDisconnect(const std::shared_ptr<TcpSession>& session, const u96 index)
        {
            if (nullptr != tcp_disconnect_callback)
            {
                tcp_disconnect_callback(session, index);
            }

            if (DestroySession(session.get()))
            {
                return DelSession(index);
            }
        }

        void TcpServerService::OnMessage(const std::shared_ptr<TcpSession>& session, QIODevice* buffer, const u96 index)
        {
            if (nullptr != tcp_message_callback)
            {
                tcp_message_callback(session, buffer, index);
            }
        }

        void TcpServerService::OnSendReceipt(const std::shared_ptr<TcpSession>& session, const i64 bytes, const u96 index)
        {
            if (nullptr != tcp_sendreceipt_callback)
            {
                tcp_sendreceipt_callback(session, bytes, index);
            }
        }

        void TcpServerService::OnDisplayError(const std::shared_ptr<TcpSession>& session, QAbstractSocket::SocketError error, const u96 index)
        {
            if (nullptr != tcp_error_callback)
            {
                tcp_error_callback(session, error, index);
            }
        }
    }
}