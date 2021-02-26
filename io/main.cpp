#include <QtCore/QCoreApplication>
#include <windows.h>
#include <tcp_server_service.h>
#include <tcp_server.h>
#include <tcp_listen.h>
int main(int argc, char *argv[])
{
    using namespace Shata::Socket;
    QCoreApplication a(argc, argv);
    printf("主线程：%d\n", GetCurrentThreadId());
    TcpServerService server;
    server.AddListenPort(0, QHostAddress("0.0.0.0"), 8888);
    server.AddListenPort(1, QHostAddress("::"), 8888);
    server.AddListenPort(2, QHostAddress("0.0.0.0"), 9999);
    server.CreaterServer();
    return a.exec();
}