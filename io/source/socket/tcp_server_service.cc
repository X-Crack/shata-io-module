#include <tcp_server_service.h>
#include <tcp_listen.h>
#include <tcp_server.h>
#include <event_loop_thread_pool.h>
#include <event_loop_thread.h>
#include <QtCore/QThread>
namespace Shata
{
    namespace Socket
    {
        TcpServerService::TcpServerService() : 
            event_thread_pool(std::make_unique<EventLoopThreadPool>())
        {

        }

        TcpServerService::~TcpServerService()
        {

        }

        void TcpServerService::AddListenPort(const quint32 index, const QHostAddress& address, quint16 port)
        {

            tcp_server_pool.emplace(index, std::make_unique<TcpListen>(this));
            // Î±´úÂë
            tcp_server_pool[index]->moveToThread(event_thread_pool->GetEventThread(index));
        }
    }
}