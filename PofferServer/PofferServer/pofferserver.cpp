#include "pofferserver.h"
#include "clienthandler.h"
#include <QThread>
#include <QDebug>
#include <QNetworkInterface>

PofferServer::PofferServer(QObject *parent) : QTcpServer(parent)
{
}

// void PofferServer::startServer(quint16 port)
// {
//     if (this->listen(QHostAddress::Any, port)) {
//         qDebug() << "Server started on port" << port;
//     } else {
//         qDebug() << "Server could not start!";
//     }
// }

void PofferServer::startServer(quint16 port)
{
    // listen to all ip
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started successfully.";

        //valid ip
        QString localIp = "127.0.0.1";
        const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : ipAddressesList) {
            // ip that is ipv4
            if (address != QHostAddress::LocalHost && address.toIPv4Address()) {
                localIp = address.toString();
                break;
            }
        }

        // print
        qDebug() << "========================================";
        qDebug() << "Server is listening on:";
        qDebug() << "IP:  " << localIp;
        qDebug() << "Port:" << port;
        qDebug() << "========================================";
        qDebug() << "Waiting for clients to connect...";

    } else {
        qCritical() << "Server could not start! Error:" << this->errorString();
    }
}

// this is a virtual function in qtcpsocket
void PofferServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "New client connecting..." << socketDescriptor;

    QThread* thread = new QThread;
    ClientHandler* handler = new ClientHandler(socketDescriptor);
    handler->moveToThread(thread);

    connect(thread, &QThread::started, handler, &ClientHandler::process);
    connect(handler, &ClientHandler::finished, thread, &QThread::quit);
    // garantin delete space safety after end
    connect(thread, &QThread::finished, handler, &ClientHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    // connection game chat player
    connect(handler, &ClientHandler::chatMessageReceived, GameManager::instance(), &GameManager::onChatMessageReceived);

    thread->start();
}
