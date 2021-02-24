#include <tcp_listen.h>
#include <tcp_server.h>
namespace Shata
{
    namespace Socket
    {
        TcpListen::TcpListen(const quint64 index, QObject* object) :
            tcp_server_index(index),
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

        bool TcpListen::CreaterListen(const QHostAddress& address, quint16 port)
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

        bool TcpListen::CreaterListen(QHostAddress* address, quint16 port)
        {
            return CreaterListen(std::move(std::ref(*address)), port);
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