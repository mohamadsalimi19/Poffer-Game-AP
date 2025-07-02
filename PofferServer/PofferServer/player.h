#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QJsonObject>

class ClientHandler;

class Player {
private:
    QString m_username;
    QString m_passwordHash;
    QString m_firstName;
    QString m_lastName;
    QString m_email;
    QString m_phoneNumber;
    // every player has a handler that help it to communicate with core game
    ClientHandler* m_handler = nullptr;
    // hsitory
    QList<QJsonObject> m_gameHistory;

public:
    //constructor give a json and make player
    explicit Player(const QJsonObject& data);

    // Getters
    QString getUsername() const;
    QString getPasswordHash() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getEmail() const;
    QString getPhoneNumber() const;
    ClientHandler* getHandler() const;

    // Setters
    void setUsername(const QString& username);
    void setPasswordHash(const QString& hash);
    void setFirstName(const QString& name);
    void setLastName(const QString& name);
    void setEmail(const QString& email);
    void setPhoneNumber(const QString& phone);
    void setHandler(ClientHandler* handler);
    // convet to json
    QJsonObject toJson() const;

    void addGameResult(const QJsonObject& result);
    const QList<QJsonObject>& getGameHistory() const;
    void setGameHistory(const QList<QJsonObject>& history);

};

#endif // PLAYER_H
