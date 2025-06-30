#include "poffer.h"
#include "ui_poffer.h"
#include"QMessageBox"
#include"QDebug"
#include"QPainter"
#include"QTimer"
#include"QLabel"
#include<QJsonArray>
#include<QPushButton>
#include<QEventLoop>
#include"QMetaObject"
Poffer::Poffer(SocketManager* socket, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poffer),
    client_socket(socket), // دریافت و نگه‌داری
    username(username),
    round{1}
{
    ui->setupUi(this);    
    this->setFixedSize(1300, 750);
    get_card();
    connect(this, &Poffer::turn_showed, this,[=](){
        QJsonObject payload;
        QJsonObject mainobject;
        mainobject["command"] = "request_card";
        payload["username"] = username;
        mainobject["payload"] = payload;
        QJsonDocument doc(mainobject);
        auto json_to_send = doc.toJson(QJsonDocument::Compact);
      //  client_socket->sendData(json_to_send);
        show_wait();
    });
    connect(client_socket,&SocketManager::dataReceived,this,&Poffer::onServerResponse);
    connect(this, &Poffer::card_recived, this, &Poffer::choose_Card);
    connect(this,&Poffer::card_selected,this,&Poffer::show_wait);
    connect(this,&Poffer::round_result,this,&Poffer::finish_round);

    show_myhand();
    show_point();

}
bool finish_turn = false;

void Poffer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/img/Game-Ground-Photo.JPG");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}
void Poffer::delayWithEventLoop(int second , QLabel* p_lable , QLabel* o_lable){
    QEventLoop loop;
    QTimer::singleShot(second, &loop, &QEventLoop::quit);
    p_lable->hide();
    o_lable->hide();
    loop.exec();
}

void Poffer::start_round(){
    if(round==2){
        QString starter;
        Card player_startcard;
        Card opp_startcard;
        choose_turn(starter,player_startcard,opp_startcard);
        show_turn(player_startcard,opp_startcard);
    }
    else{
        QByteArray t;
       // get_list_card_of_turn();
    }
}



void Poffer::finish_round(QVector<Card> op_card,  QString result,QString my_hand_rank,  QString opponent_hand_rank,    QString my_score,   QString opponent_score) {
    int cardWidth = 130;
    int cardHeight = 180;

    for (int i = 0; i < 5 && i < op_card.size(); i++) {
        op_cards_place[i]->setIcon(QIcon(op_card[i].imagePath));
        op_cards_place[i]->setIconSize(QSize(cardWidth, cardHeight));
    }
    op_point_labale->setText(opponent_score);
    my_point_labale->setText(my_score);
    QLabel* result_show = new QLabel(this);
    result_show->setText("result: " + result);
    result_show->setAlignment(Qt::AlignCenter);
    result_show->setStyleSheet("color: white; background-color: #444; font: 16pt;");
    result_show->setGeometry(400, 300, 500, 50);
    result_show->show();

    QLabel* my_hand_rank_show = new QLabel(this);
    my_hand_rank_show->setText(" my hand: " + my_hand_rank);
    my_hand_rank_show->setAlignment(Qt::AlignCenter);
    my_hand_rank_show->setStyleSheet("color: white; background-color: #555; font: 14pt;");
    my_hand_rank_show->setGeometry(400, 360, 500, 50);
    my_hand_rank_show->show();

    QLabel* opponent_hand_rank_show = new QLabel(this);
    opponent_hand_rank_show->setText("opp hand: " + opponent_hand_rank);
    opponent_hand_rank_show->setAlignment(Qt::AlignCenter);
    opponent_hand_rank_show->setStyleSheet("color: white; background-color: #555; font: 14pt;");
    opponent_hand_rank_show->setGeometry(400, 420, 500, 50);
    opponent_hand_rank_show->show();

    QTimer::singleShot(6000, this, [=]() {
        result_show->hide();
        my_hand_rank_show->hide();
        opponent_hand_rank_show->hide();
        result_show->deleteLater();
        my_hand_rank_show->deleteLater();
        opponent_hand_rank_show->deleteLater();
        emit turn_showed();
    });
}



