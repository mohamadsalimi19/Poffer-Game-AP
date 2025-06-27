#include "hand.h"

void Hand::addCard(const Card& card){
    try{
        if(m_cards.size()>5){
            throw Hand_exception("The number of cards in hand is full!");
        }
        m_cards.push_back(card);
    }
    catch(const Hand_exception& err){
        err.outputOnDebug();
    }
}


void Hand::clear(){
    try{
        m_cards.clear();
    }
    catch(const Hand_exception& err){
        err.outputOnDebug();
    }
}

const QVector<Card>& Hand::getCards() const{
    return m_cards;
}
