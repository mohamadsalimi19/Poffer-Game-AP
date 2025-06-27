#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QPixmap background(":/img/Game-Ground-Photo.JPG");
    QGraphicsPixmapItem* background_itme = scene->addPixmap(background);
    background_itme->setZValue(-1);








}





void Client::read_cards(){

    all_card["Coin-2"] = QPixmap(":/Coin-Cards/Coin-2.JPG");
    all_card["Coin-3"] = QPixmap(":/Coin-Cards/Coin-3.JPG");
    all_card["Coin-4"] = QPixmap(":/Coin-Cards/Coin-4.JPG");
    all_card["Coin-5"] = QPixmap(":/Coin-Cards/Coin-5.JPG");
    all_card["Coin-6"] = QPixmap(":/Coin-Cards/Coin-6.JPG");
    all_card["Coin-7"] = QPixmap(":/Coin-Cards/Coin-7.JPG");
    all_card["Coin-8"] = QPixmap(":/Coin-Cards/Coin-8.JPG");
    all_card["Coin-9"] = QPixmap(":/Coin-Cards/Coin-9.JPG");
    all_card["Coin-10"] = QPixmap(":/Coin-Cards/Coin-10.JPG");
    all_card["Coin-Bitcoin"] = QPixmap(":/Coin-Cards/Coin-Bitcoin.JPG");
    all_card["Coin-King"] = QPixmap(":/Coin-Cards/Coin-King.JPG");
    all_card["Coin-Queen"] = QPixmap(":/Coin-Cards/Coin-Queen.JPG");
    all_card["Coin-Soldier"] = QPixmap(":/Coin-Cards/Coin-Soldier.JPG");

    all_card["Gold-2"] = QPixmap(":/Gold-Cards/Gold-2.JPG");
    all_card["Gold-3"] = QPixmap(":/Gold-Cards/Gold-3.JPG");
    all_card["Gold-4"] = QPixmap(":/Gold-Cards/Gold-4.JPG");
    all_card["Gold-5"] = QPixmap(":/Gold-Cards/Gold-5.JPG");
    all_card["Gold-6"] = QPixmap(":/Gold-Cards/Gold-6.JPG");
    all_card["Gold-7"] = QPixmap(":/Gold-Cards/Gold-7.JPG");
    all_card["Gold-8"] = QPixmap(":/Gold-Cards/Gold-8.JPG");
    all_card["Gold-9"] = QPixmap(":/Gold-Cards/Gold-9.JPG");
    all_card["Gold-10"] = QPixmap(":/Gold-Cards/Gold-10.JPG");
    all_card["Gold-King"] = QPixmap(":/Gold-Cards/Gold-King.JPG");
    all_card["Gold-Queen"] = QPixmap(":/Gold-Cards/Gold-Queen.JPG");
    all_card["Gold-Soldier"] = QPixmap(":/Gold-Cards/Gold-Soldier.JPG");

    all_card["Dollar-2"] = QPixmap(":/Dollar-Cards/Dollar-2.JPG");
    all_card["Dollar-3"] = QPixmap(":/Dollar-Cards/Dollar-3.JPG");
    all_card["Dollar-4"] = QPixmap(":/Dollar-Cards/Dollar-4.JPG");
    all_card["Dollar-5"] = QPixmap(":/Dollar-Cards/Dollar-5.JPG");
    all_card["Dollar-6"] = QPixmap(":/Dollar-Cards/Dollar-6.JPG");
    all_card["Dollar-7"] = QPixmap(":/Dollar-Cards/Dollar-7.JPG");
    all_card["Dollar-8"] = QPixmap(":/Dollar-Cards/Dollar-8.JPG");
    all_card["Dollar-9"] = QPixmap(":/Dollar-Cards/Dollar-9.JPG");
    all_card["Dollar-10"] = QPixmap(":/Dollar-Cards/Dollar-10.JPG");
    all_card["Dollar-King"] = QPixmap(":/Dollar-Cards/Dollar-King.JPG");
    all_card["Dollar-Queen"] = QPixmap(":/Dollar-Cards/Dollar-Queen.JPG");
    all_card["Dollar-Soldier"] = QPixmap(":/Dollar-Cards/Dollar-Soldier.JPG");





}







Client::~Client()
{
    delete ui;
}