void Poffer::onServerResponse(QByteArray data){
    if (waiting_label) {
        waiting_label->hide();
        waiting_label->deleteLater();
        waiting_label = nullptr;
    }
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(this, "خطا", "داده از سرور خراب است.");
        return;
    }
    QJsonObject obj = doc.object();


    if (obj["response"].toString()=="select_card_request"){
        QVector<Card> cards;
        QJsonArray card_array = obj["payload"].toObject()["cards"].toArray();
        for (auto val : card_array) {
            auto c = val.toObject();
            QString suit = c["suit"].toString();
            QString rank = c["rank"].toString();
            auto t = find_card(rank,suit);
            qDebug()<<t.rank;
            cards.push_back(find_card(rank, suit));
        }
        emit card_recived(cards, username, true);
    }

    else if(obj["response"].toString()=="round_started"){
        auto  playload_2 = obj["payload"].toObject();
        auto oppunent_card =playload_2["opponent_card"].toObject();
        auto player_card = playload_2["player_card"].toObject() ;
        auto player_startcard = find_card( player_card["rank"].toString(), player_card["suit"].toString());
        auto opp_startcard = find_card( oppunent_card["rank"].toString(), oppunent_card["suit"].toString());
        show_turn(player_startcard,opp_startcard);
    }

    else if(obj["response"].toString()=="round_result"){
        QVector<Card> opponent_hand;
        QJsonArray card_array = obj["payload"].toObject()["opponent_hand"].toArray();
        for (auto val : card_array) {
            auto c = val.toObject();
            QString suit = c["suit"].toString();
            QString rank = c["rank"].toString();
            auto t = find_card(rank,suit);
            qDebug()<<t.rank;
            opponent_hand.push_back(find_card(rank, suit));
        }
        auto payload = obj["payload"].toObject();
        QString result = payload["result"].toString();
        QString my_hand_rank = payload["my_hand_rank"].toString();
        QString opponent_hand_rank = payload["opponent_hand_rank"].toString();
        QString my_score = payload["my_score"].toString();
        QString opponent_score = payload["opponent_score"].toString();
        emit round_result(opponent_hand,result,my_hand_rank,opponent_hand_rank,my_score,opponent_score);
    }




}

void Poffer::show_point(){
    my_point_labale = new QLabel(this);
    op_point_labale = new QLabel(this);

    my_point_labale->setText("my point :0");
    my_point_labale->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    my_point_labale->setStyleSheet("color: white; background-color: rgba(0,0,0,0.5); font: 14pt 'Segoe UI'; padding: 5px; border-radius: 8px;");
    my_point_labale->setGeometry(20, 680, 200, 40);
    my_point_labale->show();

    op_point_labale->setText("enemy point :");
    op_point_labale->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    op_point_labale->setStyleSheet("color: white; background-color: rgba(0,0,0,0.5); font: 14pt 'Segoe UI'; padding: 5px; border-radius: 8px;");
    op_point_labale->setGeometry(20, 20, 200, 40);
    op_point_labale->show();
}


void Poffer::handle_received_cards(QByteArray data) {
    if (waiting_label) {
        waiting_label->hide();
        waiting_label->deleteLater();
        waiting_label = nullptr;
    }

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
    QJsonDocument doc = QJsonDocument::fromJson(test_json);
    if (doc.isNull()) {
        QMessageBox::warning(this, "خطا", "داده از سرور خراب است.");
        return;
    }

    QJsonObject obj = doc.object();

    QVector<Card> cards;
    QJsonArray card_array = obj["payload"].toObject()["cards"].toArray();
    for (auto val : card_array) {
        auto c = val.toObject();
        QString suit = c["suit"].toString();
        QString rank = c["rank"].toString();
        auto t = find_card(rank,suit);
        qDebug()<<t.rank;
        cards.push_back(find_card(rank, suit));
    }

    emit card_recived(cards, username, true);
}
void Poffer::request_card(){




    connect(client_socket , &SocketManager::dataReceived , this , &Poffer::on_turn_showed);
    QJsonObject payload;
    QJsonObject mainobject;
    mainobject["command"] = "request_card";
    payload["username"] = username;
    mainobject["payload"] = payload;
    QVector<Card> c;
    QJsonDocument doc(mainobject);
    auto json_to_send = doc.toJson(QJsonDocument::Compact);
    client_socket->sendData(json_to_send);
}

void Poffer::on_turn_showed(){
    QVector<Card> given_card;
    bool turn = true;
    emit card_recived(given_card , username , turn);
}

