#include <QCoreApplication>
#include "pofferserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PofferServer myServer;
    myServer.startServer(8888);

    return a.exec();
}
