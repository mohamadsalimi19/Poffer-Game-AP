#include "clienthandler.h"
#include "usermanager.h"
#include "gamesession.h"
#include "card.h"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

//////////////////////////////////////////////////////////////////////////
ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QObject(parent),
    m_socketDescriptor(socketDescriptor)
{
}

//////////////////////////////////////////////////////////////////////////
void ClientHandler::process()
{
    // started with neew player
    qDebug() << "ClientHandler started in thread:" << QThread::currentThread();

    //
    m_socket = new QTcpSocket();
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        qCritical() << "Could not set socket descriptor:" << m_socket->errorString(); // impotant error
        delete m_socket; // free alocate d memory
        emit finished(); // call main threat our work is end
        return;
    }

    // ۲. سیگنال‌های سوکت را به اسلات‌های خودمان وصل می‌کنیم
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);

    qDebug() << "Client connected with descriptor:" << m_socketDescriptor;
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::onReadyRead()
{
    // read all data in socker puffer (Qbyte)
    QByteArray data = m_socket->readAll();
    qDebug() << "Received from" << m_socketDescriptor << ":" << data;

    // convert to json doc
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Received invalid JSON from" << m_socketDescriptor;
        return;
    }

    // read json and process it
    processMessage(doc.object());
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::onDisconnected()
{
    qDebug() << "Client disconnected:" << m_socketDescriptor;
    m_socket->deleteLater(); // schadul for delete
    emit finished(); // به ترد اصلی خبر می‌دهیم که کار ما تمام شد
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::sendJson(const QJsonObject& json)
{
    // if socket == nullptr && isn't open[
    if (m_socket && m_socket->isOpen()) {
        // convet to byte array for send
        QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
        data.append('\n');
        m_socket->write(data);
        qDebug() << "Sent to" << m_socketDescriptor << ":" << data;
    }
}

//////////////////////////////////////////////////////////////////////////
/// \brief ClientHandler::processMessage
/// \param message
/// void
///
void ClientHandler::processMessage(const QJsonObject& message)
{
    // exctract command


    QString command = message["command"].toString();
    if (command.isEmpty()) {
        qWarning() << "Received message with no command.";
        return;
    }
    // excratct payload
    QJsonObject payload = message["payload"].toObject();

    if (command == "login") {
        handleLogin(payload);
    } else if (command == "signup") {
        handleSignup(payload);
    } else if (command == "request_game") {
        handleRequestGame(payload);
    } else if (command == "select_card") {
        handleSelectCard(payload);
    }
     else if (command == "edit_profile") {
     handleEditProfile(payload);
    }
    else if (command == "request_profile_data") {
        handleRequestProfileData(payload);
    }
    // TODO: دستورات دیگر مثل انتخاب کارت و ... را هم اینجا اضافه کن
}
//////////////////////////////////////////////////////////////////////////
/// \brief ClientHandler::handleLogin
/// \param payload
/// with this function we cas access to login undirectly and its better because help us more

void ClientHandler::handleLogin(const QJsonObject& payload)
{
    QString username = payload["username"].toString();
    QString passwordHash = payload["password_hash"].toString();

    Player* loggedInPlayer = nullptr;
    UserManager::LoginResult result = UserManager::instance()->login(username, passwordHash, loggedInPlayer);

    QJsonObject response;
    QJsonObject responsePayload;

    switch (result) {
    case UserManager::Login_Success:
        this->m_player = loggedInPlayer;
        this->m_player->setHandler(this);
        qDebug() << "Player" << m_player->getUsername() << "authenticated and linked to handler.";

        // checking reconnection
        { // for use domain variable
            GameSession* pausedSession = GameManager::instance()->findPausedGameForPlayer(m_player);
            if (pausedSession) {
                // player has a game
                qDebug() << "Found a paused game for" << m_player->getUsername() << ". Reconnecting...";
                this->m_gameSession = pausedSession;
                pausedSession->reconnectPlayer(m_player);
            } else {
                // formal login
                response["response"] = "auth_success";
                sendJson(response);
            }
        }
        break;

    case UserManager::User_NotFound:
        response["response"] = "auth_error";
        responsePayload["message"] = "User not found.";
        response["payload"] = responsePayload;
        sendJson(response);
        break;

    case UserManager::Wrong_Password:
        response["response"] = "auth_error";
        responsePayload["message"] = "Incorrect password.";
        response["payload"] = responsePayload;
        sendJson(response);
        break;
    }
}
//////////////////////////////////////////////////////////////////////////
/// \brief ClientHandler::handleSignup
/// \param payload
///
void ClientHandler::handleSignup(const QJsonObject& payload)
{
    // checking sign up
    bool success = UserManager::instance()->signup(payload);

    QJsonObject response;
    if (success) {
        response["response"] = "auth_success";
    } else {
        response["response"] = "auth_error";
        response["payload"] = QJsonObject{{"message", "Username already exists."}};
    }
    sendJson(response);
}

//////////////////////////////////////////////////////////////////////////
/// \brief ClientHandler::handleRequestGame
/// \param payload

void ClientHandler::handleRequestGame(const QJsonObject& /*payload*/)
{
    // if a player already play a game or doesn't exist
    if (!m_player || m_gameSession) {
        qWarning() << "Player" << (m_player ? m_player->getUsername() : "Unauthenticated")
        << "sent a game request at an invalid time.";
        // TODO: می‌توانی یک پیام خطا هم برای کلاینت بفرستی
      //  return;
    }

   qDebug() << "Player" << m_player->getUsername() << "is requesting a game.";

   GameManager::instance()->playerWantsToPlay(m_player);
    QJsonObject response;
    response["response"] = "waiting_for_opponent";
    sendJson(response);
}

//////////////////////////////////////////////////////////////////////////
/// \brief ClientHandler::startGame
/// \param session
// Notifying the client to start the game
void ClientHandler::startGame(GameSession* session)
{
    if (!m_player) return; // dont do anything

    m_gameSession = session; // set game session user

    qDebug() << "Notifying player" << m_player->getUsername() << "that their game has started.";

    // make a json pm for client
    QJsonObject payload;
    // TODO: در آینده باید اطلاعات حریف را هم اینجا اضافه کنیم
    // payload["opponent_username"] = ...;

    QJsonObject response;
    response["response"] = "game_started";
    response["payload"] = payload;

    sendJson(response);
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::handleSelectCard(const QJsonObject& payload)
{
    if (!m_gameSession) {
        qWarning() << "Player sent select_card command but is not in a game.";
        return;
    }

    //extract payload
    if (!payload.contains("selected_card")) {
        qWarning() << "select_card command received with no card data.";
        return;
    }
    QJsonObject cardObj = payload["selected_card"].toObject();
    Card selectedCard = Card::fromJson(cardObj);

    qDebug() << "Player" << m_player->getUsername() << "selected card with Rank:" << selectedCard.getRank() << "Suit:" << selectedCard.getSuit();

    m_gameSession->playerSelectedCard(m_player, selectedCard);
}
//////////////////////////////////////////////////////////////////////////
ClientHandler::~ClientHandler()
{
    qDebug() << "ClientHandler for socket" << m_socketDescriptor << "is being destroyed.";
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::handleEditProfile(const QJsonObject& payload)
{
    if (!m_player) return; // اگر کاربر لاگین نکرده، هیچ

    bool success = UserManager::instance()->updatePlayerProfile(m_player->getUsername(), payload);

    QJsonObject response;
    if (success) {
        response["response"] = "profile_update_success";
    } else {
        response["response"] = "profile_update_error";
        qDebug() << "invalid change information";
    }
    sendJson(response);
}
//////////////////////////////////////////////////////////////////////////
void ClientHandler::handleRequestProfileData(const QJsonObject& /*payload*/)
{
    // check player logged in ?
    if (!m_player) {
        qWarning() << "Profile data requested by an unauthenticated user.";

        // send error message
        QJsonObject errorResponse;
        errorResponse["response"] = "error";
        QJsonObject errorPayload;
        errorPayload["message"] = "User not authenticated. Please login again.";
        errorResponse["payload"] = errorPayload;
        sendJson(errorResponse);
        return;
    }

    qDebug() << "Profile data requested by" << m_player->getUsername() << ". Sending data...";

    // make json
    QJsonObject response;
    response["response"] = "profile_data_response";

    response["payload"] = m_player->toJson();

    sendJson(response);
}
