#include <tcp_server.h>
#include <tcp_listen.h>
#include <windows.h>
namespace Shata
{
    namespace Socket
    {
        TcpServer::TcpServer(QObject* object) :
            QTcpServer(object),
            tcp_listen(std::make_unique<TcpListen>(this))
        {

        }

        TcpServer::~TcpServer()
        {

        }

        bool TcpServer::CreaterServer(const QHostAddress& address, u16 port)
        {
            if (tcp_listen->CreaterListen(address, port))
            {
                return true;
            }
            return false;
        }

        bool TcpServer::DestroyServer()
        {
            return tcp_listen->DestroyListen();
        }

        void TcpServer::incomingConnection(qintptr handler)
        {
            emit SendConnectionNotify(handler);
        }
    }
}