#include "deck.h"

Deck::Deck() {
    try{

        for (int s = Card::COIN; s <= Card::DIAMOND; ++s){  // 52 time
            for(int r = Card::TWO ; r <= Card::BITCOIN ; r++){
                m_cards.push_back( Card(static_cast<Card::Suit> (s) , static_cast<Card::Rank> (r)) ) ;// static cast : int -> enum
            }
        }
    }
    catch (const Deck_exception& err){
        err.outputOnDebug();
    }

}
 /*
    try{

    }
    catch (const Deck_exception& err){
        err.outputOnDebug();
    }
  * */


void Deck::shuffle(){
    try{
        auto rng = QRandomGenerator::global(); // range is a pointer to a random value. global fun make a random and return pointer to it
        std::shuffle(m_cards.begin(), m_cards.end(), *rng); // suffle in algorithm library
    }
    catch (const Deck_exception& err){
        err.outputOnDebug();
    }
}


Card Deck::deal(){
    try{
        // a card in end of vector choose then remove from deck
        if (!m_cards.isEmpty()) {
            return m_cards.takeLast();
        }
        throw Deck_exception("there isn't any card on vector m_cards !");
    }
    catch (const Deck_exception& err){
        err.outputOnDebug();
        return Card(); // give a card for prevent crash program
    }
}



Card Deck::dealDiamondCard()
{
    // find diamond cards
    for (int i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i].getSuit() == Card::DIAMOND) {

            return m_cards.takeAt(i);
        }
    }
   // if don't find ---> never happend this situation
    return deal();
}
