#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QJsonObject>

class Player {
private:
    QString m_username;
    QString m_passwordHash;
    // می‌توانید اطلاعات دیگر مثل امتیاز و... را هم اینجا اضافه کنید

public:
    Player(const QString& username, const QString& passwordHash);

    QString getUsername() const;
    QString getPasswordHash() const;
    QJsonObject toJson() const;

};

#endif // PLAYER_H
