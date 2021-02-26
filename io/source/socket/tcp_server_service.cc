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
            
        }

        TcpServerService::~TcpServerService()
        {

        }

        bool TcpServerService::AddListenPort(const quint32 index, const QHostAddress& address, quint16 port)
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

            //  session 静态线程池                                      server 静态线程池（根据监听端口数量启动对应数量的线程）
            if (tcp_session_thread_pool->CreaterEventThreadPool(8) && tcp_server_thread_pool->CreaterEventThreadPool(tcp_socket_pool.size()))
            {
                for (qint32 i = 0; i < tcp_socket_pool.size(); ++i)
                {
                    // 暂时不用处理 delete 后期在写销毁逻辑代码
                    if (tcp_server_pool.emplace(i, new TcpServer()).second)
                    {
                        if (connect(tcp_server_pool[i], &TcpServer::SendConnectionNotify, this, &TcpServerService::OnConnection, Qt::QueuedConnection))
                        {
                            if (tcp_server_pool[i]->CreaterServer(tcp_socket_pool[i]->GetHost(), tcp_socket_pool[i]->GetPort()))
                            {
                                // 为了不影响主线程消息队列，启动新的线程执行 OnConnection，此外所有动作将会以异步的方式投递到主线程中。 
                                tcp_server_pool[i]->moveToThread(tcp_server_thread_pool->GetEventThread(i));
                                {
                                    continue;
                                }
                            }
 
                            if (disconnect(tcp_server_pool[i], &TcpServer::SendConnectionNotify, this, &TcpServerService::OnConnection))
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

        void TcpServerService::AddSession(const quint32 index, qintptr handler)
        {
            if (tcp_socket_session.find(index) == tcp_socket_session.end())
            {
                if (tcp_socket_session.emplace(index, std::make_unique<TcpSession>(index, handler)).second)
                {
                    // 将会话投递到线程中运行，某一个线程暂时阻塞不影响其它线程消息执行顺序（投递是均衡分配的）。
                    InitializeSession(GetSession(index), tcp_session_thread_pool->GetEventThread(index));
                }
            }
        }

        void TcpServerService::DelSession(const quint32 index)
        {
            if (tcp_socket_session.find(index) != tcp_socket_session.end())
            {
                tcp_socket_session.erase(index);
                tcp_index_multiplexing.emplace(index);
            }
        }

        TcpSession* TcpServerService::GetSession(const quint32 index)
        {
            if (tcp_socket_session.find(index) != tcp_socket_session.end())
            {
                return tcp_socket_session[index].get();
            }
            return nullptr;
        }

        void TcpServerService::InitializeSession(TcpSession* session, TcpThread* thread)
        {
            if (nullptr != session)
            {
                if (connect(session, &TcpSession::SendDisconsNotify, this, &TcpServerService::OnDisconnect, Qt::QueuedConnection))
                {
                    // 将会话任务送到线程中运行
                    session->moveToThread(thread);
                }
            }
        }

        const quint32 TcpServerService::GetPlexingIndex(quint32 index)
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
            AddSession(GetPlexingIndex(), handler);
        }

        void TcpServerService::OnDisconnect(const quint32 index)
        {
            DelSession(index);
        }
    }
}