#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "deck.h"
#include "hand.h"
#include "handevaluator.h"
#include "player.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>

using namespace std ;

class GameSession_exception: public exception {
public:
    GameSession_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};

class GameSession : public QObject
{
    Q_OBJECT // for use socket

private:
    void startDraftingPhase(); // start sharing cards
    void sendDraftPoolToCurrentPlayer(); // give 7 card to current palyer


    Player* breakTie(const Hand& hand1, const Hand& hand2, HandEvaluator::HandRank rank); // if 2 hand is equal chek it in herre

    Player* m_player1; // first player
    Player* m_player2; // sec
    Player* m_currentPlayerForDraft; // cur player

    Hand m_player1_hand;  // hand of player 1
    Hand m_player2_hand; // hand of player 2

    int m_player1_score;
    int m_player2_score;

    Deck m_deck;   // deck that update every roudn
    HandEvaluator m_evaluator;

    QVector<Card> m_draftPool; // 7 cards that give to players


public:
    GameSession(Player* player1, Player* player2, QObject *parent = nullptr);

    void startNewRound();

    // void evaluateAndFinishRound(); // caculate winner
    Player* evaluateAndFinishRound();

    // this function clalled by client hanlder while player select a card
    void playerSelectedCard(Player* player, const Card& selectedCard);

    // this functions help us to test logic server
    void setPlayer1Hand(const Hand& hand) {
        m_player1_hand = hand;
    }

    void setPlayer2Hand(const Hand& hand) {
        m_player2_hand = hand;
    }

};

#endif // GAMESESSION_H
