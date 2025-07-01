#include "socketmanager.h"
#include"QMessageBox"
#include"QJsonObject"
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
    QMessageBox::warning(nullptr,"con","connected");
    emit connected();
}

void SocketManager::sendData(const QByteArray &data) {
        mySocket->write(data);


}

void SocketManager::onDataRead() {
    buffer.append(mySocket->readAll());
    while (true) {
        int endLineIndex = buffer.indexOf('\n');
        if (endLineIndex == -1)
            break; // هنوز یک پیام کامل دریافت نشده

        QByteArray jsonData = buffer.left(endLineIndex);
        buffer = buffer.mid(endLineIndex + 1); // حذف پیام پردازش شده

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull()) {
            emit dataReceived(jsonData);
            qDebug() << "Received JSON:" << jsonData;
        } else {
            qDebug() << "Invalid JSON received:" << jsonData;
        }
    }
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


