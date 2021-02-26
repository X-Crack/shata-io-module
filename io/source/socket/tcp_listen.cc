#include <tcp_listen.h>
#include <tcp_server.h>
namespace Shata
{
    namespace Socket
    {
        TcpListen::TcpListen(TcpServer* server) :
            tcp_server(server),
            QObject(server)
        {

        }

        TcpListen::~TcpListen()
        {

        }

        bool TcpListen::CreaterListen(const QHostAddress& address, quint16 port)
        {
            return tcp_server->listen(address, port);
        }
    }
}