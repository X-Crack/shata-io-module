#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__
#include <event_config.h>
#include <QtNetwork/QHostAddress>
namespace Shata
{
    namespace Socket
    {
        class TcpSocket
        {
        public:
            explicit TcpSocket(const QHostAddress& address, const quint16 port);
            virtual ~TcpSocket();
        public:
            void AddListenPort(const QHostAddress& address, quint16 port);
            const QHostAddress& GetHost();
            const quint16 GetPort();
        private:
            QHostAddress                                        tcp_address;
            quint16                                             tcp_port;
        };
    }
}
#endif // __TCP_SOCKET_H__