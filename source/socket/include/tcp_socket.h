﻿#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__
/*!
 *
 * file: tcp_socket.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/26
 *
 */
#include <event_config.h>
namespace Shata
{
    namespace Socket
    {
        class TCP_SERVICE_EXPORT TcpSocket
        {
        public:
            explicit TcpSocket(const QHostAddress& address, const u16 port);
            virtual ~TcpSocket();
        public:
            void AddListenPort(const QHostAddress& address, u16 port);
            const QHostAddress& GetHost();
            const u16 GetPort();
        private:
            QHostAddress                                        tcp_address;
            u16                                                 tcp_port;
        };
    }
}
#endif // __TCP_SOCKET_H__