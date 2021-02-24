#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
/*!
 *
 * file: tcp_server.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/24
 *
 */
#include <QtNetwork/QTcpServer>
namespace Shata
{
    namespace Socket
    {
        class TcpServer : public QTcpServer
        {
        public:
            Q_OBJECT
        public:
            explicit TcpServer(QObject* object);
            virtual ~TcpServer();
        protected:
            virtual void incomingConnection(qintptr handler) override;
        signals:
            void SendConnectionNotify(qintptr handler);
        };
    }
}
#endif // __TCP_SERVER_H__