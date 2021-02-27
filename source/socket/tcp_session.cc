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
            if (setSocketDescriptor(handler))
            {
                connect(this, &TcpSession::SendCloseNotify,         this, &TcpSession::Close, Qt::QueuedConnection);

                connect(this, &QTcpSocket::readyRead,               this, &TcpSession::OnMessage, Qt::QueuedConnection);
                connect(this, &QTcpSocket::bytesWritten,            this, &TcpSession::OnSendmsg, Qt::QueuedConnection);
                connect(this, &QTcpSocket::disconnected,            this, &TcpSession::OnDiscons, Qt::QueuedConnection);
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
        
        bool TcpSession::Send(const char* data, const i64 len)
        {
            if (QAbstractSocket::isValid())
            {
                return len == QAbstractSocket::write(data, len);
            }
            return false;
        }

        bool TcpSession::Send(const char* data)
        {
            if (QAbstractSocket::isValid())
            {
                return strlen(data) == QAbstractSocket::write(data);
            }
            return false;
        }

        bool TcpSession::Send(const std::string& data)
        {
            if (QAbstractSocket::isValid())
            {
                return data.size() == QAbstractSocket::write(data.c_str(), data.size());
            }
            return false;
        }

        bool TcpSession::Send(const QByteArray& data)
        {
            if (QAbstractSocket::isValid())
            {
                return data.size() == QAbstractSocket::write(data); // 发送超大数据时可能会进行分块发送，QT WRITE 内部实现机制暂不了解，如有问题在解决问题。
            }
            return false;
        }

        void TcpSession::Close()
        {
            if (QAbstractSocket::isValid() && QAbstractSocket::SocketState::ConnectedState == state())
            {
                return QAbstractSocket::close();
            }
        }

        void TcpSession::OnDiscons()
        {
            emit SendDisconsNotify(shared_from_this(), tcp_index);
        }

        void TcpSession::OnMessage()
        {
            if (QAbstractSocket::isValid())
            {
                if (this == qobject_cast<TcpSession*>(sender()))
                {
                    emit SendMessageNotify(shared_from_this(), this, tcp_index);
                }
            }
        }

        void TcpSession::OnSendmsg(i64 bytes)
        {
            emit SendReceiptNotify(shared_from_this(), bytes, tcp_index);
        }

        void TcpSession::OnDisplayError(QAbstractSocket::SocketError error)
        {
            /*
            QAbstractSocket::ConnectionRefusedError	                     0	该连接被对等方拒绝（或超时）。
            QAbstractSocket::RemoteHostClosedError	                     1	远程主机关闭了连接。请注意，客户端套接字（即此套接字）将在发送远程关闭通知后关闭。
            QAbstractSocket::HostNotFoundError	                         2	找不到主机地址。
            QAbstractSocket::SocketAccessError	                         3	套接字操作失败，因为该应用程序缺少必需的特权。
            QAbstractSocket::SocketResourceError	                     4	本地系统资源不足（例如，套接字过多）。
            QAbstractSocket::SocketTimeoutError	                         5	套接字操作超时。
            QAbstractSocket::DatagramTooLargeError	                     6	数据报大于操作系统的限制（可以低至8192字节）。
            QAbstractSocket::NetworkError	                             7	网络发生错误（例如，网络电缆意外插入）。
            QAbstractSocket::AddressInUseError	                         8	指定给QAbstractSocket :: bind（）的地址已经在使用中，并且被设置为互斥的。
            QAbstractSocket::SocketAddressNotAvailableError	             9	QAbstractSocket :: bind（）指定的地址不属于主机。
            QAbstractSocket::UnsupportedSocketOperationError	        10	本地操作系统不支持请求的套接字操作（例如，缺少IPv6支持）。
            QAbstractSocket::ProxyAuthenticationRequiredError	        12	套接字正在使用代理，并且代理需要身份验证。
            QAbstractSocket::SslHandshakeFailedError	                13	SSL / TLS握手失败，因此连接已关闭（仅在QSslSocket中使用）
            QAbstractSocket::UnfinishedSocketOperationError	            11	仅由QAbstractSocketEngine使用，上一次尝试的操作尚未完成（仍在后台进行）。
            QAbstractSocket::ProxyConnectionRefusedError	            14	无法连接到代理服务器，因为与该服务器的连接被拒绝
            QAbstractSocket::ProxyConnectionClosedError	                15	与代理服务器的连接意外关闭（在建立与最终对等方的连接之前）
            QAbstractSocket::ProxyConnectionTimeoutError	            16	与代理服务器的连接超时或代理服务器在身份验证阶段停止响应。
            QAbstractSocket::ProxyNotFoundError	                        17	找不到用setProxy（）（或应用程序代理）设置的代理地址。
            QAbstractSocket::ProxyProtocolError	                        18	与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应。
            QAbstractSocket::OperationError	                            19	套接字处于不允许状态时尝试执行操作。
            QAbstractSocket::SslInternalError	                        20	使用的SSL库报告内部错误。这可能是由于库安装错误或配置错误造成的。
            QAbstractSocket::SslInvalidUserDataError	                21	提供了无效的数据（证书，密钥，密码等），其使用导致SSL库中的错误。
            QAbstractSocket::TemporaryError	                            22	发生临时错误（例如，操作将阻塞并且套接字未阻塞）。
            QAbstractSocket::UnknownSocketError	                        -1	发生无法识别的错误。
            */

            switch (error)
            {
            case QAbstractSocket::SocketError::RemoteHostClosedError: return;
            case QAbstractSocket::SocketError::HostNotFoundError:
            case QAbstractSocket::SocketError::SocketResourceError:
            case QAbstractSocket::SocketError::SocketTimeoutError: Close(); return;
            }

            if (QAbstractSocket::isValid())
            {
                emit SendDisplayErrorNotify(shared_from_this(), error, tcp_index);
            }
        }

        void TcpSession::Cleanup()
        {
            setSocketState(SocketState::UnconnectedState);

            disconnect(this, &QTcpSocket::readyRead,                this, &TcpSession::OnMessage);
            disconnect(this, &QTcpSocket::disconnected,             this, &TcpSession::OnDiscons);
            disconnect(this, &QTcpSocket::bytesWritten,             this, &TcpSession::OnSendmsg);
#if QT_DEPRECATED_SINCE(5, 0)
            disconnect(this, static_cast<void(QAbstractSocket::*)(SocketError)>(&QAbstractSocket::error), this, &TcpSession::OnDisplayError);
#else
            disconnect(this, &QAbstractSocket::errorOccurred,       this, &TcpSession::OnDisplayError);
#endif
        }
    }
}