void Poffer::show_myhand(){
    int cardWidth = 130;
    int cardHeight = 180;
    int spacing = 20;
    int totalWidth = 5 * cardWidth + 4 * spacing;
    int startX = (1200 - totalWidth) / 2;

    int playerY = 550;
    int opponentY = 20;

    for (int i = 0; i < 5; ++i) {
        QPushButton* btn = new QPushButton(this);
        int x = startX + i * (cardWidth + spacing);
        btn->setFixedSize(cardWidth, cardHeight);
        btn->move(x, playerY);
        btn->show();
        player_cards_place.push_back(btn);
    }

    for (int i = 0; i < 5; ++i) {
        QPushButton* btn = new QPushButton(this);
        int x = startX + i * (cardWidth + spacing);
        btn->setFixedSize(cardWidth, cardHeight);
        btn->move(x, opponentY);
        btn->show();
        op_cards_place.push_back(btn);
    }
}


void Poffer::animation(QPoint final_pos , QPushButton* button){
    QTimer* timer = new QTimer();
    connect(timer,&QTimer::timeout,this,[=](){
        QPoint pos = button->pos();
        pos.setX(pos.x() + 5);
        button->move(pos);
        if(button->pos().x()==final_pos.x()){
            timer->stop();
        }

    });

    timer->start(20);

}


void Poffer::choose_Card(QVector<Card> c, QString starter, bool a) {
    int cardWidth = 130;
    int cardHeight = 180;
    int spacing = 20;
    QVector<QPushButton*> btn;
    int totalWidth = 7 * cardWidth + 6 * spacing;
    int startX = (1200 - totalWidth) / 2;
    int y = (700 - cardHeight) / 2;


    // کارت اول
    QPushButton *button1 = new QPushButton(this);
    QPushButton *button2 = new QPushButton(this);
    QPushButton *button3 = new QPushButton(this);
    QPushButton *button4 = new QPushButton(this);
    QPushButton *button5 = new QPushButton(this);
    QPushButton *button6 = new QPushButton(this);
    QPushButton *button7 = new QPushButton(this);







    QPixmap pix1(c[0].imagePath);
    button1->setIcon(QIcon(pix1));
    button1->setIconSize(QSize(cardWidth, cardHeight));
    button1->setFixedSize(cardWidth, cardHeight);
    QPoint pos_1;
    pos_1.setX(startX + 0 * (cardWidth + spacing));
    pos_1.setY(y);
    button1->move(20, 255);
    button1->show();
    animation(pos_1 , button1);
    connect(button1, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[0].rank + " of " + c[0].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[0].suit;
        payload["rank"] = c[0].rank;
        myhand.push_back(c[0]);
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;
        player_cards_place[round]->setIcon(c[0].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();


    });



    // کارت دوم
    QPixmap pix2(c[1].imagePath);
    button2->setIcon(QIcon(pix2));
    button2->setIconSize(QSize(cardWidth, cardHeight));
    button2->setFixedSize(cardWidth, cardHeight);
    QPoint pos_2;
    pos_2.setX(startX + 1 * (cardWidth + spacing));
    pos_2.setY(y);
    button2->move(20, 255);
    button2->show();
    animation(pos_2 , button2);
    connect(button2, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[1].rank + " of " + c[1].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[1].suit;
        payload["rank"] = c[1].rank;
        myhand.push_back(c[1]);

        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;
        player_cards_place[round]->setIcon(c[1].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit card_selected();
    });

    // کارت سوم
    QPixmap pix3(c[2].imagePath);
    button3->setIcon(QIcon(pix3));
    button3->setIconSize(QSize(cardWidth, cardHeight));
    button3->setFixedSize(cardWidth, cardHeight);
    QPoint pos_3;
    pos_3.setX(startX + 2 * (cardWidth + spacing));
    pos_3.setY(y);
    button3->move(20, 255);
    button3->show();
    animation(pos_3 , button3);
    connect(button3, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[2].rank + " of " + c[2].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[2].suit;
        payload["rank"] = c[2].rank;
        myhand.push_back(c[2]);

        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;
        player_cards_place[round]->setIcon(c[2].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
    });

    // کارت چهارم
    QPixmap pix4(c[3].imagePath);
    button4->setIcon(QIcon(pix4));
    button4->setIconSize(QSize(cardWidth, cardHeight));
    button4->setFixedSize(cardWidth, cardHeight);
    QPoint pos_4;
    pos_4.setX(startX + 3 * (cardWidth + spacing));
    pos_4.setY(y);
    button4->move(20, 255);
    button4->show();
    animation(pos_4 , button4);
    connect(button4, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[3].rank + " of " + c[3].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[3].suit;
        payload["rank"] = c[3].rank;
        myhand.push_back(c[3]);

        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;
        player_cards_place[round]->setIcon(c[3].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
    });

    // کارت پنجم
    QPixmap pix5(c[4].imagePath);
    button5->setIcon(QIcon(pix5));
    button5->setIconSize(QSize(cardWidth, cardHeight));
    button5->setFixedSize(cardWidth, cardHeight);
    QPoint pos_5;
    pos_5.setX(startX + 4 * (cardWidth + spacing));
    pos_5.setY(y);
    button5->move(20, 255);
    button5->show();
    animation(pos_5 , button5);
    connect(button5, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[4].rank + " of " + c[4].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[4].suit;
        payload["rank"] = c[4].rank;
        myhand.push_back(c[4]);
        player_cards_place[round]->setIcon(c[4].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
    });

    // کارت ششم
    QPixmap pix6(c[5].imagePath);
    button6->setIcon(QIcon(pix6));
    button6->setIconSize(QSize(cardWidth, cardHeight));
    button6->setFixedSize(cardWidth, cardHeight);
    QPoint pos_6;
    pos_6.setX(startX + 5 * (cardWidth + spacing));
    pos_6.setY(y);
    button6->move(20, 255);
    button6->show();
    animation(pos_6 , button6);
    connect(button6, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Card Selected", "You selected: " + c[5].rank + " of " + c[5].suit);
        QJsonObject payload;
        payload["username"] = username;
        payload["suit"] = c[5].suit;
        payload["rank"] = c[5].rank;
        myhand.push_back(c[5]);
        player_cards_place[round]->setIcon(c[5].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

        // ارسال به سرور
        client_socket->sendData(json_to_send);
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
    });

    if(c.size()>6){
        QPixmap pix7(c[6].imagePath);
        button7->setIcon(QIcon(pix7));
        button7->setIconSize(QSize(cardWidth, cardHeight));
        button7->setFixedSize(cardWidth, cardHeight);
        QPoint pos_7;
        pos_7.setX(startX + 6 * (cardWidth + spacing));
        pos_7.setY(y);
        button7->move(20, 255);
        button7->show();
        animation(pos_7 , button7);
        connect(button7, &QPushButton::clicked, this, [=]() {
            QMessageBox::information(this, "Card Selected", "You selected: " + c[6].rank + " of " + c[6].suit);
            QJsonObject payload;
            payload["username"] = username;
            payload["suit"] = c[6].suit;
            payload["rank"] = c[6].rank;
            myhand.push_back(c[6]);

            QJsonObject mainobject;
            mainobject["command"] = "select_card";
            mainobject["payload"] = payload;

            QJsonDocument doc(mainobject);
            QByteArray json_to_send = doc.toJson(QJsonDocument::Compact);

            // ارسال به سرور
            client_socket->sendData(json_to_send);
            button1->hide();
            button2->hide();
            button3->hide();
            button4->hide();
            button5->hide();
            button6->hide();
            button7->hide();
        });
    }

}




