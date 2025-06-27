#ifndef HAND_H
#define HAND_H
#include <iostream>
#include "card.h"
#include <QVector>

using namespace std;

class Hand_exception : public exception {
public:
    Hand_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};

class Hand
{
private:
    QVector<Card> m_cards;

public:
    Hand()= default ;

    void addCard(const Card& card);
    void clear();
    const QVector<Card>& getCards() const;


};

#endif // HAND_H


// difrent bitween deck and hand : hand is like books and deck like library
