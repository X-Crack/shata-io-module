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
            explicit TcpListen(const quint64 index, QObject* object = nullptr);
            virtual ~TcpListen();
        public slots:
            bool CreaterListen(const QHostAddress& address, quint16 port);
            bool CreaterListen(QHostAddress* address, quint16 port);
            bool DestroyListen();
        signals:
            void SendConnectionNotify(qintptr handler);
        private:
            TcpServer*                                                  tcp_server;
            quint64                                                     tcp_server_index;
        };
    }
}
#endif // __tcp_listen_H__