#include "gamesession.h"
#include "clienthandler.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QMap>
#include <QList>
#include <algorithm>
#include <QTimer>
#include "usermanager.h"

// check this file again after complete another classes ::::: remembre mohamad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////////////////////////////////////////////////////////////////////////////
GameSession::GameSession(Player* player1, Player* player2, QObject *parent)
    : QObject(parent) , m_player1(player1),
    m_player2(player2) , m_currentPlayerForDraft(player1), // check here again for first player
    m_player1_score(0) , m_player2_score(0)
{
    qDebug() << "GameSession created bitween" << player1->getUsername() << "and" << player2->getUsername();

    // create timer
    m_disconnectTimer = new QTimer(this);
    m_disconnectTimer->setSingleShot(true); // timer use just one time
    connect(m_disconnectTimer, &QTimer::timeout, this, &GameSession::onDisconnectTimerTimeout);

    // --- بخش جدید: تنظیم تایمر عدم فعالیت ---
    // ساخت و اتصال تایمر هشدار (۲۰ ثانیه)
    m_warningTimer = new QTimer(this);
    m_warningTimer->setSingleShot(true);
    connect(m_warningTimer, &QTimer::timeout, this, &GameSession::onWarningTimerTimeout);

    // ساخت و اتصال تایمر هشدار (3۰ ثانیه)
    m_inactivityTimer = new QTimer(this);
    m_inactivityTimer->setSingleShot(true);
    connect(m_inactivityTimer, &QTimer::timeout, this, &GameSession::onInactivityTimeout);

    m_inactivityStrikes[m_player1] = 0;
    m_inactivityStrikes[m_player2] = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::startNewRound()
{
    m_round_number++; // increase round number
    qDebug() << "<--------- Starting Round" << m_round_number << "--------->";

    m_player1_hand.clear();
    m_player2_hand.clear();

    if (m_round_number == 1) {
        // for round 1
        qDebug() << "Round 1: Determining starter with Diamond cards...";
        Deck starterDeck;
        starterDeck.shuffle();
        Card card1 = starterDeck.dealDiamondCard();
        Card card2 = starterDeck.dealDiamondCard();

        qDebug() << "Determination cards ->" << m_player1->getUsername() << ": Diamond Rank" << card1.getRank()
                 << "---" << m_player2->getUsername() << ": Diamond Rank" << card2.getRank();

        if (card1.getRank() > card2.getRank()) {
            m_currentPlayerForDraft = m_player1;
        } else {
            m_currentPlayerForDraft = m_player2;
        }

    } else {
        // another rounds
        // update current player
        m_currentPlayerForDraft = (m_currentPlayerForDraft == m_player1) ? m_player2 : m_player1;
        qDebug() << "Round" << m_round_number << ": Turn alternates to the other player.";
    }

    qDebug() << m_currentPlayerForDraft->getUsername() << "will start the drafting phase.";

    qDebug() << "Preparing a new full deck for the round...";
    m_deck = Deck();
    m_deck.shuffle();

    startDraftingPhase();
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::startDraftingPhase()
{
    qDebug() << "Starting a new mini-draft...";
    m_draftPool.clear();
    m_isFirstPickInMiniRound = true; // every mini round start with 7 card

    for (int i = 0; i < 7; ++i) {
        if(m_deck.cardCount() > 0) //checking for not empty
            m_draftPool.append(m_deck.deal());
    }

    sendDraftPoolToCurrentPlayer();
}
//////////////////////////////////////////////////////////////////////////////////////////
/// send draft pool to players and then choose player suitable card
/// so clinet send a json to this part
void GameSession::sendDraftPoolToCurrentPlayer()
{
    qDebug() << "Sending" << m_draftPool.size() << "cards to" << m_currentPlayerForDraft->getUsername();
    QJsonArray cardsArray;
    for (const Card& card : m_draftPool) {
        cardsArray.append(card.toJson());
    }
    QJsonObject payload;
    payload["cards"] = cardsArray;
    QJsonObject response;
    response["response"] = "select_card_request";
    response["payload"] = payload;

    // send signal
    emit sendMessageToPlayer(m_currentPlayerForDraft, response);

    //m_inactivityTimer->start(30000);
    QMetaObject::invokeMethod(m_warningTimer, "start", Qt::QueuedConnection, Q_ARG(int, 20000));
    QMetaObject::invokeMethod(m_inactivityTimer, "start", Qt::QueuedConnection, Q_ARG(int, 30000));

    qDebug() << "Started 30s inactivity timer for" << m_currentPlayerForDraft->getUsername();
}
//////////////////////////////////////////////////////////////////////////////////////////
/// \brief GameSession::playerSelectedCard
/// \param player
/// \param selectedCard
///while a player select a card this function has been run
void GameSession::playerSelectedCard(Player* player, const Card& selectedCard)
{
    if (player != m_currentPlayerForDraft) {
        qWarning() << "Received card selection from" << player->getUsername() << "but it's not their turn.";
        return;
    }

    bool card_removed = m_draftPool.removeOne(selectedCard);
    if (!card_removed) {
        qWarning() << "Player" << player->getUsername() << "selected a card that is not in the draft pool.";
        return;
    }

    //m_inactivityTimer->stop();
    QMetaObject::invokeMethod(m_inactivityTimer, "stop", Qt::QueuedConnection);
    qDebug() << "Inactivity timer stopped for" << player->getUsername();

    // add card to player hand
    if (player == m_player1) m_player1_hand.addCard(selectedCard);
    else m_player2_hand.addCard(selectedCard);

    qDebug() << player->getUsername() << "selected a card. Their hand size is now:" << (player == m_player1 ? m_player1_hand.getCards().size() : m_player2_hand.getCards().size());

    if (m_isFirstPickInMiniRound) {
        // if first select?
        m_isFirstPickInMiniRound = false; // now turn player 2
        m_currentPlayerForDraft = (m_currentPlayerForDraft == m_player1) ? m_player2 : m_player1;
        sendDraftPoolToCurrentPlayer();
    } else {
        // is second select ? end mini round
        qDebug() << "Mini-draft finished.";

        // checking full hand?
        if (m_player1_hand.getCards().size() >= 5) {
            qDebug() << "All drafting rounds are complete. Evaluating hands...";
            evaluateAndFinishRound();
        } else {
            //m_currentPlayerForDraft = (m_currentPlayerForDraft == m_player1) ? m_player2 : m_player1; incorrect code
            // next mini round
            startDraftingPhase();
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
/*oid GameSession::evaluateAndFinishRound()
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
}*/
///////////////////////////**********///////////////////////////////
/// \brief rankToString
/// \param rank
/// \return
/// helping function to evaluateAndFinishRound
QString rankToString(HandEvaluator::HandRank rank)
{
    switch (rank) {
    case HandEvaluator::INVALID_HAND:      return "INVALID_HAND";
    case HandEvaluator::MESSY_HAND:        return "MESSY_HAND";
    case HandEvaluator::SINGLE_PAIR:      return "SINGLE_PAIR";
    case HandEvaluator::DOUBLE_PAIR:      return "DOUBLE_PAIR";
    case HandEvaluator::THREE_OF_A_KIND: return "THREE_OF_A_KIND";
    case HandEvaluator::SERIES:            return "SERIES";
    case HandEvaluator::MSC_HAND:          return "MSC_HAND";
    case HandEvaluator::PENTHOUSE:       return "PENTHOUSE";
    case HandEvaluator::FOUR_OF_A_KIND:  return "FOUR_OF_A_KIND";
    case HandEvaluator::ORDER_HAND:        return "ORDER_HAND";
    case HandEvaluator::GOLDEN_HAND:       return "GOLDEN_HAND";
    default:                               return "UNKNOWN";
    }
}
///////////////////////////**********///////////////////////////////
/// \brief GameSession::evaluateAndFinishRound
/// \return
///
Player* GameSession::evaluateAndFinishRound(){
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

    QJsonArray hand1_json_array, hand2_json_array;
    for(const auto& card : m_player1_hand.getCards()) { hand1_json_array.append(card.toJson()); }
    for(const auto& card : m_player2_hand.getCards()) { hand2_json_array.append(card.toJson()); }

    // make json player 1
    QJsonObject payload1;
    payload1["result"] = (winner == m_player1) ? "won" : (winner == m_player2) ? "lost" : "draw";
    payload1["my_hand_rank"] = rankToString(rank1);// helping function
    payload1["opponent_hand_rank"] = rankToString(rank2);
    payload1["opponent_hand"] = hand2_json_array;
    payload1["my_score"] = m_player1_score;
    payload1["opponent_score"] = m_player2_score;
    QJsonObject response1;
    response1["response"] = "round_result";
    response1["payload"] = payload1;
    emit sendMessageToPlayer(m_player1, response1);

    // make json playre 2
    QJsonObject payload2;
    payload2["result"] = (winner == m_player2) ? "won" : (winner == m_player1) ? "lost" : "draw";
    payload2["my_hand_rank"] = rankToString(rank2);
    payload2["opponent_hand_rank"] = rankToString(rank1);
    payload2["opponent_hand"] = hand1_json_array;
    payload2["my_score"] = m_player2_score;
    payload2["opponent_score"] = m_player1_score;
    QJsonObject response2;
    response2["response"] = "round_result";
    response2["payload"] = payload2;
    emit sendMessageToPlayer(m_player2, response2);



    // checking end game
    if (m_player1_score >= 2) {
        endGame(m_player1, "Won 2 rounds.");
    } else if (m_player2_score >= 2) {
        endGame(m_player2, "Won 2 rounds.");
    } else {
        // wait 5 sec
        QTimer::singleShot(5000, this, &GameSession::startNewRound);
    }


    return winner;
}


//////////////////////////////////////////////////////////////////////////////////////////
// helped function
// this funnction help us to determine pairs with return a map that carry Rank card and count it in a hand
QMap<Card::Rank, int> getRankCounts(const Hand& hand) {
    // make map
    QMap<Card::Rank, int> counts;
    // traversal map
    for (const Card& card : hand.getCards()) {
        // find or make key and add 1 to value
        counts[card.getRank()]++;
    }
    return counts;
}

// find rank with its count in map ( value : n )
Card::Rank findNOfAKindRank(int n, const QMap<Card::Rank, int>& counts) {
    for(auto it = counts.constBegin(); it != counts.constEnd(); ++it) {
        if (it.value() == n) {
            return it.key();
        }
    }
    return Card::TWO; // never reach here
}

// this function compare list kicker and find player winner
Player* compareKickers(Player* p1, Player* p2, QList<Card::Rank> kickers1, QList<Card::Rank> kickers2)
{
    // sorting
    std::sort(kickers1.begin(), kickers1.end(), std::greater<Card::Rank>());
    std::sort(kickers2.begin(), kickers2.end(), std::greater<Card::Rank>());

    for (int i = 0; i < kickers1.size(); ++i) {
        if (kickers1[i] > kickers2[i]) return p1;
        if (kickers2[i] > kickers1[i]) return p2;
    }

    return nullptr; // this part never happend
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

    // in this part make 2 map to find pairs in map
    auto counts1 = getRankCounts(hand1);
    auto counts2 = getRankCounts(hand2);

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
            auto s1_highest = hand1.getCards().last().getRank();
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

            // find rank
            Card::Rank rank1_primary = findNOfAKindRank(n, counts1);
            Card::Rank rank2_primary = findNOfAKindRank(n, counts2);

            if (rank1_primary > rank2_primary) return m_player1;
            if (rank2_primary > rank1_primary) return m_player2;
            break;
        }

        // base on pair cards if doesn;t work base on other cards
        case HandEvaluator::DOUBLE_PAIR: {
            QList<Card::Rank> pairs1, pairs2;
            Card::Rank kicker1 = Card::TWO, kicker2 = Card::TWO;

            // find ranks for pair and kicker
            for(auto it = counts1.constBegin(); it != counts1.constEnd(); ++it) {
                if(it.value() == 2) pairs1.append(it.key());
                else if(it.value() == 1) kicker1 = it.key();
            }
            for(auto it = counts2.constBegin(); it != counts2.constEnd(); ++it) {
                if(it.value() == 2) pairs2.append(it.key());
                else if(it.value() == 1) kicker2 = it.key();
            }

            std::sort(pairs1.begin(), pairs1.end(), std::greater<Card::Rank>());
            std::sort(pairs2.begin(), pairs2.end(), std::greater<Card::Rank>());

            // checking biger than
            // big pair
            if (pairs1[0] > pairs2[0]) return m_player1;
            if (pairs2[0] > pairs1[0]) return m_player2;
            // sec piar
            if (pairs1[1] > pairs2[1]) return m_player1;
            if (pairs2[1] > pairs1[1]) return m_player2;
            // kicker
            if (kicker1 > kicker2) return m_player1;
            if (kicker2 > kicker1) return m_player2;

            break;
        }

        case HandEvaluator::SINGLE_PAIR: {
            Card::Rank pairRank1 = Card::TWO, pairRank2 = Card::TWO;
            QList<Card::Rank> kickers1, kickers2;

            for(auto it = counts1.constBegin(); it != counts1.constEnd(); ++it) {
                if(it.value() == 2) pairRank1 = it.key(); else kickers1.append(it.key());
            }
            for(auto it = counts2.constBegin(); it != counts2.constEnd(); ++it) {
                if(it.value() == 2) pairRank2 = it.key(); else kickers2.append(it.key());
            }

            // compare pairs
            if (pairRank1 > pairRank2) return m_player1;
            if (pairRank2 > pairRank1) return m_player2;
            // compare kicker with helped function
            return compareKickers(m_player1, m_player2, kickers1, kickers2);
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
             if (sortedCards2[0].getSuit() > sortedCards1[0].getSuit()) return m_player2;
             break;
        }
        default :
            return nullptr;
    }

    return nullptr; // this part never happend
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::playerDisconnected(Player* disconnectedPlayer)
{
    // if a player disconneced befor
    if (m_disconnectedPlayer) return;

    qDebug() << "GameSession notified that" << disconnectedPlayer->getUsername() << "has disconnected. Starting 60s timer...";
    m_disconnectedPlayer = disconnectedPlayer;
    m_disconnectTimer->start(60000);
    GameManager::instance()->gameSessionPaused(this);

    //sending notify to other player
    Player* remainingPlayer = (disconnectedPlayer == m_player1) ? m_player2 : m_player1;
    if (remainingPlayer && remainingPlayer->getHandler()) {
        QJsonObject payload;
        payload["wait_time"] = 60;

        QJsonObject response;
        response["response"] = "opponent_disconnected";
        response["payload"] = payload;

        remainingPlayer->getHandler()->sendJson(response);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::onDisconnectTimerTimeout()
{
    qDebug() << "Disconnect timer for" << m_disconnectedPlayer->getUsername() << "timed out.";

    // disconnected player exist ..
    if (m_disconnectedPlayer) {
        // othre player is winner
        Player* winner = (m_disconnectedPlayer == m_player1) ? m_player2 : m_player1;
        endGame(winner, "Opponent failed to reconnect in time.");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::reconnectPlayer(Player* player)
{
    if (player == m_disconnectedPlayer) {
        qDebug() << "Player" << player->getUsername() << "reconnected successfully!";
        m_disconnectedPlayer = nullptr;
        m_disconnectTimer->stop();

        QJsonObject response;
        response["response"] = "game_resumed";

        if (m_player1->getHandler()) m_player1->getHandler()->sendJson(response);
        if (m_player2->getHandler()) m_player2->getHandler()->sendJson(response);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::endGame(Player* winner, const QString& reason)
{
    qDebug() << "Game is ending. Winner:" << (winner ? winner->getUsername() : "None (Draw)") << "Reason:" << reason;

    // find loser
    Player* loser = nullptr;
    if (winner) {
        loser = (winner == m_player1) ? m_player2 : m_player1;
    }

    // add to history
    if (winner && loser) {
        QJsonObject winnerResult;
        winnerResult["result"] = "won";
        winnerResult["opponent"] = loser->getUsername();
        UserManager::instance()->addGameResult(winner->getUsername(), winnerResult);

        QJsonObject loserResult;
        loserResult["result"] = "lost";
        loserResult["opponent"] = winner->getUsername();
        UserManager::instance()->addGameResult(loser->getUsername(), loserResult);
    }

    // make json for winner
    if (winner) {
        QJsonObject payload_win;
        payload_win["result"] = "You Won!";
        payload_win["reason"] = reason;

        QJsonObject response_win;
        response_win["response"] = "game_over";
        response_win["payload"] = payload_win;

        emit sendMessageToPlayer(winner, response_win);
    }

    // make json for loser
    if (loser) {
        QJsonObject payload_lose;
        payload_lose["result"] = "You Lost!";
        payload_lose["reason"] = reason;

        QJsonObject response_lose;
        response_lose["response"] = "game_over";
        response_lose["payload"] = payload_lose;

        emit sendMessageToPlayer(loser, response_lose);
    }


    // notify end game
    emit gameFinished(this);
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameManager::onSendMessageToPlayer(Player* player, const QJsonObject& message)
{
    if (player && player->getHandler()) {
        // use invokeMethod for safety to use sendjson
        QMetaObject::invokeMethod(player->getHandler(), "sendJson", Qt::QueuedConnection,Q_ARG(QJsonObject, message));
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
Player* GameSession::getOpponent(Player* player) const
{
    if (player == m_player1) {
        return m_player2;
    } else if (player == m_player2) {
        return m_player1;
    }
    return nullptr; // never reach here
}
//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::onInactivityTimeout()
{
    Player* inactivePlayer = m_currentPlayerForDraft;
    qDebug() << "Player" << inactivePlayer->getUsername() << "was inactive!";

    // تعداد اخطارهای او را یکی اضافه می‌کنیم
    m_inactivityStrikes[inactivePlayer]++;
    int currentStrikes = m_inactivityStrikes[inactivePlayer];
    qDebug() << "This is strike" << currentStrikes << "for" << inactivePlayer->getUsername();

    // بر اساس تعداد اخطارها تصمیم می‌گیریم
    if (currentStrikes >= 2) {
        qDebug() << "Player has 2 strikes. They lose the game.";
        Player* winner = getOpponent(inactivePlayer);
        endGame(winner, "Opponent was inactive for a second time.");
    } else {
        qDebug() << "Selecting a random card for them.";
        if (!m_draftPool.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(m_draftPool.size());
            Card randomCard = m_draftPool[randomIndex];
            playerSelectedCard(inactivePlayer, randomCard);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
// اسلات جدید برای ارسال هشدار بعد از ۲۰ ثانیه
void GameSession::onWarningTimerTimeout()
{
    if (!m_currentPlayerForDraft) return;
    qDebug() << "20s passed. Sending 10s warning to" << m_currentPlayerForDraft->getUsername();

    QJsonObject payload;
    payload["time_remaining"] = 10;
    QJsonObject response;
    response["response"] = "inactivity_warning";
    response["payload"] = payload;
    emit sendMessageToPlayer(m_currentPlayerForDraft, response);
}

