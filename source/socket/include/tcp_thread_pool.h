#ifndef __TCP_THREAD_POOL_H__
#define __TCP_THREAD_POOL_H__
/*!
 *
 * file: tcp_thread_pool.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/26
 *
 */
#include <event_config.h>
#include <unordered_map>
#include <memory>
namespace Shata
{
    namespace Socket
    {
        class TcpThread;
        class TCP_SERVICE_EXPORT TcpThreadPool : public QObject
        {
        public:
            Q_OBJECT;
        public:
            explicit TcpThreadPool(QObject* object = nullptr);
            virtual ~TcpThreadPool();
        public:
            bool CreaterThreadPool(const u96 size);
            bool DestroyThreadPool();
            TcpThread* GetEventThread(const u96 index);
        private:
            std::unordered_map<u96, std::unique_ptr<TcpThread>>                    event_thread_pool;
        };
    }
}
#endif // __TCP_THREAD_POOL_H__