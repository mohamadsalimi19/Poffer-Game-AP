#include "player.h"

/////////////////////////////////////////////////////////////////////
Player::Player(const QString& username, const QString& passwordHash)
    : m_username(username),
    m_passwordHash(passwordHash)
{
    // بدنه سازنده خالی است چون تمام کار در Initializer List انجام شد
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
