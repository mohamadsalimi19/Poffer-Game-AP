#include "poffer.h"
#include "ui_poffer.h"
#include"QMessageBox"
#include"QDebug"
#include"QPainter"
#include"QTimer"
#include"QLabel"
#include<QJsonArray>
#include<QPushButton>
Poffer::Poffer(SocketManager* socket, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poffer),
    client_socket(socket), // دریافت و نگه‌داری
    username(username)
{
    ui->setupUi(this);    
    this->setFixedSize(1300, 750);
    get_card();



    start_round();


}

void Poffer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/img/Game-Ground-Photo.JPG");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}


void Poffer::start_round(){
    bool turn = false;
    QString starter;
    Card player_startcard;
    Card opp_startcard;
    QVector<Card> given_card;
    Card choosen_card;
    choose_turn(starter,player_startcard,opp_startcard);
  //  show_turn(player_startcard,opp_startcard);
    given_card = get_list_card_of_turn(given_card,starter,turn);
    qDebug()<<given_card[1].suit;
    choose_Card(given_card , starter , turn);


}






void Poffer::choose_Card(QVector<Card> c, QString starter, bool a) {
    int cardWidth = 130;
    int cardHeight = 180;
    int spacing = 20;

    int totalWidth = 7 * cardWidth + 6 * spacing;
    int startX = (1200 - totalWidth) / 2;
    int y = (700 - cardHeight) / 2;

    // کارت اول
    QPushButton *button1 = new QPushButton(this);
    QPixmap pix1(c[0].imagePath);
    button1->setIcon(QIcon(pix1));
    button1->setIconSize(QSize(cardWidth, cardHeight));
    button1->setFixedSize(cardWidth, cardHeight);
    button1->move(startX + 0 * (cardWidth + spacing), y);
    button1->show();
    connect(button1, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[0].rank + " of " + c[0].suit);
    });

    // کارت دوم
    QPushButton *button2 = new QPushButton(this);
    QPixmap pix2(c[1].imagePath);
    button2->setIcon(QIcon(pix2));
    button2->setIconSize(QSize(cardWidth, cardHeight));
    button2->setFixedSize(cardWidth, cardHeight);
    button2->move(startX + 1 * (cardWidth + spacing), y);
    button2->show();
    connect(button2, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[1].rank + " of " + c[1].suit);
    });

    // کارت سوم
    QPushButton *button3 = new QPushButton(this);
    QPixmap pix3(c[2].imagePath);
    button3->setIcon(QIcon(pix3));
    button3->setIconSize(QSize(cardWidth, cardHeight));
    button3->setFixedSize(cardWidth, cardHeight);
    button3->move(startX + 2 * (cardWidth + spacing), y);
    button3->show();
    connect(button3, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[2].rank + " of " + c[2].suit);
    });

    // کارت چهارم
    QPushButton *button4 = new QPushButton(this);
    QPixmap pix4(c[3].imagePath);
    button4->setIcon(QIcon(pix4));
    button4->setIconSize(QSize(cardWidth, cardHeight));
    button4->setFixedSize(cardWidth, cardHeight);
    button4->move(startX + 3 * (cardWidth + spacing), y);
    button4->show();
    connect(button4, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[3].rank + " of " + c[3].suit);
    });

    // کارت پنجم
    QPushButton *button5 = new QPushButton(this);
    QPixmap pix5(c[4].imagePath);
    button5->setIcon(QIcon(pix5));
    button5->setIconSize(QSize(cardWidth, cardHeight));
    button5->setFixedSize(cardWidth, cardHeight);
    button5->move(startX + 4 * (cardWidth + spacing), y);
    button5->show();
    connect(button5, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[4].rank + " of " + c[4].suit);
    });

    // کارت ششم
    QPushButton *button6 = new QPushButton(this);
    QPixmap pix6(c[5].imagePath);
    button6->setIcon(QIcon(pix6));
    button6->setIconSize(QSize(cardWidth, cardHeight));
    button6->setFixedSize(cardWidth, cardHeight);
    button6->move(startX + 5 * (cardWidth + spacing), y);
    button6->show();
    connect(button6, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[5].rank + " of " + c[5].suit);
    });







    QPushButton *button7 = new QPushButton(this);
    QPixmap pix7(c[6].imagePath);
    button7->setIcon(QIcon(pix7));
    button7->setIconSize(QSize(cardWidth, cardHeight));
    button7->setFixedSize(cardWidth, cardHeight);
    button7->move(startX + 6 * (cardWidth + spacing), y);
    button7->show();
    connect(button7, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[6].rank + " of " + c[6].suit);
    });
}




