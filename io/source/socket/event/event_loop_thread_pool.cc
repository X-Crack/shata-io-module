#include <event_loop_thread_pool.h>
#include <event_loop_thread.h>
namespace Shata
{
    namespace Socket
    {
        EventLoopThreadPool::EventLoopThreadPool(QObject* object)
        {

        }

        EventLoopThreadPool::~EventLoopThreadPool()
        {

        }

        bool EventLoopThreadPool::CreaterEventThreadPool(const qint32 size)
        {
            for (qint32 i = 0; i < size; ++i)
            {
                event_thread_pool.emplace(i, std::make_unique<EventLoopThread>(this));
            }
            return true;
        }

        EventLoopThread* EventLoopThreadPool::GetEventThread(const qint32 index)
        {
            return event_thread_pool[index % event_thread_pool.size()].get();
        }
    }
}