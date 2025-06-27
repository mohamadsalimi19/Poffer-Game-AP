#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class SocketManager : public QObject
{
    Q_OBJECT
    QByteArray response;
public:
    explicit SocketManager(QObject *parent = nullptr);

    void connectToServer(QString ip, quint16 port);
    void sendData(const QByteArray &data);
    QByteArray get_response();

signals:
    void connected();
    void disconnected();
    void dataReceived(QByteArray data);
    void dataWritten(qint64 bytes);

private slots:
    void onConnected();
    void onDisconnected();
    void onDataRead();
    void onWritten(qint64 bytes);

private:
    QTcpSocket *mySocket;
};

#endif // SOCKETMANAGER_H
