#include <tcp_server_service.h>
#include <tcp_session.h>
namespace Shata
{
    namespace Socket
    {
        bool OnConnection(const std::shared_ptr<TcpSession>& session, const u96 index)
        {
            printf("用户进入：%d\n", index);
            return true;
        }

        bool OnDisconnect(const std::shared_ptr<TcpSession>& session, const u96 index)
        {
            printf("用户离开：%d\n", index);
            return true;
        }

        bool OnMessage(const std::shared_ptr<TcpSession>& session, QIODevice* buffer, const u96 index)
        {
            printf("用户消息：%d 消息长度：%lld\n", index, buffer->size());
            session->Send(buffer->read(buffer->size()));
            return true;
        }

        bool OnSendReceipt(const std::shared_ptr<TcpSession>& session, const i64 bytes, const u96 index)
        {
            printf("发送消息：%d 消息长度：%lld\n", index, bytes);
            return true;
        }

        bool OnDisplayError(const std::shared_ptr<TcpSession>& session, QAbstractSocket::SocketError error, const u96 index)
        {
            printf("系统错误：%d 错误代码：%d\n", index, error);
            return true;
        }
    }
}

int main(int argc, char *argv[])
{
    using namespace Shata::Socket;
    QCoreApplication a(argc, argv);
    printf("主线程：%d\n", GetCurrentThreadId());
    TcpServerService server; 
    server.AddListenPort(0, QHostAddress("0.0.0.0"), 8888);
    server.AddListenPort(1, QHostAddress("::"), 8888);
    server.AddListenPort(2, QHostAddress("0.0.0.0"), 9999);
    server.SetConnectionCallback(Shata::Socket::OnConnection);
    server.SetDisconnectCallback(Shata::Socket::OnDisconnect);
    server.SetMessageCallback(Shata::Socket::OnMessage);
    server.SetSendReceiptCallback(Shata::Socket::OnSendReceipt);
    server.SetDisplayErrorCallback(Shata::Socket::OnDisplayError);
    server.CreaterServer();
    return a.exec();
}