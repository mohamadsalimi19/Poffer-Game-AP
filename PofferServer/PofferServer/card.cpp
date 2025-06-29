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


QJsonObject Card::toJson() const
{
    QJsonObject cardJson;
    // convert enum to strign for json
    cardJson["suit"] = QVariant::fromValue(suit).toString();
    cardJson["rank"] = QVariant::fromValue(rank).toString();
    return cardJson;
}

Card Card::fromJson(const QJsonObject& json)
{
    // convet json to enum
    QString suitStr = json["suit"].toString();
    QString rankStr = json["rank"].toString();

    // find suit
    // make default suit
    Suit suit = COIN;
    if (suitStr == "GOLD") suit = GOLD;
    else if (suitStr == "DOLLAR") suit = DOLLAR;
    else if (suitStr == "DIAMOND") suit = DIAMOND;

    // find rank
    // rank with default vallue
    Rank rank = TWO;
    if (rankStr == "THREE") rank = THREE;
    else if (rankStr == "FOUR") rank = FOUR;
    else if (rankStr == "FOUR") rank = FIVE;
    else if (rankStr == "FOUR") rank = SIX;
    else if (rankStr == "FOUR") rank = SEVEN;
    else if (rankStr == "FOUR") rank = EIGHT;
    else if (rankStr == "FOUR") rank = NINE;
    else if (rankStr == "FOUR") rank = TEN;
    else if (rankStr == "FOUR") rank = SOLDIER;
    else if (rankStr == "FOUR") rank = QUEEN;
    else if (rankStr == "BITCOIN") rank = BITCOIN;

    return Card(suit, rank);
}