void Poffer::show_turn(Card p , Card o){
    QPixmap pix_player(p.imagePath);
    QPixmap pix_enemy(o.imagePath);

    QLabel* p_lable = new QLabel(this);
    QLabel* o_lable = new QLabel(this);

    p_lable->setFixedSize(150, 200);
    o_lable->setFixedSize(150, 200);

    p_lable->setPixmap(pix_player.scaled(p_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    o_lable->setPixmap(pix_enemy.scaled(o_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    p_lable->move(20, 255);   // وسط سمت چپ
    o_lable->move(20, 255); // وسط سمت راست

    p_lable->show();
    o_lable->show();

    QTimer* timer = new QTimer(this);
    QTimer* timer_2 = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        QPoint pos = p_lable->pos();
        pos.setX(pos.x() + 10);
        pos.setY(pos.y() - 5);
        p_lable->move(pos);
        if(pos.x() >= 535 || pos.y() >= 430) {
            timer->stop();
        }
    });
    connect(timer_2, &QTimer::timeout, this, [=]() mutable {
        QPoint pos = o_lable->pos();
        pos.setX(pos.x() + 10);
        pos.setY(pos.y() + 5);
        o_lable->move(pos);
        if(pos.x() >= 535 ) {
            timer_2->stop();
        }
    });
    timer->start(30);
    timer_2->start(30);
    QTimer::singleShot(3000,this,[=](){
        p_lable->hide();
        o_lable->hide();
    });


}




void Poffer::choose_turn(QString& starter, Card& player_startcard, Card& opp_startcard){
    QJsonObject playload;
    QJsonObject mainobject;
    mainobject["command"] = "start_player";
    mainobject["payload"] = playload;
    QJsonDocument doc(mainobject);
    auto json_to_send = doc.toJson(QJsonDocument::Compact);
    client_socket->sendData(json_to_send);
    //auto res = client_socket->get_response();
    QByteArray test_response = R"(
{
  "response": "start_player",
  "payload": {
    "starter": "player1",
    "player_card": {
      "suit": "DIAMOND",
      "rank": "EIGHT"
    },
    "opponent_card": {
      "suit": "DIAMOND",
      "rank": "SEVEN"
    }
  }
}
)";
    auto res = test_response;

    QJsonObject mainobj;
    QJsonObject playload_2;
    QJsonObject cards;
    QJsonDocument doc_2 = QJsonDocument::fromJson(res);
    mainobject = doc_2.object();
    if(mainobject["response"].toString()!="start_player"){
        QMessageBox::warning(this,"warning","sevrer couldnot response");
        return;
    }
    playload_2 = mainobject["payload"].toObject();
    starter = playload_2["starter"].toString();
    auto oppunent_card =playload_2["opponent_card"].toObject();
    auto player_card = playload_2["player_card"].toObject() ;
    player_startcard = find_card( player_card["rank"].toString(), player_card["suit"].toString());
    opp_startcard = find_card( oppunent_card["rank"].toString(), oppunent_card["suit"].toString());
}






Card Poffer::find_card(QString rank , QString suit){
    for(auto a:deck){
        if(a.rank==rank&&suit==a.suit){
            return a;
        }
    }
    return Card();
}








QVector<Card> Poffer::get_list_card_of_turn(QVector<Card> c2 , QString starter , bool a){
    QJsonObject payload;
    QJsonObject mainobject;
    mainobject["command"] = "request_card";
    mainobject["payload"] = payload;
    QVector<Card> c;
    QJsonDocument doc(mainobject);
    auto json_to_send = doc.toJson(QJsonDocument::Compact);
    client_socket->sendData(json_to_send);

    // گرفتن پاسخ




   // auto res = client_socket->get_response();
    QByteArray test_json = R"(
{
  "response": "cards_given",
  "payload": {
    "starter_username": "mohammad99",
    "cards": [
      { "suit": "DIAMOND", "rank": "KING" },
      { "suit": "GOLD", "rank": "TEN" },
      { "suit": "COIN", "rank": "TEN" },
      { "suit": "DOLLAR", "rank": "BITCOIN" },
      { "suit": "DIAMOND", "rank": "FIVE" },
      { "suit": "COIN", "rank": "SOLDIER" },
      { "suit": "GOLD", "rank": "QUEEN" }
    ]
  }
}
)";
    auto res = test_json;
    QJsonDocument doc_2 = QJsonDocument::fromJson(res);

    QJsonObject mainobj = doc_2.object();

    if (mainobj["response"].toString() != "cards_given") {
        QMessageBox::warning(this, "warning", "Server could not respond");
        return c;
    }

    QJsonObject payload_2 = mainobj["payload"].toObject();
    QJsonArray cards_array = payload_2["cards"].toArray();

    // ساخت کارت‌ها و اضافه به c
    for (const auto &card_val : cards_array) {
        QJsonObject card_obj = card_val.toObject();
        QString suit = card_obj["suit"].toString();
        QString rank = card_obj["rank"].toString();
        c.push_back(find_card(rank, suit));
    }

    if(payload_2["starter_username"]==starter){
        a= true;
    }
    else{
        a = false;
    }

    return c;



}






