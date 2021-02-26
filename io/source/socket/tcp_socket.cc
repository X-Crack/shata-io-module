#include <tcp_socket.h>

namespace Shata
{
    namespace Socket
    {
        TcpSocket::TcpSocket(const QHostAddress& address, const quint16 port) :
            tcp_address(address),
            tcp_port(port)
        {

        }

        TcpSocket::~TcpSocket()
        {

        }

        void TcpSocket::AddListenPort(const QHostAddress& address, quint16 port)
        {
            tcp_address = address;
            tcp_port = port;
        }

        const QHostAddress& TcpSocket::GetHost()
        {
            return std::cref(tcp_address);
        }

        const quint16 TcpSocket::GetPort()
        {
            return tcp_port;
        }
    }
}