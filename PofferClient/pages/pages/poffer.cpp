#include "poffer.h"
#include "ui_poffer.h"
#include"QDebug"
#include"QPainter"
#include"QTimer"
#include"QLabel"
#include<QJsonArray>
#include<QPushButton>
#include<QEventLoop>
#include"QMetaObject"
#include"menu.h"
Poffer::Poffer(SocketManager* socket, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poffer),
    client_socket(socket), // دریافت و نگه‌داری
    username(username),
    round{0}
{
    ui->setupUi(this);    
    this->setFixedSize(1300, 750);
    get_card();
    pause_button();
   /* connect(this, &Poffer::turn_showed, this,[=](){
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

*/
    connect(client_socket,&SocketManager::dataReceived,this,&Poffer::onServerResponse);
    connect(this, &Poffer::card_recived, this, &Poffer::choose_Card);
    connect(this,&Poffer::card_selected,this,&Poffer::show_wait);
    connect(this,&Poffer::round_result,this,&Poffer::finish_round);
    connect(this,&Poffer::game_over,this,&Poffer::finish_game);
    connect(pauseButton,&QPushButton::clicked,this,&Poffer::pause_requset);






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
    }
}

// این تابع جدید را به poffer.cpp اضافه کن

void Poffer::resetBoardForNewRound()
{
    qDebug() << "Resetting board for the next round...";

    // ۱. پاک کردن کارت‌های دست بازیکن از روی صفحه
    for (QPushButton* button : player_cards_place) {
        button->setIcon(QIcon()); // آیکون دکمه را خالی می‌کنیم
    }

    // ۲. پاک کردن کارت‌های دست حریف از روی صفحه
    for (QPushButton* button : op_cards_place) {
        button->setIcon(QIcon()); // آیکون دکمه را خالی می‌کنیم
    }

    // ۳. خالی کردن داده‌های دست فعلی بازیکن
    myhand.clear();
}

void Poffer::finish_round(const QVector<Card>& op_card, const QString& result, const QString& my_hand_rank, const QString& opponent_hand_rank, const QString& my_score, const QString& opponent_score)
{
    // ۱. نمایش کارت‌های دست حریف
    int cardWidth = 130;
    int cardHeight = 180;
    for (int i = 0; i < 5 && i < op_card.size(); ++i) {
        if (op_cards_place.size() > i && op_cards_place[i]) {
            op_cards_place[i]->setIcon(QIcon(op_card[i].imagePath));
            op_cards_place[i]->setIconSize(QSize(cardWidth, cardHeight));
        }
    }

    // ۲. آپدیت کردن امتیازات
    if (op_point_labale) op_point_labale->setText("Opponent Score: " + opponent_score);
    if (my_point_labale) my_point_labale->setText("Your Score: " + my_score);

    // ۳. ایجاد و نمایش لیبل‌های نتیجه در وسط صفحه
    QLabel* result_show = new QLabel(this);
    result_show->setText("Round Result: " + result);
    result_show->setAlignment(Qt::AlignCenter);
    result_show->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.7); font: 20pt 'Segoe UI'; border-radius: 10px; padding: 5px;");
    result_show->setGeometry(400, 300, 500, 50);
    result_show->show();

    QLabel* my_hand_rank_show = new QLabel(this);
    my_hand_rank_show->setText("Your Hand: " + my_hand_rank);
    my_hand_rank_show->setAlignment(Qt::AlignCenter);
    my_hand_rank_show->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.6); font: 14pt; padding: 5px;");
    my_hand_rank_show->setGeometry(400, 360, 500, 40);
    my_hand_rank_show->show();

    QLabel* opponent_hand_rank_show = new QLabel(this);
    opponent_hand_rank_show->setText("Opponent's Hand: " + opponent_hand_rank);
    opponent_hand_rank_show->setAlignment(Qt::AlignCenter);
    opponent_hand_rank_show->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.6); font: 14pt; padding: 5px;");
    opponent_hand_rank_show->setGeometry(400, 410, 500, 40);
    opponent_hand_rank_show->show();

    // ۴. شروع یک تایمر برای پاکسازی صفحه بعد از چند ثانیه
    QTimer::singleShot(6000, this, [=]() {
        // مخفی کردن و حذف لیبل‌های موقتی
        result_show->deleteLater();
        my_hand_rank_show->deleteLater();
        opponent_hand_rank_show->deleteLater();

        // تمیز کردن صفحه بازی برای راند بعدی
        resetBoardForNewRound();

        // اعلام آمادگی برای شروع راند بعدی
        // این سیگنال می‌تواند باعث شود کلاینت دوباره درخواست بازی بدهد یا منتظر پیام سرور بماند
        myhand.clear();
        player_cards_place.clear();
        op_cards_place.clear();
        emit turn_showed();
    });
}

