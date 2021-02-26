#ifndef __TCP_SESSION_H__
#define __TCP_SESSION_H__
#include <event_config.h>
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
            explicit TcpSession(const u96 index, qintptr handler, QObject* object = nullptr);
            virtual ~TcpSession();
        public slots:
            // 使用槽来定义 Send 未来可能会用到不同线程调用，方便绑定。
            bool Send(const char* data, qint64 len);
            bool Send(const char* data);
            bool Send(const std::string& data);
            bool Send(const QByteArray& data);
        signals:
            // 客户断开通知信号
            void SendDisconsNotify(const u96 index);
        private:
            // 客户断开
            void OnDiscons();
            // 客户消息
            void OnMessage();
            // 发送回执
            void OnSendmsg(qint64 bytes);
            // 错误回执
            void OnDisplayError(QAbstractSocket::SocketError ex);
        private:
            quint32                                                             tcp_index;
        };
    }
}
#endif // __TCP_SESSION_H__