#include <tcp_listen.h>
#include <tcp_server.h>
namespace Shata
{
    namespace Socket
    {
        TcpListen::TcpListen(QObject* object) : 
            QObject(object),
            tcp_server(new TcpServer(this))
        {

        }

        TcpListen::~TcpListen()
        {
            if (nullptr != tcp_server)
            {
                delete tcp_server;
                tcp_server = nullptr;
            }
        }

        bool TcpListen::CreaterListen(QHostAddress address, quint16 port)
        {
            if (nullptr != tcp_server)
            {
                if (connect(tcp_server, &TcpServer::SendConnectionNotify, this, &TcpListen::SendConnectionNotify, Qt::QueuedConnection))
                {
                    if (tcp_server->listen(address, port))
                    {
                        return true;
                    }
                    return disconnect(tcp_server, &TcpServer::SendConnectionNotify, this, &TcpListen::SendConnectionNotify);
                }
            }
            return false;
        }

        bool TcpListen::DestroyListen()
        {
            if (nullptr != tcp_server)
            {
                tcp_server->close();
            }

            if (nullptr != tcp_server)
            {
                return disconnect(tcp_server, &TcpServer::SendConnectionNotify, this, &TcpListen::SendConnectionNotify);
            }
            
            return false;
        }
    }
}