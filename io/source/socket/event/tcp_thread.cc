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
            // ����ģʽ������Ϣѭ������ֹ�߳��������٣��ⲿ���� quit wait ��ȫ�����̣߳���
            exec();
        }
    }
}