#ifndef __TCP_THREAD_H__
#define __TCP_THREAD_H__
/*!
 *
 * file: tcp_thread.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/26
 *
 */
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
#endif // __TCP_THREAD_H__