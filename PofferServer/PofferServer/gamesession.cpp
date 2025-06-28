#include "gamesession.h"
#include <QDebug>

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
void GameSession::startNewRound(){
    try{
         qDebug() << "<--------- start Round --------->";

        // clear hands
        m_player1_hand.clear();
        m_player2_hand.clear();

        //new deck
        m_deck = Deck();
        m_deck.shuffle();
        qDebug() << "shuffle card successfuly";

        // start sharing cardsd
        startDraftingPhase();

    }
    catch(const GameSession_exception& err){
        err.outputOnDebug();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::startDraftingPhase(){
    try{
        qDebug() << "Drafting phase has started...";
        // reset draft pool
        m_draftPool.clear();

        // select 7 cards
        for (int i = 0; i < 7; ++i) {
            m_draftPool.push_back(m_deck.deal());
        }

        // check this part again
        m_currentPlayerForDraft = m_player1;
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
    // این تابع باید یک پیام JSON بسازد و آن را برای کلاینت بازیکنی
    // که نوبتش است، ارسال کند.

    qDebug() << "Sending" << m_draftPool.size() << "cards to" << m_currentPlayerForDraft->getUsername();

    // TODO: در اینجا باید به ClientHandler مربوط به m_currentPlayerForDraft بگوییم
    // که یک پیام "select_card_request" با محتوای m_draftPool برایش بفرستد.
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameSession::playerSelectedCard(Player* player, const Card& selectedCard)
{
    // این تابع زمانی اجرا می‌شود که بازیکنی کارتی را انتخاب کرده باشد

    // چک می‌کنیم آیا نوبت همین بازیکن بوده یا نه
    if (player != m_currentPlayerForDraft) {
        qDebug() << "Warning:" << player->getUsername() << "played out of turn!";
        return;
    }

    qDebug() << player->getUsername() << "selected a card.";

    // ۱. کارت انتخاب شده را به دست بازیکن اضافه می‌کنیم
    if (player == m_player1) {
        m_player1_hand.addCard(selectedCard);
    } else {
        m_player2_hand.addCard(selectedCard);
    }

    // remove selected card from draft : Card class needs == operator
    m_draftPool.removeOne(selectedCard); //

    // ۳. چک می‌کنیم آیا دست‌ها پر شده‌اند
    if (m_player1_hand.getCards().size() == 5 && m_player2_hand.getCards().size() == 5) {
        qDebug() << "Drafting is complete. Evaluating hands...";
        evaluateAndFinishRound();
    } else {
        // اگر دست‌ها پر نشده، نوبت را به بازیکن دیگر می‌دهیم
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
        // دست‌ها برابر است، باید تساوی را بشکنیم
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
/// \brief GameSession::breakTie
/// \param hand1
/// \param hand2
/// \param rank
/// \return Player winner
/// this function break situation to suitable type then fingure out winner
Player* GameSession::breakTie(const Hand& hand1, const Hand& hand2, HandEvaluator::HandRank rank)
{
    qDebug() << "Breaking a tie for rank:" << rank;
    // TODO: این بخش مهم‌ترین منطق مقایسه جزئیات دست‌هاست
    // برای هر رتبه باید منطق مقایسه پیاده شود
    // مثلا برای SINGLE_PAIR، اول ارزش جفت‌ها و بعد کیکرها مقایسه می‌شود

    // به عنوان یک راه حل موقت، در صورت تساوی، برنده را null برمی‌گردانیم
    return nullptr;
}
