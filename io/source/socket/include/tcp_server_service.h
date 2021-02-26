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
            bool AddListenPort(const quint32 index, const QHostAddress& address, quint16 port);
            bool CreaterServer();

        private:
            void AddSession(const quint32 index, qintptr handler);
            void DelSession(const quint32 index);
            TcpSession* GetSession(const quint32 index);
            void InitializeSession(TcpSession* session, TcpThread* thread);
        private:
            const quint32 GetPlexingIndex(quint32 index = 0);
        private slots:
            void OnConnection(qintptr handler);
            void OnDisconnect(const quint32 index);
        private:
            std::unordered_map<quint32, TcpServer*>                                     tcp_server_pool;
            std::unordered_map<quint32, std::unique_ptr<TcpSocket>>                     tcp_socket_pool;
            std::unique_ptr<TcpThreadPool>                                              tcp_server_thread_pool;
            std::unique_ptr<TcpThreadPool>                                              tcp_session_thread_pool;
            std::unordered_map<quint32, std::unique_ptr<TcpSession>>                    tcp_socket_session;
            std::priority_queue<quint32, std::deque<quint32>, std::greater<quint32>>    tcp_index_multiplexing;
            std::atomic<quint32>                                                        tcp_index;
        };
    }
}

#endif // __TCP_SERVER_SERVICE_H__