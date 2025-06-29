#include "player.h"

/////////////////////////////////////////////////////////////////////
Player::Player(const QString& username, const QString& passwordHash)
    : m_username(username),
    m_passwordHash(passwordHash)
{

}

/////////////////////////////////////////////////////////////////////

QString Player::getUsername() const
{
    return m_username;
}

QString Player::getPasswordHash() const
{
    return m_passwordHash;
}
/////////////////////////////////////////////////////////////////////


// make json from player data
QJsonObject Player::toJson() const
{
    QJsonObject playerJson;
    playerJson["username"] = m_username;
    playerJson["password_hash"] = m_passwordHash;
    // if add anothre information fill it here

    return playerJson;
}

/////////////////////////////////////////////////////////////////////
void Player::setHandler(ClientHandler* handler) {
    m_handler = handler;
}
/////////////////////////////////////////////////////////////////////
ClientHandler* Player::getHandler() const {
    return m_handler;
}
/////////////////////////////////////////////////////////////////////
