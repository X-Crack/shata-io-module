#ifndef __TCP_SERVER_SERVICE_H__
#define __TCP_SERVER_SERVICE_H__
/*!
 *
 * file: tcp_server_service.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/24
 *
 */
#include <event_config.h>
#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <queue>
namespace Shata
{
    namespace Socket
    {
        class TcpThread;
        class TcpThreadPool;
        class TcpServer;
        class TcpListen;
        class TcpSocket;
        class TcpSession;
        class TcpServerService : public QObject
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpServerService();
            virtual ~TcpServerService();
        public:
            friend TcpServer;
        public:
            bool AddListenPort(const u96 index, const QHostAddress& address, quint16 port);
            bool CreaterServer();
            bool DestroyServer();
        public:
            void SetConnectionCallback(const InterfaceConnection& callback);
            void SetDisconnectCallback(const InterfaceDisconnect& callback);
            void SetMessageCallback(const InterfaceMessage& callback);
            void SetDisplayErrorCallback(const InterfaceDisplayError& callback);
        private:
            void AddSession(const u96 index, qintptr handler);
            void DelSession(const u96 index);
            const std::shared_ptr<TcpSession>& GetSession(const u96 index);
            void CreaterSession(TcpSession* session, TcpThread* thread, const u96 index);
            bool DestroySession(TcpSession* session);
        private:
            const quint32 GetPlexingIndex(u96 index = 0);
        private slots:
            void OnConnection(qintptr handler);
            void OnDisconnect(const std::shared_ptr<TcpSession>& session, const u96 index);
            void OnMessage(const std::shared_ptr<TcpSession>& session, QIODevice* buffer, const u96 index);
            void OnDisplayError(const std::shared_ptr<TcpSession>& session, QAbstractSocket::SocketError error, const u96 index);
        private:
            std::unordered_map<u96, std::unique_ptr<TcpServer>>                     tcp_server_pool;
            std::unordered_map<u96, std::unique_ptr<TcpSocket>>                     tcp_socket_pool;
            std::unique_ptr<TcpThreadPool>                                          tcp_server_thread_pool;
            std::unique_ptr<TcpThreadPool>                                          tcp_session_thread_pool;
            std::unordered_map<u96, std::shared_ptr<TcpSession>>                    tcp_socket_session;
            std::priority_queue<u96, std::deque<quint32>, std::greater<quint32>>    tcp_index_multiplexing;
            std::atomic<u96>                                                        tcp_index;
            InterfaceConnection                                                     tcp_connection_callback;
            InterfaceDisconnect                                                     tcp_disconnect_callback;
            InterfaceMessage                                                        tcp_message_callback;
            InterfaceDisplayError                                                   tcp_error_callback;
        };
    }
}

#endif // __TCP_SERVER_SERVICE_H__