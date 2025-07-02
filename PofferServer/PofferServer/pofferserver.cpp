#include "pofferserver.h"
#include "clienthandler.h"
#include <QThread>
#include <QDebug>

PofferServer::PofferServer(QObject *parent) : QTcpServer(parent)
{
}

void PofferServer::startServer(quint16 port)
{
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Server could not start!";
    }
}


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
