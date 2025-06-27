#include <QCoreApplication>
#include <QDebug> // برای چاپ کردن خروجی در کنسول
#include "deck.h" // کلاسی که می‌خواهیم تست کنیم

// این main موقتی فقط برای تست کردن است
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "--- Starting Deck and Card Test ---";

    // 1. یک دسته کارت بساز
    Deck myDeck;
    qDebug() << "Deck created with 52 cards successfully.";

    // 2. دسته کارت را بُر بزن
    myDeck.shuffle();
    qDebug() << "Deck has been shuffled.";

    // 3. پنج کارت از دسته بکش و نمایش بده
    qDebug() << "\nDealing 5 cards:";
    for (int i = 0; i < 5; ++i) {
        Card dealtCard = myDeck.deal();
        // فعلا فقط عددهای مربوط به نوع و ارزش کارت را چاپ می‌کنیم
        qDebug() << "Card" << i + 1 << "-> Suit:" << dealtCard.getSuit() << "Rank:" << dealtCard.getRank();
    }

    qDebug() << "\n--- Test Finished ---";

    // از برنامه خارج می‌شویم تا سرور اصلی اجرا نشود
    return 0;
}
