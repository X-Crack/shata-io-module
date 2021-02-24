#include <tcp_server.h>
namespace Shata
{
    namespace Socket
    {
        TcpServer::TcpServer(QObject* object) :
            QTcpServer(object)
        {

        }

        TcpServer::~TcpServer()
        {

        }

        void TcpServer::incomingConnection(qintptr handler)
        {
            emit SendConnectionNotify(handler);
        }
    }
}