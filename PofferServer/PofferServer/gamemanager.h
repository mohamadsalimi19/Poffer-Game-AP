#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QList>

class Player;
class GameSession;


class GameManager : public QObject
{
    Q_OBJECT

private slots: // <<--- یک بخش جدید برای اسلات‌های خصوصی
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

};

#endif // GAMEMANAGER_H
