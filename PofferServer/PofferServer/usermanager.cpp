#include "usermanager.h"

///////////////////////////////////////////////////////////////////
// پیاده‌سازی الگوی Singleton
UserManager* UserManager::instance()
{
    static UserManager manager;
    return &manager;
}
///////////////////////////////////////////////////////////////////
UserManager::UserManager(QObject *parent) : QObject(parent)
{
    loadUsers(); // در زمان ساخته شدن، کاربران قبلی را از فایل بارگذاری می‌کنیم
}
///////////////////////////////////////////////////////////////////
void UserManager::loadUsers()
{
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open users.json file for reading. A new one will be created.";
        return;
    }

    QByteArray data = file.readAll(); // read all file then close it ---> data is all data
    file.close(); // close file

    // make a json doc : qbyte -- formjson --> qjsondoc
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qDebug() << "users.json is not a valid JSON array.";
        return;
    }

    QJsonArray usersArray = doc.array();
    for (const QJsonValue& value : usersArray) {
        QJsonObject userObj = value.toObject();
        QString username = userObj["username"].toString();
        QString passHash = userObj["password_hash"].toString();

        Player* player = new Player(username, passHash);
        m_users.insert(username, player);
    }
    qDebug() << m_users.count() << "users loaded successfully.";
}
///////////////////////////////////////////////////////////////////
void UserManager::saveUsers()
{
    QJsonArray usersArray;
    for (Player* player : m_users.values()) {
        usersArray.append(player->toJson());
    }

    QJsonDocument doc(usersArray);

    QFile file("users.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Could not open users.json file for writing.";
        return;
    }
    file.write(doc.toJson());
    file.close();
}
///////////////////////////////////////////////////////////////////
/// \brief UserManager::login
/// \param username
/// \param passwordHash
/// \param outPlayer
/// \return
/// send out player as call by refrence
UserManager::LoginResult UserManager::login(const QString& username, const QString& passwordHash, Player*& outPlayer)
{
    outPlayer = nullptr; // reset outplayer

    if (!m_users.contains(username)) {
        return User_NotFound; // user doesnt exist
    }

    Player* player = m_users.value(username);
    if (player->getPasswordHash() == passwordHash) {
        outPlayer = player;
        return Login_Success;
    } else {
        return Wrong_Password;
    }
}
///////////////////////////////////////////////////////////////////
bool UserManager::signup(const QJsonObject& userData)
{
    QString username = userData["username"].toString();
    if (m_users.contains(username)) {
        return false; // این نام کاربری قبلاً گرفته شده
    }

    QString passwordHash = userData["password_hash"].toString();
    Player* newPlayer = new Player(username, passwordHash);

    m_users.insert(username, newPlayer);

    saveUsers(); // کاربر جدید را در فایل ذخیره می‌کنیم

    qDebug() << "New user signed up:" << username;
    return true;
}
///////////////////////////////////////////////////////////////////
UserManager::~UserManager()
{
    qDeleteAll(m_users);
    m_users.clear();
    qDebug() << "UserManager destroyed and all players cleaned up.";
}
