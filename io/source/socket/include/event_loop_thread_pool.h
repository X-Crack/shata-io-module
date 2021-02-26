#ifndef __EVENT_LOOP_THREAD_POOL_H__
#define __EVENT_LOOP_THREAD_POOL_H__
#include <QtCore/QThreadPool>
#include <unordered_map>
#include <memory>
namespace Shata
{
    namespace Socket
    {
        class EventLoopThread;
        class EventLoopThreadPool : public QObject
        {
        public:
            Q_OBJECT;
        public:
            explicit EventLoopThreadPool(QObject* object = nullptr);
            virtual ~EventLoopThreadPool();
        public:
            void CreaterEventThreadPool(const qint32 size);
            EventLoopThread* GetEventThread(const qint32 index);
        private:
            void InitalizeEventThread(const quint32 index);
        private:
            std::unordered_map<qint32, std::unique_ptr<EventLoopThread>>                    event_thread_pool;
        };
    }
}
#endif // __EVENT_LOOP_THREAD_POOL_H__