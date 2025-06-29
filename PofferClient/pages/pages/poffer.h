#ifndef POFFER_H
#define POFFER_
#pragma once
#include <QWidget>
#include<socketmanager.h>
#include<QJsonObject>
#include<QPushButton>
#include<QLabel>
#include"card.h"


namespace Ui {
class Poffer;
}

class Poffer : public QWidget
{
    Q_OBJECT
    SocketManager* client_socket; // نگه‌داری اشاره‌گر بدون کپی
    QString username;
    QString starter;
    QList<Card> deck;
    QJsonObject obj;
    QVector<Card> myhand;
    QVector<QPushButton*>player_cards_place;
    QVector<QPushButton*>op_cards_place;
    int round;
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
    QVector<Card> get_list_card_of_turn( QString s, bool a);
    void choose_Card(QVector<Card> c ,  QString starter , bool a);
    void animation(QPoint final_pos  , QPushButton* button );
    void delayWithEventLoop(int second ,QLabel* p_lable , QLabel* o_lable);
    void on_turn_showed();
    void show_myhand();
private slots:


signals:
    Card card_choosen();
    void turn_showed();
    void card_recived(QVector<Card> , QString a , bool b);


private:
    Ui::Poffer *ui;
};

#endif // POFFER_H
