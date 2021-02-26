#ifndef __EVENT_LOOP_THREAD_H__
#define __EVENT_LOOP_THREAD_H__
#include <event_config.h>
namespace Shata
{
    namespace Socket
    {
        class TCP_SERVICE_EXPORT TcpThread : public QThread
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpThread(QObject* object = nullptr);
            virtual ~TcpThread();
        public:
        private:
            virtual void run() override;
        };
    }
}
#endif // __EVENT_LOOP_THREAD_H__