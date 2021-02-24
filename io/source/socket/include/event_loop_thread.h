#ifndef __EVENT_LOOP_THREAD_H__
#define __EVENT_LOOP_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QObject>
namespace Shata
{
    namespace Socket
    {
        class EventLoopThread : public QThread
        {
        public:
            Q_OBJECT;
        public:
            explicit EventLoopThread(QObject* object = nullptr);
            virtual ~EventLoopThread();
        public:
        private:
            virtual void run() override;
        };
    }
}
#endif // __EVENT_LOOP_THREAD_H__