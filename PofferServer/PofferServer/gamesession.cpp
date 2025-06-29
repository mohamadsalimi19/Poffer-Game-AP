#include "gamesession.h"
#include "clienthandler.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QMap>
#include <QList>
#include <algorithm>

// check this file again after complete another classes ::::: remembre mohamad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////////////////////////////////////////////////////////////
GameSession::GameSession(Player* player1, Player* player2, QObject *parent)
    : QObject(parent) , m_player1(player1),
    m_player2(player2) , m_currentPlayerForDraft(player1), // check here again for first player
    m_player1_score(0) , m_player2_score(0)
{
    qDebug() << "GameSession created bitween" << player1->getUsername() << "and" << player2->getUsername();
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::startNewRound()
{
    qDebug() << "<--------- Starting New Round --------->";

    m_player1_hand.clear();
    m_player2_hand.clear();

    qDebug() << "Determining who starts the round...";

    // temp deck for detemine first player
    Deck starterDeck;
    starterDeck.shuffle();

    // 2 card
    Card card1 = starterDeck.dealDiamondCard();
    Card card2 = starterDeck.dealDiamondCard();

    qDebug() << "Determination cards ->" << m_player1->getUsername() << ": Diamond Rank" << card1.getRank()
             << "---" << m_player2->getUsername() << ": Diamond Rank" << card2.getRank();

    // determine
    if (card1.getRank() > card2.getRank()) {
        m_currentPlayerForDraft = m_player1;
    } else {
        // if equal player 2 started -- > never  happend
        m_currentPlayerForDraft = m_player2;
    }

    qDebug() << m_currentPlayerForDraft->getUsername() << "will start the drafting phase.";


    qDebug() << "Preparing a new full deck for the round...";
    m_deck = Deck();
    m_deck.shuffle();

    startDraftingPhase();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::startDraftingPhase(){
    try{
            qDebug() << "Drafting phase has started...";
            m_draftPool.clear();

            for (int i = 0; i < 7; ++i) {
                m_draftPool.append(m_deck.deal());
            }

            sendDraftPoolToCurrentPlayer();
    }
    catch(const GameSession_exception& err){
        err.outputOnDebug();
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
/// send draft pool to players and then choose player suitable card
/// so clinet send a json to this part
void GameSession::sendDraftPoolToCurrentPlayer()
{
    qDebug() << "Sending" << m_draftPool.size() << "cards to" << m_currentPlayerForDraft->getUsername();

    // for save all card make a array json
    QJsonArray cardsArray;
    for (const Card& card : m_draftPool) {
        cardsArray.append(card.toJson());
    }

    // final message
    QJsonObject payload;
    payload["cards"] = cardsArray;

    QJsonObject response;
    response["response"] = "select_card_request";
    response["payload"] = payload;

    // send message
    m_currentPlayerForDraft->getHandler()->sendJson(response);
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief GameSession::playerSelectedCard
/// \param player
/// \param selectedCard
///while a player select a card this function has been run
void GameSession::playerSelectedCard(Player* player, const Card& selectedCard)
{
    // checkin turn player
    if (player != m_currentPlayerForDraft) {
        qDebug() << "Warning:" << player->getUsername() << "played out of turn!";
        return;
    }

    qDebug() << player->getUsername() << "selected a card.";

    // add card to player hand
    if (player == m_player1) {
        m_player1_hand.addCard(selectedCard);
    } else {
        m_player2_hand.addCard(selectedCard);
    }

    // remove selected card from draft : Card class needs == operator
    m_draftPool.removeOne(selectedCard); //

    // checking capacity hand
    if (m_player1_hand.getCards().size() == 5 && m_player2_hand.getCards().size() == 5) {
        qDebug() << "Drafting is complete. Evaluating hands...";
        evaluateAndFinishRound();
    } else {
        m_currentPlayerForDraft = (m_currentPlayerForDraft == m_player1) ? m_player2 : m_player1;
        sendDraftPoolToCurrentPlayer();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::evaluateAndFinishRound()
{
      Player* winner = nullptr;
    // calculate winner
    // evaluateHand is a member fucntion in handEvaluator class
    HandEvaluator::HandRank rank1 = m_evaluator.evaluateHand(m_player1_hand);
    HandEvaluator::HandRank rank2 = m_evaluator.evaluateHand(m_player2_hand);

    // who is winner ?
    // if ranks is equal then check exception
    if (rank1 > rank2) {
        winner = m_player1;
    } else if (rank2 > rank1) {
        winner = m_player2;
    } else {
        // if 2 hand is equal most be break and determine a winner
        winner = breakTie(m_player1_hand, m_player2_hand, rank1);
    }

    if (winner == m_player1) {
        m_player1_score++;
        qDebug() << m_player1->getUsername() << "wins the round!";
    } else if (winner == m_player2) {
        m_player2_score++;
        qDebug() << m_player2->getUsername() << "wins the round!";
    } else {
        qDebug() << "The round is a draw!";
    }

    // TODO: یک پیام "round_result" به هر دو کلاینت بفرست
    // این پیام باید شامل نتیجه، امتیازات جدید و دست حریف باشد

    // چک کردن پایان بازی
    if (m_player1_score >= 2 || m_player2_score >= 2) {
        qDebug() << "Game Over!";
        // TODO: پیام "game_over" را به هر دو کلاینت بفرست
    } else {
        // شروع راند بعدی
        startNewRound();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
// helped function
QMap<Card::Rank, int> getRankCounts(const Hand& hand) {
    QMap<Card::Rank, int> counts;
    for (const Card& card : hand.getCards()) {
        counts[card.getRank()]++;
    }
    return counts;
}

Card::Rank findNOfAKindRank(int n, const QMap<Card::Rank, int>& counts) {
    for(auto it = counts.constBegin(); it != counts.constEnd(); ++it) {
        if (it.value() == n) {
            return it.key();
        }
    }
    return Card::TWO; // هرگز نباید به اینجا برسد
}
//////////////////////////////////////////////////////////////////////////////////////////
/// \brief GameSession::breakTie
/// \param hand1
/// \param hand2
/// \param rank
/// \return Player winner
/// this function break situation to suitable type then fingure out winner
Player* GameSession::breakTie(const Hand& hand1, const Hand& hand2, HandEvaluator::HandRank rank)
{
    qDebug() << "Breaking a tie for rank:" << rank;

    switch (rank) {
        case HandEvaluator::GOLDEN_HAND: {
            // just suit cards
            if (hand1.getCards()[0].getSuit() > hand2.getCards()[0].getSuit()) return m_player1;
            if (hand2.getCards()[0].getSuit() > hand1.getCards()[0].getSuit()) return m_player2;
            break;
        }
        case HandEvaluator::ORDER_HAND:
        case HandEvaluator::SERIES: {
            // highest rank
            auto s1_highest = hand1.getCards().last().getRank(); // دست‌ها در HandEvaluator مرتب شده‌اند
            auto s2_highest = hand2.getCards().last().getRank();
            if (s1_highest > s2_highest) return m_player1;
            if (s2_highest > s1_highest) return m_player2;
            break;
        }
        case HandEvaluator::FOUR_OF_A_KIND:
        case HandEvaluator::PENTHOUSE:
        case HandEvaluator::THREE_OF_A_KIND: {
            // base on set of same rank card
            auto counts1 = getRankCounts(hand1);
            auto counts2 = getRankCounts(hand2);
            int n = (rank == HandEvaluator::FOUR_OF_A_KIND) ? 4 : 3;

            Card::Rank rank1_primary = findNOfAKindRank(n, counts1);
            Card::Rank rank2_primary = findNOfAKindRank(n, counts2);

            if (rank1_primary > rank2_primary) return m_player1;
            if (rank2_primary > rank1_primary) return m_player2;
            break;
        }

        // base on pair cards if doesn;t work base on other cards
        case HandEvaluator::DOUBLE_PAIR:
        case HandEvaluator::SINGLE_PAIR: {


            break;
        }

        case HandEvaluator::MSC_HAND:
        case HandEvaluator::MESSY_HAND: {
            // compare cards
             auto sortedCards1 = hand1.getCards();
             auto sortedCards2 = hand2.getCards();
             // sorting cards
             std::sort(sortedCards1.begin(), sortedCards1.end(), [](const Card& a, const Card& b) { return a.getRank() > b.getRank(); });
             std::sort(sortedCards2.begin(), sortedCards2.end(), [](const Card& a, const Card& b) { return a.getRank() > b.getRank(); });


             for (int i = 0; i < 5; ++i) {
                 if (sortedCards1[i].getRank() > sortedCards2[i].getRank()) return m_player1;
                 if (sortedCards2[i].getRank() > sortedCards1[i].getRank()) return m_player2;
             }
             // compare suit
             if (sortedCards1[0].getSuit() > sortedCards2[0].getSuit()) return m_player1;
             else return m_player2;
             break;
        }
        default :
            return nullptr;
    }

    return nullptr; // اگر بعد از همه مقایسه‌ها باز هم مساوی بود، راند مساوی است
}
