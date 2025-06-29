#ifndef CARD_H
#define CARD_H
#include<QString>
#include"QPixmap"
struct Card
{
    QString rank;
    QString suit;
    QPixmap imagePath;
    Card(const QString& s, const QString& r, const QString& img)
        : suit(s), rank(r), imagePath(img) {}
    Card(){};
};

#endif // CARD_H
