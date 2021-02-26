#include <tcp_session.h>
#include <windows.h>
namespace Shata
{
    namespace Socket
    {
        TcpSession::TcpSession(const u96 index, qintptr handler, QObject* object) :
            tcp_index(index),
            QTcpSocket(object)
        {

            qRegisterMetaType<std::shared_ptr<TcpSession>>("std::shared_ptr<TcpSession>");

            if (setSocketDescriptor(handler))
            {
                connect(this, &QTcpSocket::readyRead,               this, &TcpSession::OnMessage, Qt::QueuedConnection);
                connect(this, &QTcpSocket::disconnected,            this, &TcpSession::OnDiscons, Qt::QueuedConnection);
                //connect(this, &QTcpSocket::bytesWritten,            this, &TcpSession::OnSendmsg, Qt::QueuedConnection);
#if QT_DEPRECATED_SINCE(5, 0)
                connect(this, static_cast<void(QAbstractSocket::*)(SocketError)>(&QAbstractSocket::error), this, &TcpSession::OnDisplayError, Qt::QueuedConnection);
#else
                connect(this, &QAbstractSocket::errorOccurred,      this, &TcpSession::OnDisplayError, Qt::QueuedConnection);
#endif
            }
        }

        TcpSession::~TcpSession()
        {
            Cleanup();
        }
        
        bool TcpSession::Send(const char* data, qint64 len)
        {
            return len == write(data, len); 
        }

        bool TcpSession::Send(const char* data)
        {
            return strlen(data) == write(data);
        }

        bool TcpSession::Send(const std::string& data)
        {
            return data.size() == write(data.c_str(), data.size());
        }

        bool TcpSession::Send(const QByteArray& data)
        {
            return data.size() == write(data); // 发送超大数据时可能会进行分块发送，QT WRITE 内部实现机制暂不了解，如有问题在解决问题。
        }

        void TcpSession::OnDiscons()
        {
            emit SendDisconsNotify(shared_from_this(), tcp_index);
        }

        void TcpSession::OnMessage()
        {
            if (this == qobject_cast<TcpSession*>(sender()))
            {
                //Send(QIODevice::read(QIODevice::size()));
                emit SendMessageNotify(shared_from_this(), this, tcp_index);
            }
            else
            {
                // 不会出现这种错误，除非有人篡改程序。
                emit SendMessageNotify(shared_from_this(), qobject_cast<TcpSession*>(sender()), tcp_index);
            }
        }

        void TcpSession::OnSendmsg(i64 bytes)
        {

        }

        void TcpSession::OnDisplayError(QAbstractSocket::SocketError ex)
        {

        }

        void TcpSession::Cleanup()
        {
            setSocketState(SocketState::ClosingState);

            disconnect(this, &QTcpSocket::readyRead,                this, &TcpSession::OnMessage);
            disconnect(this, &QTcpSocket::disconnected,             this, &TcpSession::OnDiscons);
            //connect(this, &QTcpSocket::bytesWritten,              this, &TcpSession::OnSendmsg);
#if QT_DEPRECATED_SINCE(5, 0)
            disconnect(this, static_cast<void(QAbstractSocket::*)(SocketError)>(&QAbstractSocket::error), this, &TcpSession::OnDisplayError);
#else
            disconnect(this, &QAbstractSocket::errorOccurred,       this, &TcpSession::OnDisplayError);
#endif
        }
    }
}