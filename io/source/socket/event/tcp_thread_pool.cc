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

        bool TcpThreadPool::CreaterThreadPool(const u96 size)
        {
            for (u96 i = 0; i < size; ++i)
            {
                event_thread_pool.emplace(i, std::make_unique<TcpThread>(this));
                event_thread_pool[i]->start();
            }
            return true;
        }

        bool TcpThreadPool::DestroyThreadPool()
        {
            for (auto& [index, thread] : event_thread_pool)
            {
                if (thread->isRunning())
                {
                    thread->quit();
                }
                
                if (thread->wait(10000))
                {
                    continue;
                }
                else
                {
                    // timer out
                }
                return false;
            }
            return true;
        }

        TcpThread* TcpThreadPool::GetEventThread(const u96 index)
        {
            return event_thread_pool[index % event_thread_pool.size()].get();
        }
    }
}