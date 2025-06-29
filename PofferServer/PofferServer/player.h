#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QJsonObject>

class ClientHandler;

class Player {
private:
    QString m_username;
    QString m_passwordHash;
    // every player has a handler that help it to communicate with core game
    ClientHandler* m_handler = nullptr;


public:
    Player(const QString& username, const QString& passwordHash);

    QString getUsername() const;
    QString getPasswordHash() const;
    QJsonObject toJson() const;

    void setHandler(ClientHandler* handler);
    ClientHandler* getHandler() const;


};

#endif // PLAYER_H
