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
#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>
#include <unordered_map>
#include <memory>
namespace Shata
{
    namespace Socket
    {
        class EventLoopThreadPool;
        class TcpListen;
        class TcpServerService
        {
        public:
            explicit TcpServerService();
            virtual ~TcpServerService();
        public:
            void AddListenPort(const quint32 index, const QHostAddress& address, quint16 port);
        private:
            std::unordered_map<quint32, std::unique_ptr<TcpListen>>                     tcp_server_pool;
            std::unique_ptr<EventLoopThreadPool>                                        event_thread_pool;
        };
    }
}

#endif // __TCP_SERVER_SERVICE_H__