void Poffer::show_turn(Card p , Card o){
    QPixmap pix_player(p.imagePath);
    QPixmap pix_enemy(o.imagePath);

    QLabel* p_lable = new QLabel(this);
    QLabel* o_lable = new QLabel(this);

    p_lable->setFixedSize(130, 180);
    o_lable->setFixedSize(130, 180);

    p_lable->setPixmap(pix_player.scaled(p_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    o_lable->setPixmap(pix_enemy.scaled(o_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    p_lable->move(100, 250);   // وسط سمت چپ
    o_lable->move(980, 250); // وسط سمت راست

    p_lable->show();
    o_lable->show();

    QTimer* timer = new QTimer(this);
    QTimer* timer_2 = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        QPoint pos = p_lable->pos();
        pos.setY(pos.y() + 8);
        p_lable->move(pos);
        if(pos.y() >= 549 ) {
            timer->stop();
        }
    });
    connect(timer_2, &QTimer::timeout, this, [=]() mutable {
        QPoint pos = o_lable->pos();
        pos.setX(pos.x());
        pos.setY(pos.y() - 8);
        o_lable->move(pos);
        if(pos.y() <= 20 ) {
            timer_2->stop();

        }
    });
    timer->start(30);
    timer_2->start(30);
    QTimer::singleShot(5000, this, [=](){
        p_lable->hide();
        o_lable->hide();
        qDebug()<<"www";
        emit turn_showed();
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



void Poffer::show_wait() {
    QLabel* label = new QLabel(this);
    label->setText("waiting...........");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: white; background-color: #444; font: 16pt;");
    label->setGeometry(400, 350, 500, 50);
    label->show();
    waiting_label = label;
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



