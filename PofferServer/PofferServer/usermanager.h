#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "player.h"
#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
class UserManager : public QObject
{
    Q_OBJECT
private:

    // read and write from file
    void loadUsers();
    void saveUsers();

    // map for save players
    QMap<QString, Player*> m_users;

public:

    enum LoginResult {
        Login_Success,
        User_NotFound,
        Wrong_Password
    };

    // explicit beacuase dont make with default
    explicit UserManager(QObject *parent = nullptr);

    virtual ~UserManager();

    // static function becuase just need a one object no more than
    static UserManager* instance();

    // login function --> yes or no  ::: updated -- > return more detail
    LoginResult login(const QString& username, const QString& passwordHash, Player*& outPlayer);

    // sign up a new playre
    bool signup(const QJsonObject& userData);


};

#endif // USERMANAGER_H
