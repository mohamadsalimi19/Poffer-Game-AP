#ifndef HANDEVALUATOR_H
#define HANDEVALUATOR_H
#include "hand.h"
#include <iostream>

using namespace std;

class HandEvaluator_exception : public exception {
public:
    HandEvaluator_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};

class HandEvaluator
{
public:
    enum HandRank {
        INVALID_HAND=-1,
        MESSY_HAND=0,
        SINGLE_PAIR,
        DOUBLE_PAIR,
        THREE_OF_A_KIND,
        SERIES,
        MSC_HAND,
        PENTHOUSE,
        FOUR_OF_A_KIND,
        ORDER_HAND,
        GOLDEN_HAND
    };

    HandEvaluator()= default;
    HandRank evaluateHand(const Hand& hand);

private:
    // توابع کمکی که هر کدام یک الگو را بررسی می‌کنند
    bool isGoldenHand(const QVector<Card>& sortedCards);
    bool isOrderHand(const QVector<Card>& sortedCards);
    bool isFourOfAKind(const QVector<Card>& sortedCards);
    bool isPenthouse(const QVector<Card>& sortedCards);
    bool isMscHand(const QVector<Card>& sortedCards);
    bool isSeries(const QVector<Card>& sortedCards);
    bool isThreeOfAKind(const QVector<Card>& sortedCards);
    bool isDoublePair(const QVector<Card>& sortedCards);
    bool isSinglePair(const QVector<Card>& sortedCards);
};

#endif // HANDEVALUATOR_H