void Poffer::pause_requset()
{
    QWidget* overlay = nullptr;

   // جلوگیری از اضافه کردن چندباره

    overlay = new QWidget(this);
    overlay->setGeometry(this->rect());
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    overlay->show();

    QPushButton* resumeButton = new QPushButton("Resume", overlay);
    resumeButton->setFixedSize(200, 80);
    resumeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"      // سبز ملایم
        "    color: white;"                    // متن سفید واضح
        "    font-size: 20px;"                 // متن کمی بزرگ‌تر
        "    font-weight: bold;"              // ضخیم برای دیده شدن بهتر
        "    padding: 10px 20px;"             // فاصله مناسب
        "    border: none;"
        "    border-radius: 12px;"            // گردی مناسب
        "}"
        "QPushButton:hover {"
        "    background-color: #45A049;"      // سبز تیره‌تر هنگام هاور
        "}"
        "QPushButton:pressed {"
        "    background-color: #3e8e41;"      // سبز تیره‌تر هنگام کلیک
        "}"
        );    resumeButton->move((overlay->width() - resumeButton->width()) / 2,(overlay->height() - resumeButton->height()) / 2 );
    resumeButton->show();
    connect(resumeButton, &QPushButton::clicked, this ,[=](){

        overlay->hide();
       // overlay->deleteLater();
    });


    QPushButton* exitButton = new QPushButton("Exit Game", overlay);
    exitButton->setFixedSize(200, 80);
    exitButton->move((overlay->width() - exitButton->width()) / 2, overlay->height() / 2 + 70);
    exitButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #E53935;"      // قرمز زیبا و قابل دید
        "    color: white;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #D32F2F;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #C62828;"
        "}"
        );

    // کانکشن بستن به اسلات خروج از بازی
    exitButton->show();
    connect(exitButton, &QPushButton::clicked, this, [=]() {
        // اگر نیاز است قبل از خروج پیام pause یا خروج ارسال شود اینجا بزن
        overlay->hide();
    });


}