void Poffer::get_card(){

    // COIN
    // COIN
    deck.append(Card{"COIN", "TWO", ":/img/Coin-2.JPG"});
    deck.append(Card{"COIN", "THREE", ":/img/Coin-3.JPG"});
    deck.append(Card{"COIN", "FOUR", ":/img/Coin-4.JPG"});
    deck.append(Card{"COIN", "FIVE", ":/img/Coin-5.JPG"});
    deck.append(Card{"COIN", "SIX", ":/img/Coin-6.JPG"});
    deck.append(Card{"COIN", "SEVEN", ":/img/Coin-7.JPG"});
    deck.append(Card{"COIN", "EIGHT", ":/img/Coin-8.JPG"});
    deck.append(Card{"COIN", "NINE", ":/img/Coin-9.JPG"});
    deck.append(Card{"COIN", "TEN", ":/img/Coin-10.JPG"});
    deck.append(Card{"COIN", "SOLDIER", ":/img/Coin-Soldier.JPG"});
    deck.append(Card{"COIN", "QUEEN", ":/img/Coin-Queen.JPG"});
    deck.append(Card{"COIN", "KING", ":/img/Coin-King.JPG"});
    deck.append(Card{"COIN", "BITCOIN", ":/img/Coin-Bitcoin.JPG"});

    // GOLD
    deck.append(Card{"GOLD", "TWO", ":/img/Gold-2.JPG"});
    deck.append(Card{"GOLD", "THREE", ":/img/Gold-3.JPG"});
    deck.append(Card{"GOLD", "FOUR", ":/img/Gold-4.JPG"});
    deck.append(Card{"GOLD", "FIVE", ":/img/Gold-5.JPG"});
    deck.append(Card{"GOLD", "SIX", ":/img/Gold-6.JPG"});
    deck.append(Card{"GOLD", "SEVEN", ":/img/Gold-7.JPG"});
    deck.append(Card{"GOLD", "EIGHT", ":/img/Gold-8.JPG"});
    deck.append(Card{"GOLD", "NINE", ":/img/Gold-9.JPG"});
    deck.append(Card{"GOLD", "TEN", ":/img/Gold-10.JPG"});
    deck.append(Card{"GOLD", "SOLDIER", ":/img/Gold-Soldier.JPG"});
    deck.append(Card{"GOLD", "QUEEN", ":/img/Gold-Queen.JPG"});
    deck.append(Card{"GOLD", "KING", ":/img/Gold-King.JPG"});
    deck.append(Card{"GOLD", "BITCOIN", ":/img/Gold-Bitcoin.JPG"});

    // DIAMOND
    deck.append(Card{"DIAMOND", "TWO", ":/img/Dimond-2.JPG"});
    deck.append(Card{"DIAMOND", "THREE", ":/img/Dimond-3.JPG"});
    deck.append(Card{"DIAMOND", "FOUR", ":/img/Dimond-4.JPG"});
    deck.append(Card{"DIAMOND", "FIVE", ":/img/Dimond-5.JPG"});
    deck.append(Card{"DIAMOND", "SIX", ":/img/Dimond-6.JPG"});
    deck.append(Card{"DIAMOND", "SEVEN", ":/img/Dimond-7.JPG"});
    deck.append(Card{"DIAMOND", "EIGHT", ":/img/Dimond-8.JPG"});
    deck.append(Card{"DIAMOND", "NINE", ":/img/Dimond-9.JPG"});
    deck.append(Card{"DIAMOND", "TEN", ":/img/Dimond-10.JPG"});
    deck.append(Card{"DIAMOND", "SOLDIER", ":/img/Dimond-Solider.JPG"});
    deck.append(Card{"DIAMOND", "QUEEN", ":/img/Dimond-Queen.JPG"});
    deck.append(Card{"DIAMOND", "KING", ":/img/Dimond-King.JPG"});
    deck.append(Card{"DIAMOND", "BITCOIN", ":/img/Dimond-Bitcoin.JPG"});

    // DOLLAR
    deck.append(Card{"DOLLAR", "TWO", ":/img/Dollar-2.JPG"});
    deck.append(Card{"DOLLAR", "THREE", ":/img/Dollar-3.JPG"});
    deck.append(Card{"DOLLAR", "FOUR", ":/img/Dollar-4.JPG"});
    deck.append(Card{"DOLLAR", "FIVE", ":/img/Dollar-5.JPG"});
    deck.append(Card{"DOLLAR", "SIX", ":/img/Dollar-6.JPG"});
    deck.append(Card{"DOLLAR", "SEVEN", ":/img/Dollar-7.JPG"});
    deck.append(Card{"DOLLAR", "EIGHT", ":/img/Dollar-8.JPG"});
    deck.append(Card{"DOLLAR", "NINE", ":/img/Dollar-9.JPG"});
    deck.append(Card{"DOLLAR", "TEN", ":/img/Dollar-10.JPG"});
    deck.append(Card{"DOLLAR", "SOLDIER", ":/img/Dollar-Solider.JPG"});
    deck.append(Card{"DOLLAR", "QUEEN", ":/img/Dollar-Queen.JPG"});
    deck.append(Card{"DOLLAR", "KING", ":/img/Dollar-King.JPG"});
    deck.append(Card{"DOLLAR", "BITCOIN", ":/img/Dollar-Bitcoin.JPG"});


}









Poffer::~Poffer()
{
    delete ui;
}



