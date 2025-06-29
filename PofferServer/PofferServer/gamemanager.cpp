#include "gamemanager.h"
#include "gamesession.h"
#include "player.h"
#include "clienthandler.h"
#include <QDebug>
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
        createNewGame(player1, player2);
    }
}
/////////////////////////////////////////////////////////////////////////

void GameManager::createNewGame(Player* player1, Player* player2)
{
    // make a session for 2 player
    GameSession* newSession = new GameSession(player1, player2);
    m_active_games.append(newSession);

    // notifying player for start game
    player1->getHandler()->startGame(newSession);
    player2->getHandler()->startGame(newSession);

    newSession->startNewRound();
}
/////////////////////////////////////////////////////////////////////////
