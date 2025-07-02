#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include "gamemanager.h"
using namespace std;

class Player;
class GameSession;

class ClientHandler_exception : public exception{
public:
    ClientHandler_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};



class ClientHandler : public QObject
{
    Q_OBJECT


private:
    void processMessage(const QJsonObject& message);

    // function to process commands
    void handleLogin(const QJsonObject& payload);
    void handleSignup(const QJsonObject& payload);
    void handleRequestGame(const QJsonObject& payload);
    void handleSelectCard(const QJsonObject& payload);
    void handleEditProfile(const QJsonObject& payload);
    void handleRequestProfileData(const QJsonObject& payload);
    // values
    qintptr m_socketDescriptor; // a pointer
    QTcpSocket* m_socket = nullptr;
    Player* m_player = nullptr;
    GameSession* m_gameSession = nullptr; // check player in game

public:
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr);
    virtual ~ClientHandler();
    //Notifying the client to start the game
    //void startGame(GameSession* session);
    //void sendJson(const QJsonObject& json);

public slots:
    void process(); // main function that
    void onReadyRead();
    void onDisconnected();
    void startGame(GameSession* session);
    void sendJson(const QJsonObject& json);

signals:
    void finished();
    void chatMessageReceived(Player* sender, const QString& message);
};

#endif // CLIENTHANDLER_H
