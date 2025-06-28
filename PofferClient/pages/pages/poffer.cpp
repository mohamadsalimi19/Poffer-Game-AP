#include "poffer.h"
#include "ui_poffer.h"
#include"QMessageBox"
#include"QDebug"
#include"QPainter"
Poffer::Poffer(SocketManager* socket, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poffer),
    client_socket(socket), // دریافت و نگه‌داری
    username(username)
{
    ui->setupUi(this);    
    this->setFixedSize(this->width(), this->height());

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
    QString starter;
    Card player_startcard;
    Card opp_startcard;
    choose_turn(starter,player_startcard,opp_startcard);
    show_turn(player_startcard,opp_startcard);

}


void Poffer::show_turn(Card p , Card o){
    QPixmap pix_player(p.imagePath);
    QPixmap pix_enemy(o.imagePath);
    ui->label->setPixmap(pix_player.scaled(ui->label->size(), Qt::KeepAspectRatio));
    ui->label_2->setPixmap(pix_enemy.scaled(ui->label->size(), Qt::KeepAspectRatio));


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
      "suit": "Dimond",
      "rank": "8"
    },
    "opponent_card": {
      "suit": "Dimond",
      "rank": "7"
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














void Poffer::get_card(){

    // COIN
    deck.append(Card{"Coin", "2", ":/img/Coin-2.JPG"});
    deck.append(Card{"Coin", "3", ":/img/Coin-3.JPG"});
    deck.append(Card{"Coin", "4", ":/img/Coin-4.JPG"});
    deck.append(Card{"Coin", "5", ":/img/Coin-5.JPG"});
    deck.append(Card{"Coin", "6", ":/img/Coin-6.JPG"});
    deck.append(Card{"Coin", "7", ":/img/Coin-7.JPG"});
    deck.append(Card{"Coin", "8", ":/img/Coin-8.JPG"});
    deck.append(Card{"Coin", "9", ":/img/Coin-9.JPG"});
    deck.append(Card{"Coin", "10", ":/img/Coin-10.JPG"});
    deck.append(Card{"Coin", "Solider", ":/img/Coin-Soldier.JPG"});
    deck.append(Card{"Coin", "Queen", ":/img/Coin-Queen.JPG"});
    deck.append(Card{"Coin", "Dolar", ":/img/Coin-King.JPG"});
    deck.append(Card{"Coin", "Bitcoin", ":/img/Coin-Bitcoin.JPG"});

    // GOLD
    deck.append(Card{"Gold", "2", ":/img/Gold-2.JPG"});
    deck.append(Card{"Gold", "3", ":/img/Gold-3.JPG"});
    deck.append(Card{"Gold", "4", ":/img/Gold-4.JPG"});
    deck.append(Card{"Gold", "5", ":/img/Gold-5.JPG"});
    deck.append(Card{"Gold", "6", ":/img/Gold-6.JPG"});
    deck.append(Card{"Gold", "7", ":/img/Gold-7.JPG"});
    deck.append(Card{"Gold", "8", ":/img/Gold-8.JPG"});
    deck.append(Card{"Gold", "9", ":/img/Gold-9.JPG"});
    deck.append(Card{"Gold", "10", ":/img/Gold-10.JPG"});
    deck.append(Card{"Gold", "Solider", ":/img/Gold-Soldier.JPG"});
    deck.append(Card{"Gold", "Queen", ":/img/Gold-Queen.JPG"});
    deck.append(Card{"Gold", "Dolar", ":/img/Gold-King.JPG"});
    deck.append(Card{"Gold", "Bitcoin", ":/img/Gold-Bitcoin.JPG"});

    // DIAMOND
    deck.append(Card{"Dimond", "2", ":/img/Dimond-2.JPG"});
    deck.append(Card{"Dimond", "3", ":/img/Dimond-3.JPG"});
    deck.append(Card{"Dimond", "4", ":/img/Dimond-4.JPG"});
    deck.append(Card{"Dimond", "5", ":/img/Dimond-5.JPG"});
    deck.append(Card{"Dimond", "6", ":/img/Dimond-6.JPG"});
    deck.append(Card{"Dimond", "7", ":/img/Dimond-7.JPG"});
    deck.append(Card{"Dimond", "8", ":/img/Dimond-8.JPG"});
    deck.append(Card{"Dimond", "9", ":/img/Dimond-9.JPG"});
    deck.append(Card{"Dimond", "10", ":/img/Dimond-10.JPG"});
    deck.append(Card{"Dimond", "Solider", ":/img/Dimond-Solider.JPG"});
    deck.append(Card{"Dimond", "Queen", ":/img/Dimond-Queen.JPG"});
    deck.append(Card{"Dimond", "Dolar", ":/img/Dimond-Dolar.JPG"});
    deck.append(Card{"Dimond", "Bitcoin", ":/img/Dimond-Bitcoin.JPG"});

    // DOLAR
    deck.append(Card{"Dollar", "2", ":/img/Dollar-2.JPG"});
    deck.append(Card{"Dollar", "3", ":/img/Dollar-3.JPG"});
    deck.append(Card{"Dollar", "4", ":/img/Dollar-4.JPG"});
    deck.append(Card{"Dollar", "5", ":/img/Dollar-5.JPG"});
    deck.append(Card{"Dollar", "6", ":/img/Dollar-6.JPG"});
    deck.append(Card{"Dollar", "7", ":/img/Dollar-7.JPG"});
    deck.append(Card{"Dollar", "8", ":/img/Dollar-8.JPG"});
    deck.append(Card{"Dollar", "9", ":/img/Dollar-9.JPG"});
    deck.append(Card{"Dollar", "10", ":/img/Dollar-10.JPG"});
    deck.append(Card{"Dollar", "Solider", ":/img/Dollar-Solider.JPG"});
    deck.append(Card{"Dollar", "Queen", ":/img/Dollar-Queen.JPG"});
    deck.append(Card{"Dollar", "Dolar", ":/img/Dollar-King.JPG"});
    deck.append(Card{"Dollar", "Bitcoin", ":/img/Dollar-Bitcoin.JPG"});

}









Poffer::~Poffer()
{
    delete ui;
}
