#include <tcp_thread.h>

namespace Shata
{
    namespace Socket
    {
        TcpThread::TcpThread(QObject* object) :
            QThread(object)
        {

        }

        TcpThread::~TcpThread()
        {

        }

        void TcpThread::run()
        {
            // 阻塞模式运行消息循环，防止线程自我销毁（外部调用 quit wait 安全结束线程）。
            exec();
        }
    }
}