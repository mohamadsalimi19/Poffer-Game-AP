#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QList>

class Card;
class Player;
class GameSession;
class ClientHandler;
class QJsonObject;

class GameManager : public QObject
{
    Q_OBJECT

private slots:
    void createNewGame(Player* player1, Player* player2);

private:
    //void createNewGame(Player* player1, Player* player2);

    QList<Player*> m_waiting_lobby;
    QList<GameSession*> m_active_games;
    QList<GameSession*> m_paused_games;

public:
    explicit GameManager(QObject *parent = nullptr);

    // static because just need a one game manager
    static GameManager* instance();

    void playerWantsToPlay(Player* player);

    void gameSessionPaused(GameSession* session);
    GameSession* findPausedGameForPlayer(Player* player);

public slots:
    void onGameFinished(GameSession* session);
    void onSendMessageToPlayer(Player* player, const QJsonObject& message);
    void onLoginRequested(ClientHandler* handler, const QJsonObject& payload);
    void onSignupRequested(ClientHandler* handler, const QJsonObject& payload);
    void onGameRequested(ClientHandler* handler);
    void onCardSelected(ClientHandler* handler, const Card& card);

private slots:
    void onPlayerWantsToPlay(Player* player);

};

#endif // GAMEMANAGER_H
