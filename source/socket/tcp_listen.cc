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

        bool TcpListen::CreaterListen(const QHostAddress& address, u16 port)
        {
            if (tcp_server->isListening())
            {
                return true;
            }
            return tcp_server->listen(address, port);
        }

        bool TcpListen::DestroyListen()
        {
            if (nullptr != tcp_server)
            {
                if (tcp_server->isListening())
                {
                    tcp_server->close();
                }
                return true;
            }
            return false;
        }
    }
}