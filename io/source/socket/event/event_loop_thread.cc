#include <event_loop_thread.h>

namespace Shata
{
    namespace Socket
    {
        EventLoopThread::EventLoopThread(QObject* object) : 
            QThread(object)
        {

        }

        EventLoopThread::~EventLoopThread()
        {

        }

        void EventLoopThread::run()
        {

        }
    }
}