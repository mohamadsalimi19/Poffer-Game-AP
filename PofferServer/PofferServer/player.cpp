#include "player.h"

Player::Player(const QJsonObject& data)
{
    // initilize with json
    m_username = data["username"].toString();
    m_passwordHash = data["password_hash"].toString();
    m_firstName = data["name"].toString();
    m_lastName = data["lastname"].toString();
    m_email = data["gmail"].toString();
    m_phoneNumber = data["phone_number"].toString();
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
    return playerJson;
}
