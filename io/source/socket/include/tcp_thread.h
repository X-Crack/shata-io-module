#ifndef __EVENT_LOOP_THREAD_H__
#define __EVENT_LOOP_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QObject>
namespace Shata
{
    namespace Socket
    {
        class TcpThread : public QThread
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