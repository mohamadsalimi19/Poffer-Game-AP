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

////////////////////////////////////////////////////////
Card::Rank Card::getRank() const{
    return rank;
}

////////////////////////////////////////////////////////
QString suitToString(Card::Suit s) {
    switch (s) {
    case Card::COIN: return "COIN";
    case Card::GOLD: return "GOLD";
    case Card::DOLLAR: return "DOLLAR";
    case Card::DIAMOND: return "DIAMOND";
    }
    return ""; //
}
////////////////////////////////////////////////////////
QString rankToString(Card::Rank r) {
    switch (r) {
    case Card::TWO: return "TWO";
    case Card::THREE: return "THREE";
    case Card::FOUR: return "FOUR";
    case Card::FIVE: return "FIVE";
    case Card::SIX: return "SIX";
    case Card::SEVEN: return "SEVEN";
    case Card::EIGHT: return "EIGHT";
    case Card::NINE: return "NINE";
    case Card::TEN: return "TEN";
    case Card::SOLDIER: return "SOLDIER";
    case Card::QUEEN: return "QUEEN";
    case Card::KING: return "KING";
    case Card::BITCOIN: return "BITCOIN";
    }
    return ""; // never this happend
}
////////////////////////////////////////////////////////
QJsonObject Card::toJson() const
{
    QJsonObject cardJson;

    cardJson["suit"] = suitToString(suit);
    cardJson["rank"] = rankToString(rank);
    return cardJson;
}
////////////////////////////////////////////////////////
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
    else if (rankStr == "FIVE") rank = FIVE;
    else if (rankStr == "SIX") rank = SIX;
    else if (rankStr == "SEVEN") rank = SEVEN;
    else if (rankStr == "EIGHT") rank = EIGHT;
    else if (rankStr == "NINE") rank = NINE;
    else if (rankStr == "TEN") rank = TEN;
    else if (rankStr == "SOLDIER") rank = SOLDIER;
    else if (rankStr == "QUEEN") rank = QUEEN;
    else if (rankStr == "KING") rank = KING;
    else if (rankStr == "BITCOIN") rank = BITCOIN;

    return Card(suit, rank);
}
