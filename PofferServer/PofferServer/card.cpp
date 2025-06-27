#include "card.h"

Card::Card() : suit(Card::COIN) , rank(Card::TWO) {}


Card:: Card(Suit s, Rank r): suit(s) , rank(r){}

/*
    try{

    }
    catch (const Card_exception& err){
        err.outputOnDebug();
    }
*/

Card::Suit Card:: getSuit() const{
    return suit;
}


Card::Rank Card::getRank() const{
    return rank;
}
