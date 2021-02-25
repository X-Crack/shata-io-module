#include <tcp_session.h>
#include <windows.h>
namespace Shata
{
    namespace Socket
    {
        TcpSession::TcpSession(const quint32 index, qintptr handler, QObject* object) :
            tcp_index(index),
            QTcpSocket(object)
        {
            if (setSocketDescriptor(handler))
            {
                connect(this, &QTcpSocket::readyRead,               this, &TcpSession::OnMessage, Qt::QueuedConnection);
                connect(this, &QTcpSocket::disconnected,            this, &TcpSession::OnDiscons, Qt::QueuedConnection);
                connect(this, &QTcpSocket::bytesWritten,            this, &TcpSession::OnSendmsg, Qt::QueuedConnection);
#if QT_DEPRECATED_SINCE(5, 0)
                connect(this, static_cast<void(QAbstractSocket::*)(SocketError)>(&QAbstractSocket::error), this, &TcpSession::OnDisplayError, Qt::QueuedConnection);
#else
                connect(this, &QAbstractSocket::errorOccurred,      this, &TcpSession::OnDisplayError, Qt::QueuedConnection);
#endif
            }
        }

        TcpSession::~TcpSession()
        {

        }

        bool TcpSession::Send(const char* data, qint64 len)
        {
            write(data, len);
            return true;
        }

        bool TcpSession::Send(const char* data)
        {
            write(data);
            return true;
        }

        bool TcpSession::Send(const QByteArray& data)
        {
            write(data);
            return true;
        }

        void TcpSession::OnDiscons()
        {
            emit SendDisconsNotify(tcp_index);
        }

        void TcpSession::OnMessage()
        {
            TcpSession* this_socket = qobject_cast<TcpSession*>(sender());
            printf("用户：%d 数据长度：%lld\n", this_socket->tcp_index, this_socket->size());
            this_socket->read(this_socket->size());
        }

        void TcpSession::OnSendmsg(qint64 bytes)
        {
            
        }

        void TcpSession::OnDisplayError(QAbstractSocket::SocketError ex)
        {

        }
    }
}