void Poffer::onServerResponse(QByteArray data){
    /*
    if (waiting_label) {
        waiting_label->hide();
        waiting_label->deleteLater();
        waiting_label = nullptr;
    }
*/
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        //QMessageBox::warning(this, "خطا", "داده از سرور خراب است.");
        return;
    }
    QJsonObject obj = doc.object();


    if (obj["response"].toString()=="select_card_request"){
        //QMessageBox::warning(this,"card" , "card omad");
        QVector<Card> cards;
        QJsonArray card_array = obj["payload"].toObject()["cards"].toArray();
        for (auto val : card_array) {
            auto c = val.toObject();
            QString suit = c["suit"].toString();
            QString rank = c["rank"].toString();
            auto t = find_card(rank,suit);
            //  qDebug()<<t.rank;
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

    else if(obj["response"].toString()=="game_over"){
        auto payload =  obj["payload"].toObject();
        QString result = payload["result"].toString();
        QString Reson = payload["reson"].toString();
        emit game_over(result);



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


void Poffer::finish_game(QString result){

    QLabel* result_show = new QLabel(this);
    result_show->setText("result : " + result);
    result_show->setAlignment(Qt::AlignCenter);
    result_show->setStyleSheet("color: white; background-color: #444; font: 16pt;");
    result_show->setGeometry(400, 300, 500, 50);
    result_show->show();
    menu* mn = new menu();
    mn->show();
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
        //QMessageBox::warning(this, "خطا", "داده از سرور خراب است.");
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

void Poffer::pause_button(){
    QPushButton* ps = new QPushButton();
    pauseButton = new QPushButton(this);
    pauseButton->setIcon(QIcon(":/icons/pause.png")); // اگر ایکون داری
    pauseButton->setText("Pause");
    pauseButton->setCursor(Qt::PointingHandCursor);
    pauseButton->setFixedSize(100, 36);
    pauseButton->move(width() - pauseButton->width() - 15, 15); // گوشه بالا راست
    pauseButton->setStyleSheet(R"(
    QPushButton {
        background-color: #e74c3c;      /* قرمز گرم */
        color: white;                   /* متن سفید */
        border: none;
        border-radius: 8px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #c0392b;      /* تیره‌تر هنگام هاور */
    }
)");
}


void Poffer::choose_Card(QVector<Card> c, QString starter, bool a)
{
    // اگر دکمه‌های قدیمی از انتخاب قبلی وجود دارند، آنها را پاک می‌کنیم
    for (auto* button : findChildren<QPushButton*>()) {
        if (button->property("is_selection_card").toBool()) {
            button->deleteLater();
        }
    }

    int cardWidth = 130;
    int cardHeight = 180;
    int spacing = 20;
    int totalWidth = c.size() * cardWidth + (c.size() - 1) * spacing;
    int startX = (this->width() - totalWidth) / 2;
    int y = (this->height() - cardHeight) / 2;

    // با یک حلقه، برای هر کارت یک دکمه می‌سازیم
    for (int i = 0; i < c.size(); ++i) {
        QPushButton* button = new QPushButton(this);
        button->setProperty("is_selection_card", true); // یک مشخصه برای شناسایی این دکمه‌ها

        Card currentCard = c[i]; // کارت مربوط به این دکمه

        button->setIcon(QIcon(currentCard.imagePath));
        button->setIconSize(QSize(cardWidth, cardHeight));
        button->setFixedSize(cardWidth, cardHeight);
        button->move(startX + i * (cardWidth + spacing), y);
        button->show();

        // برای هر دکمه یک connect جداگانه با استفاده از لامبدا می‌نویسیم
        connect(button, &QPushButton::clicked, this, [=]() {
            qDebug() << "Card selected:" << currentCard.rank;

            // ۱. کارت انتخاب شده را به دست بازیکن اضافه می‌کنیم
            myhand.append(currentCard);

            // ۲. تصویر کارت را در جایگاه صحیح دست نهایی قرار می‌دهیم
            // اندیس صحیح، سایز فعلی دست قبل از اضافه کردن کارت جدید است
            int handIndex = myhand.size() - 1;
            if(handIndex < player_cards_place.size()) {
                player_cards_place[handIndex]->setIcon(QIcon(currentCard.imagePath));
                player_cards_place[handIndex]->setIconSize(QSize(cardWidth, cardHeight));
            }

            // ۳. پیام را برای سرور می‌سازیم و ارسال می‌کنیم
            QJsonObject card_obj = currentCard.toJson();
            QJsonObject payload;
            payload["selected_card"] = card_obj;
            QJsonObject mainobject;
            mainobject["command"] = "select_card";
            mainobject["payload"] = payload;

            QJsonDocument doc(mainobject);
            client_socket->sendData(doc.toJson(QJsonDocument::Compact));

            // ۴. تمام دکمه‌های انتخاب کارت را از صفحه حذف می‌کنیم
            for (auto* btn : findChildren<QPushButton*>()) {
                if (btn->property("is_selection_card").toBool()) {
                    btn->hide();
                    btn->deleteLater();
                }
            }

            // ۵. سیگنال می‌دهیم که انتخاب انجام شد
            emit card_selected();
        });
    }
}

/*void Poffer::choose_Card(QVector<Card> c, QString starter, bool a) {
    for(auto a:c){

        qDebug()<<a.suit;
    }
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





    if(c.size()>0){

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
        //QMessageBox::information(this, "Card Selected", "You selected: " + c[0].rank + " of " + c[0].suit);
        // ساختن آبجکت کارت انتخاب‌شده
        QJsonObject selected_card;
        selected_card["rank"] = c[0].rank;
        selected_card["suit"] = c[0].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[0]);
        player_cards_place[round]->setIcon(c[0].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit(card_selected());


    });

    }

    if(c.size()>1){
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
        //QMessageBox::information(this, "Card Selected", "You selected: " + c[1].rank + " of " + c[1].suit);
        QJsonObject selected_card;
        selected_card["rank"] = c[1].rank;
        selected_card["suit"] = c[1].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[1]);
        player_cards_place[round]->setIcon(c[1].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit card_selected();
    });
    }
    if(c.size()>2){

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
       // QMessageBox::information(this, "Card Selected", "You selected: " + c[2].rank + " of " + c[2].suit);
        QJsonObject selected_card;
        selected_card["rank"] = c[2].rank;
        selected_card["suit"] = c[2].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[2]);
        player_cards_place[round]->setIcon(c[2].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit(card_selected());

    });
    }

    // کارت چهارم
    if(c.size()>3){
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
      //  QMessageBox::information(this, "Card Selected", "You selected: " + c[3].rank + " of " + c[3].suit);
        QJsonObject selected_card;
        selected_card["rank"] = c[3].rank;
        selected_card["suit"] = c[3].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[3]);
        player_cards_place[round]->setIcon(c[3].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit(card_selected());

    });
    }

    // کارت پنجم
    if(c.size()>4){
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
       // QMessageBox::information(this, "Card Selected", "You selected: " + c[4].rank + " of " + c[4].suit);
        QJsonObject selected_card;
        selected_card["rank"] = c[4].rank;
        selected_card["suit"] = c[4].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[4]);
        player_cards_place[round]->setIcon(c[4].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit(card_selected());

    });
    }

    // کارت ششم
    if(c.size()>5){
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
      //  QMessageBox::information(this, "Card Selected", "You selected: " + c[5].rank + " of " + c[5].suit);
        QJsonObject selected_card;
        selected_card["rank"] = c[5].rank;
        selected_card["suit"] = c[5].suit;

        // ساختن payload
        QJsonObject payload;
        payload["selected_card"] = selected_card;

        // ساختن mainobject
        QJsonObject mainobject;
        mainobject["command"] = "select_card";
        mainobject["payload"] = payload;

        // ست کردن آیکن کارت
        myhand.push_back(c[5]);
        player_cards_place[round]->setIcon(c[5].imagePath);
        player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

        // ساختن JSON و آماده‌سازی برای ارسال
        QJsonDocument doc(mainobject);
        QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

        // ارسال
        client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها
        button1->hide();
        button2->hide();
        button3->hide();
        button4->hide();
        button5->hide();
        button6->hide();
        button7->hide();
        emit(card_selected());

    });
    }

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
           // QMessageBox::information(this, "Card Selected", "You selected: " + c[6].rank + " of " + c[6].suit);
            QJsonObject selected_card;
            selected_card["rank"] = c[6].rank;
            selected_card["suit"] = c[6].suit;

            // ساختن payload
            QJsonObject payload;
            payload["selected_card"] = selected_card;

            // ساختن mainobject
            QJsonObject mainobject;
            mainobject["command"] = "select_card";
            mainobject["payload"] = payload;

            // ست کردن آیکن کارت
            myhand.push_back(c[6]);
            player_cards_place[round]->setIcon(c[6].imagePath);
            player_cards_place[round]->setIconSize(QSize(cardWidth, cardHeight));

            // ساختن JSON و آماده‌سازی برای ارسال
            QJsonDocument doc(mainobject);
            QByteArray json_to_send = doc.toJson(QJsonDocument::Indented);

            // ارسال
            client_socket->sendData(json_to_send); // برای جلوگیری از چسبیدن پیام‌ها

            // ارسال به سرور
            button1->hide();
            button2->hide();
            button3->hide();
            button4->hide();
            button5->hide();
            button6->hide();
            button7->hide();
            emit(card_selected());

        });
    }

}*/




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
        //QMessageBox::warning(this,"warning","sevrer couldnot response");
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
    round++;
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
    deck.append(Card{"DIAMOND", "SOLDIER", ":/img/Dimond-Soldier.JPG"});
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
    deck.append(Card{"DOLLAR", "SOLDIER", ":/img/Dollar-Soldier.JPG"});
    deck.append(Card{"DOLLAR", "QUEEN", ":/img/Dollar-Queen.JPG"});
    deck.append(Card{"DOLLAR", "KING", ":/img/Dollar-King.JPG"});
    deck.append(Card{"DOLLAR", "BITCOIN", ":/img/Dollar-Bitcoin.JPG"});


}



Poffer::~Poffer()
{
    delete ui;
}





