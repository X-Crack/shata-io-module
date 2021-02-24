#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    return a.exec();
}