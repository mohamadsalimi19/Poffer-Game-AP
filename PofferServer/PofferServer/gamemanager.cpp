#include "gamemanager.h"
#include "gamesession.h"
#include "player.h"
#include "clienthandler.h"
#include <QTimer>
#include <QDebug>
#include <QMetaObject>
/////////////////////////////////////////////////////////////////////////
GameManager* GameManager::instance()
{
    static GameManager manager;
    return &manager;
}
/////////////////////////////////////////////////////////////////////////
GameManager::GameManager(QObject *parent) : QObject(parent) {}
/////////////////////////////////////////////////////////////////////////
void GameManager::playerWantsToPlay(Player* player)
{
    if (m_waiting_lobby.contains(player)) {
        qDebug() << "Player" << player->getUsername() << "is already in the lobby.";
        return;
    }

    qDebug() << "Player" << player->getUsername() << "added to the lobby.";
    m_waiting_lobby.append(player);

    // if count player = 2 then start gamr
    if (m_waiting_lobby.size() >= 2) {
        // take 2 first player م
        Player* player1 = m_waiting_lobby.takeFirst();
        Player* player2 = m_waiting_lobby.takeFirst();

        qDebug() << "Matching" << player1->getUsername() << "and" << player2->getUsername();
        QMetaObject::invokeMethod(this, "createNewGame", Qt::QueuedConnection,Q_ARG(Player*, player1), Q_ARG(Player*, player2));
    }
}
/////////////////////////////////////////////////////////////////////////
void GameManager::createNewGame(Player* player1, Player* player2)
{
    // create new session between 2 players
    GameSession* newSession = new GameSession(player1, player2);
    m_active_games.append(newSession);
    qDebug() << "Game session created for" << player1->getUsername() << "and" << player2->getUsername();

    // connects gamesession signal to gamemaneger slots
    connect(newSession, &GameSession::gameFinished, this, &GameManager::onGameFinished);
    connect(newSession, &GameSession::sendMessageToPlayer, this, &GameManager::onSendMessageToPlayer);

    // call safety 2 player game is start
    // QueuedConnection garantin functions run in its threats
    QMetaObject::invokeMethod(player1->getHandler(), "startGame", Qt::QueuedConnection, Q_ARG(GameSession*, newSession));
    QMetaObject::invokeMethod(player2->getHandler(), "startGame", Qt::QueuedConnection, Q_ARG(GameSession*, newSession));

    // start round 1
    newSession->startNewRound();
}
/////////////////////////////////////////////////////////////////////////
void GameManager::gameSessionPaused(GameSession* session)
{
    // add game to paused games
    if (m_active_games.removeOne(session)) {
        m_paused_games.append(session);
    }
}
/////////////////////////////////////////////////////////////////////////
GameSession* GameManager::findPausedGameForPlayer(Player* player)
{
    // add game to active games
    for (GameSession* session : m_paused_games) {
        if (session->isPlayerInSession(player)) {
            m_paused_games.removeOne(session);
            m_active_games.append(session);
            return session;
        }
    }
    return nullptr;
}
/////////////////////////////////////////////////////////////////////////
bool GameSession::isPlayerInSession(Player* player)
{
    return (player == m_player1 || player == m_player2);
}
/////////////////////////////////////////////////////////////////////////
void GameManager::onGameFinished(GameSession* session)
{
    qDebug() << "A game session has finished. Cleaning up...";
    m_active_games.removeOne(session); //delete from active list game
    session->deleteLater(); // delete obj safety
}
/////////////////////////////////////////////////////////////////////////

void GameManager::onChatMessageReceived(Player* sender, const QString& message)
{
    qDebug() << "Chat message from" << sender->getUsername() << ":" << message;

    // find session
    for (GameSession* session : m_active_games) {
        if (session->isPlayerInSession(sender)) {
            // find emeny
            Player* opponent = session->getOpponent(sender);
            if (opponent) {
                // make json
                QJsonObject payload;
                payload["sender"] = sender->getUsername();
                payload["message"] = message;

                QJsonObject response;
                response["response"] = "new_chat_message";
                response["payload"] = payload;

                // send message to opponent
                emit session->sendMessageToPlayer(opponent, response);
            }
            return;
        }
    }
}
