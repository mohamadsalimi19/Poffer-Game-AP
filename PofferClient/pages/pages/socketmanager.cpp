#include "socketmanager.h"

SocketManager::SocketManager(QObject *parent)
    : QObject{parent}
{
    mySocket = new QTcpSocket(this);

    // اتصال سیگنال‌ها فقط یک بار
    connect(mySocket, &QTcpSocket::connected, this, &SocketManager::onConnected);
    connect(mySocket, &QTcpSocket::readyRead, this, &SocketManager::onDataRead);
    connect(mySocket, &QTcpSocket::bytesWritten, this, &SocketManager::onWritten);
    connect(mySocket, &QTcpSocket::disconnected, this, &SocketManager::onDisconnected);
}

void SocketManager::connectToServer(QString ip, quint16 port) {
    mySocket->connectToHost(ip, port);
}

void SocketManager::onConnected() {
    qDebug() << "Connected to server!";
}

void SocketManager::sendData(const QByteArray &data) {
    if (mySocket->state() == QAbstractSocket::ConnectedState) {
        mySocket->write(data);
    } else {
        qDebug() << "Socket is not connected.";
    }
}

void SocketManager::onDataRead() {
    QByteArray data = mySocket->readAll();
    qDebug() << "Data read:" << data;
    // اینجا میتونی داده را پردازش کنی یا سیگنال بفرستی
}

void SocketManager::onWritten(qint64 bytes) {
    qDebug() << "Data written, bytes:" << bytes;
}

void SocketManager::onDisconnected() {
    qDebug() << "Disconnected from server.";
}


