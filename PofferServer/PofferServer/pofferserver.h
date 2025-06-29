#ifndef POFFERSERVER_H
#define POFFERSERVER_H

#include <QTcpServer>

class PofferServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit PofferServer(QObject *parent = nullptr);
    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // POFFERSERVER_H
