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
        qWarning() << "Could not open users.json for reading. A new file will be created on first signup.";
        return;
    }

    QByteArray savedData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(savedData);

    if (!doc.isArray()) {
        qWarning() << "users.json data is not a valid array. Cannot load users.";
        return;
    }

    // extract array data
    QJsonArray usersArray = doc.array();

    // make every player
    for (const QJsonValue& userValue : usersArray) {
        QJsonObject userObject = userValue.toObject();
        if (userObject.isEmpty()) {
            continue; // jump invalid object
        }

        Player* player = new Player(userObject);

        m_users.insert(player->getUsername(), player);
    }

    qDebug() << m_users.count() << "users loaded from file successfully.";
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
        return false;
    }

    Player* newPlayer = new Player(userData);
    m_users.insert(username, newPlayer);

    saveUsers();
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
///////////////////////////////////////////////////////////////////
bool UserManager::updatePlayerProfile(const QString& currentUsername, const QJsonObject& newProfileData)
{
    // find player
    if (!m_users.contains(currentUsername)) {
        qWarning() << "Attempted to update a non-existent user:" << currentUsername;
        return false;
    }
    Player* playerToUpdate = m_users.value(currentUsername);

    // change username
    QString newUsername = newProfileData["username"].toString();
    if (!newUsername.isEmpty() && newUsername != currentUsername) {
        // check avalbility
        if (m_users.contains(newUsername)) {
            qWarning() << "Update failed: New username" << newUsername << "is already taken.";
            return false; // repeated
        }

        // deleted old key
        m_users.remove(currentUsername);
        playerToUpdate->setUsername(newUsername);
        m_users.insert(newUsername, playerToUpdate);
        qDebug() << "Username changed from" << currentUsername << "to" << newUsername;
    }

    // update other information
    if (newProfileData.contains("name")) {
        playerToUpdate->setFirstName(newProfileData["name"].toString());
    }
    if (newProfileData.contains("lastname")) {
        playerToUpdate->setLastName(newProfileData["lastname"].toString());
    }
    if (newProfileData.contains("password_hash")) {
        playerToUpdate->setPasswordHash(newProfileData["password_hash"].toString());
    }
    if (newProfileData.contains("gmail")) {
        playerToUpdate->setEmail(newProfileData["gmail"].toString());
    }
    if (newProfileData.contains("phone_number")) {
        playerToUpdate->setPhoneNumber(newProfileData["phone_number"].toString());
    }

    //save all changes
    saveUsers();
    qDebug() << "Profile for user" << playerToUpdate->getUsername() << "updated successfully.";
    return true;
}
///////////////////////////////////////////////////////////////////
Player* UserManager::getPlayer(const QString& username)
{
    return m_users.value(username, nullptr);
}
