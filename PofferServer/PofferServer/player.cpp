#include "player.h"
#include <QJsonArray>
Player::Player(const QJsonObject& data)
{
    // initilize with json
    m_username = data["username"].toString();
    m_passwordHash = data["password_hash"].toString();
    m_firstName = data["name"].toString();
    m_lastName = data["lastname"].toString();
    m_email = data["gmail"].toString();
    m_phoneNumber = data["phone_number"].toString();
    // read history player
    if (data.contains("game_history") && data["game_history"].isArray()) {
        QJsonArray historyArray = data["game_history"].toArray();
        for (const QJsonValue& value : historyArray) {
            m_gameHistory.append(value.toObject());
        }
    }
}

// Getters
QString Player::getUsername() const { return m_username; }
QString Player::getPasswordHash() const { return m_passwordHash; }
QString Player::getFirstName() const { return m_firstName; }
QString Player::getLastName() const { return m_lastName; }
QString Player::getEmail() const { return m_email; }
QString Player::getPhoneNumber() const { return m_phoneNumber; }
ClientHandler* Player::getHandler() const { return m_handler; }

// Setters
void Player::setPasswordHash(const QString& hash) { m_passwordHash = hash; }
void Player::setFirstName(const QString& name) { m_firstName = name; }
void Player::setLastName(const QString& name) { m_lastName = name; }
void Player::setEmail(const QString& email) { m_email = email; }
void Player::setPhoneNumber(const QString& phone) { m_phoneNumber = phone; }
void Player::setHandler(ClientHandler* handler) { m_handler = handler; }
void Player::setUsername(const QString& username) {
    m_username = username;
}


QJsonObject Player::toJson() const
{
    QJsonObject playerJson;
    playerJson["username"] = m_username;
    playerJson["password_hash"] = m_passwordHash;
    playerJson["name"] = m_firstName;
    playerJson["lastname"] = m_lastName;
    playerJson["gmail"] = m_email;
    playerJson["phone_number"] = m_phoneNumber;

    // convet history to array
    QJsonArray historyArray;
    for (const QJsonObject& result : m_gameHistory) {
        historyArray.append(result);
    }
    playerJson["game_history"] = historyArray;

    return playerJson;
}


void Player::addGameResult(const QJsonObject& result)
{
    // add result to begin list
    m_gameHistory.prepend(result);

    // delete after 3
    if (m_gameHistory.size() > 3) {
        m_gameHistory.removeLast();
    }
}

const QList<QJsonObject>& Player::getGameHistory() const
{
    return m_gameHistory;
}

void Player::setGameHistory(const QList<QJsonObject>& history)
{
    m_gameHistory = history;
}
