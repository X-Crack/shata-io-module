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
#include <event_config.h>
#include <memory>
namespace Shata
{
    namespace Socket
    {
        class TcpListen;
        class TCP_SERVICE_EXPORT TcpServer : public QTcpServer
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpServer(QObject* object = nullptr);
            virtual ~TcpServer();
        public slots:
            bool CreaterServer(const QHostAddress& address, u16 port);
            bool DestroyServer();
        protected:
            virtual void incomingConnection(qintptr handler) override;
        signals:
            void SendConnectionNotify(qintptr handler);
        private:
            std::unique_ptr<TcpListen>                                      tcp_listen;
        };
    }
}
#endif // __TCP_SERVER_H__