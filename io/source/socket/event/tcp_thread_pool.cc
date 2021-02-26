#include <tcp_thread_pool.h>
#include <tcp_thread.h>
namespace Shata
{
    namespace Socket
    {
        TcpThreadPool::TcpThreadPool(QObject* object)
        {

        }

        TcpThreadPool::~TcpThreadPool()
        {

        }

        bool TcpThreadPool::CreaterEventThreadPool(const qint32 size)
        {
            for (qint32 i = 0; i < size; ++i)
            {
                event_thread_pool.emplace(i, std::make_unique<TcpThread>(this));
                event_thread_pool[i]->start();
            }
            return true;
        }

        TcpThread* TcpThreadPool::GetEventThread(const qint32 index)
        {
            return event_thread_pool[index % event_thread_pool.size()].get();
        }
    }
}