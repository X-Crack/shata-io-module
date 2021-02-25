#ifndef __TCP_SESSION_H__
#define __TCP_SESSION_H__
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
namespace Shata
{
    namespace Socket
    {
        class EventLoopThread;
        class TcpSession : public QTcpSocket
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpSession(const quint32 index, qintptr handler, QObject* object = nullptr);
            virtual ~TcpSession();
        public slots:
            bool Send(const char* data, qint64 len);
            bool Send(const char* data);
            bool Send(const QByteArray& data);
        signals:
            void SendDisconsNotify(const quint32 index);
        private slots:
            void OnDiscons();
            void OnMessage();
            void OnSendmsg(qint64 bytes);
            void OnDisplayError(QAbstractSocket::SocketError ex);
        private:
            quint32                                                             tcp_index;
            /// connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)),Qt::QueuedConnection);
        };
    }
}
#endif // __TCP_SESSION_H__