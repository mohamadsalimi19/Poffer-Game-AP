#ifndef CARD_H
#define CARD_H
#include<QString>
#include"QPixmap"
#include <QJsonObject>
struct Card
{
    QString rank;
    QString suit;
    QPixmap imagePath;
    Card(const QString& s, const QString& r, const QString& img)
        : suit(s), rank(r), imagePath(img) {}
    Card(){};

public:
    QJsonObject toJson() const
    {

        QJsonObject cardJson;


        cardJson["suit"] = suit;
        cardJson["rank"] = rank;


        return cardJson;
    }
};

#endif // CARD_H
