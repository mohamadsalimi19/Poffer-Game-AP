#include <QCoreApplication>
#include <QDebug>
#include "hand.h"          // کلاسی که دست را نگه می‌دارد
#include "handevaluator.h" // کلاسی که می‌خواهیم تست کنیم

// یک تابع کمکی برای چاپ کردن نام رتبه دست (برای خوانایی بهتر خروجی)
QString rankToString(HandEvaluator::HandRank rank) {
    switch (rank) {
        case HandEvaluator::INVALID_HAND: return "INVALID_HAND";
        case HandEvaluator::MESSY_HAND: return "MESSY_HAND (High Card)";
        case HandEvaluator::SINGLE_PAIR: return "SINGLE_PAIR";
        case HandEvaluator::DOUBLE_PAIR: return "DOUBLE_PAIR";
        case HandEvaluator::THREE_OF_A_KIND: return "THREE_OF_A_KIND";
        case HandEvaluator::SERIES: return "SERIES (Straight)";
        case HandEvaluator::MSC_HAND: return "MSC_HAND (Flush)";
        case HandEvaluator::PENTHOUSE: return "PENTHOUSE (Full House)";
        case HandEvaluator::FOUR_OF_A_KIND: return "FOUR_OF_A_KIND";
        case HandEvaluator::ORDER_HAND: return "ORDER_HAND (Straight Flush)";
        case HandEvaluator::GOLDEN_HAND: return "GOLDEN_HAND";
        default: return "UNKNOWN_RANK";
    }
}

// تابع اصلی تست
void runTest(const QString& testName, const Hand& hand, HandEvaluator::HandRank expectedRank) {
    HandEvaluator evaluator;
    HandEvaluator::HandRank actualRank = evaluator.evaluateHand(hand);

    qDebug().nospace() << "--- Running Test: " << testName << " ---";
    if (actualRank == expectedRank) {
        qDebug() << "    [PASSED] Expected: " << rankToString(expectedRank) << ", Got: " << rankToString(actualRank);
    } else {
        qDebug() << "    [FAILED] Expected: " << rankToString(expectedRank) << ", Got: " << rankToString(actualRank);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // --- شروع تست‌ها ---

    // تست ۱: یک دست Penthouse (فول هاوس)
    Hand penthouseHand;
    penthouseHand.addCard(Card(Card::DIAMOND, Card::KING));
    penthouseHand.addCard(Card(Card(Card::COIN, Card::KING)));
    penthouseHand.addCard(Card(Card::GOLD, Card::KING));
    penthouseHand.addCard(Card(Card::DOLLAR, Card::FIVE));
    penthouseHand.addCard(Card(Card::DIAMOND, Card::FIVE));
    runTest("Penthouse (Full House)", penthouseHand, HandEvaluator::PENTHOUSE);

    // تست ۲: یک دست Series (سری)
    Hand seriesHand;
    seriesHand.addCard(Card(Card::DIAMOND, Card::SIX));
    seriesHand.addCard(Card(Card::COIN, Card::SEVEN));
    seriesHand.addCard(Card(Card::GOLD, Card::EIGHT));
    seriesHand.addCard(Card(Card::DOLLAR, Card::NINE));
    seriesHand.addCard(Card(Card::DIAMOND, Card::TEN));
    runTest("Series (Straight)", seriesHand, HandEvaluator::SERIES);

    // تست ۳: یک دست Single Pair (جفت)
    Hand pairHand;
    pairHand.addCard(Card(Card::GOLD, Card::QUEEN));
    pairHand.addCard(Card(Card::DOLLAR, Card::QUEEN));
    pairHand.addCard(Card(Card::DIAMOND, Card::TWO));
    pairHand.addCard(Card(Card::COIN, Card::SIX));
    pairHand.addCard(Card(Card::GOLD, Card::NINE));
    runTest("Single Pair", pairHand, HandEvaluator::SINGLE_PAIR);

    // تست ۴: یک دست Messy Hand (پوچ)
    Hand messyHand;
    messyHand.addCard(Card(Card::GOLD, Card::TWO));
    messyHand.addCard(Card(Card::DOLLAR, Card::FIVE));
    messyHand.addCard(Card(Card::DIAMOND, Card::EIGHT));
    messyHand.addCard(Card(Card::COIN, Card::TEN));
    messyHand.addCard(Card(Card::GOLD, Card::KING));
    runTest("Messy Hand (High Card)", messyHand, HandEvaluator::MESSY_HAND);

    // تست ۵: یک دست نامعتبر (فقط ۴ کارت)
    Hand invalidHand;
    invalidHand.addCard(Card(Card::GOLD, Card::TWO));
    invalidHand.addCard(Card(Card::DOLLAR, Card::FIVE));
    invalidHand.addCard(Card(Card::DIAMOND, Card::EIGHT));
    invalidHand.addCard(Card(Card::COIN, Card::TEN));
    runTest("Invalid Hand (4 cards)", invalidHand, HandEvaluator::INVALID_HAND);

    // --- پایان تست‌ها ---

    qDebug() << "\nAll tests finished.";
    return 0; // از برنامه خارج شو
}
