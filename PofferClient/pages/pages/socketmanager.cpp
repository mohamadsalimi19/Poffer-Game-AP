#include "socketmanager.h"

SocketManager::SocketManager(QObject *parent)
    : QObject{parent}
{
    mySocket = new QTcpSocket(this);
    connect(mySocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(onDataRead()));
    connect(mySocket, SIGNAL(bytesWritten(qint64)), this, SLOT(onWritten(qint64)));
    connect(mySocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
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
    emit dataReceived(data);
    response = data;
    qDebug() << "Data read:" << data;
}

void SocketManager::onWritten(qint64 bytes) {
    qDebug() << "Data written, bytes:" << bytes;
}

void SocketManager::onDisconnected() {
    qDebug() << "Disconnected from server.";
}
QByteArray SocketManager::get_response(){

    return response;


}


