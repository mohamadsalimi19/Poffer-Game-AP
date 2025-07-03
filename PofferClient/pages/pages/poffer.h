#ifndef POFFER_H
#define POFFER_
#pragma once
#include <QWidget>
#include<socketmanager.h>
#include<QJsonObject>
#include<QPushButton>
#include<QLabel>
#include"card.h"

#include <QTextEdit>
#include <QLineEdit>
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
    QLabel* waiting_label ;
    QLabel* my_point_labale;
    QLabel* op_point_labale;
    QPushButton* pauseButton;
    QPushButton* continueButton;
    int round;
    int num_pause;
    QString opScore;
    QString playerScore;
    int time_warning;
    QTimer* hiddenTimer;
    QTimer* visibleTimer;
    QWidget* overlayPAUSE;
    QTimer* pauseTimer = nullptr;
    QLabel* countdownLabel = nullptr;
    int remain_time = 20;
    QWidget* overlay = nullptr;
    QTextEdit* chatTextEdit = nullptr;
    QLineEdit* chatLineEdit = nullptr;

   // QString starter;
   // Card player_start;
    //Card opp_start;


public:
    void show_point();
    void paintEvent(QPaintEvent *event) override;
    explicit Poffer(SocketManager* socket, QString username, QWidget *parent = nullptr);
    ~Poffer();
    void get_card();
    void choose_turn(QString& starter, Card& player_startcard, Card& opp_startcard);
   // void Json_turn();
    //void start_round();
    Card find_card(QString rank , QString suit);
    void show_turn(Card p , Card o);
    void show_wait( );
    void choose_Card(QVector<Card> c ,  QString starter , bool a);
    void animation(QPoint final_pos  , QPushButton* button );
    void delayWithEventLoop(int second ,QLabel* p_lable , QLabel* o_lable);
    void on_turn_showed();
    void show_myhand();
    void request_card();
    void handle_received_cards(QByteArray data);
    void onServerResponse(QByteArray data);
    void finish_round(const QVector<Card>& op_card, const QString& result, const QString& my_hand_rank, const QString& opponent_hand_rank, const QString& my_score, const QString& opponent_score);
    void finish_game(QString result);
    void pause_button();
    void pause_requset();
    void resetBoardForNewRound();
    void opponent_disconnected_show();
    void game_resumedSLOT();
    void game_pausedSLOT();
    void setupChatUI();
    void send_chat_message(const QString& msg);


private slots:


signals:
    Card card_choosen();
    void turn_showed();
    void card_recived(QVector<Card> , QString a , bool b);
    void card_selected();
    void round_result(QVector<Card> , QString result ,QString my_hand_rank , QString opponent_hand_rank , QString my_score , QString opponent_score);
    void game_over(QString result);
    void op_disconnected();
    void game_resumed();
    void game_paused();
    void round_started(Card player1_card,Card player2_card);

private:
    Ui::Poffer *ui;
};

#endif // POFFER_H
