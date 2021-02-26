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
        class TcpSession : public QTcpSocket, public std::enable_shared_from_this<TcpSession>
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
            void SendDisconsNotify(const std::shared_ptr<TcpSession>& session, const u96 index);
            void SendMessageNotify(const std::shared_ptr<TcpSession>& session, QIODevice* buffer, const u96 index);
            void SendDisplayErrorNotify(const std::shared_ptr<TcpSession>& session, QAbstractSocket::SocketError error, const u96 index);
        private slots:
            // 客户断开
            void OnDiscons();
            // 客户消息
            void OnMessage();
            // 发送回执
            void OnSendmsg(i64 bytes);
            // 错误回执
            void OnDisplayError(QAbstractSocket::SocketError error);
        private:
            void Cleanup();
        private:
            u96                                                             tcp_index;
        };
    }
}
#endif // __TCP_SESSION_H__