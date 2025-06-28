#ifndef POFFER_H
#define POFFER_H
#include <QWidget>
#include<socketmanager.h>
#include<QJsonObject>
struct Card
{
    QString rank;
    QString suit;
    QPixmap imagePath;
    Card(const QString& s, const QString& r, const QString& img)
        : suit(s), rank(r), imagePath(img) {}
    Card(){};
};




namespace Ui {
class Poffer;
}

class Poffer : public QWidget
{
    Q_OBJECT
    SocketManager* client_socket; // نگه‌داری اشاره‌گر بدون کپی
    QString username;
    QList<Card> deck;
    QJsonObject obj;
   // QString starter;
   // Card player_start;
    //Card opp_start;


public:
    void paintEvent(QPaintEvent *event) override;
    explicit Poffer(SocketManager* socket, QString username, QWidget *parent = nullptr);
    ~Poffer();
    void get_card();
    void choose_turn(QString& starter, Card& player_startcard, Card& opp_startcard);
   // void Json_turn();
    void start_round();
    Card find_card(QString rank , QString suit);
    void show_turn(Card p , Card o);
    QVector<Card> get_list_card_of_turn(QVector<Card> c , QString s, bool a);
    void choose_Card(QVector<Card> c ,  QString starter , bool a);
private slots:

private:
    Ui::Poffer *ui;
};

#endif // POFFER_H
