#ifndef CARD_H
#define CARD_H
#include <QDebug>
#include <QJsonObject>
#include <QVariant> // a library carry very data type and help us to convert enum to json file
using namespace std;

class Card_exception : public exception {
public:
    Card_exception(const char * st) : exception(st) {}
    void outputOnDebug() const {
        qDebug() << exception::what() ;
    }
};

class Card
{
public:

    // type of cards
    enum Suit {
        COIN,
        GOLD,
        DOLLAR,
        DIAMOND
    };

    // levels
    enum Rank {
        TWO = 2,
        THREE = 3,
        FOUR = 4,
        FIVE = 5,
        SIX = 6,
        SEVEN = 7,
        EIGHT = 8,
        NINE = 9,
        TEN = 10,
        SOLDIER = 11,
        QUEEN = 12,
        KING = 13,
        BITCOIN = 14
    };

    // default constructor
    Card();

    Card(Suit s, Rank r);

    virtual ~Card() = default;

    Suit getSuit() const;
    Rank getRank() const;

    bool operator == (const Card & other) const { // for use remove one function
        return (suit == other.suit && rank == other.rank);
    }

    // 2 function to convert json
    QJsonObject toJson() const;
    static Card fromJson(const QJsonObject& json);

private:
    Suit suit;
    Rank rank;

};

#endif // CARD_H
