#ifndef __EVENT_LOOP_THREAD_POOL_H__
#define __EVENT_LOOP_THREAD_POOL_H__
#include <event_config.h>
#include <QtCore/QThreadPool>
#include <unordered_map>
#include <memory>
namespace Shata
{
    namespace Socket
    {
        class TcpThread;
        class TcpThreadPool : public QObject
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpThreadPool(QObject* object = nullptr);
            virtual ~TcpThreadPool();
        public:
            bool CreaterEventThreadPool(const qint32 size);
            TcpThread* GetEventThread(const qint32 index);
        private:
            std::unordered_map<qint32, std::unique_ptr<TcpThread>>                    event_thread_pool;
        };
    }
}
#endif // __EVENT_LOOP_THREAD_POOL_H__