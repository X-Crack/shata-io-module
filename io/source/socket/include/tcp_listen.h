#ifndef __TCP_LISTEN_H__
#define __TCP_LISTEN_H__
/*!
 *
 * file: tcp_listen.h
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
namespace Shata
{
    namespace Socket
    {
        class TcpServer;
        class TcpListen : public QObject
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpListen(TcpServer* server);
            virtual ~TcpListen();
        public:
            bool CreaterListen(const QHostAddress& address, quint16 port);
        private:
            TcpServer*                                                  tcp_server;
        };
    }
}
#endif // __tcp_listen_H__