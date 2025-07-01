#ifndef DECK_H
#define DECK_H
#include <iostream>
#include "card.h"
#include <QVector> // use vec for our deck cards
#include <QRandomGenerator> // for use genarate random value to send suffle
#include <algorithm> // for use suffle function
using namespace std;

class Deck_exception : public exception {
public:
    Deck_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};

class Deck
{
private:
    QVector<Card> m_cards;

public:
    Deck(); // make 52 card

    void shuffle(); // shuffling cards
    Card deal();    // give one card to next player

    // share diamond card for detemine fisrt player
    Card dealDiamondCard();

    int cardCount() const ;


};

#endif // DECK_H
