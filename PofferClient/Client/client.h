#ifndef CLIENT_H
#define CLIENT_H

#include"qgraphicsview.h"
#include"QGraphicsScene"
#include"qpixmap.h"
#include"QPixmap"
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include<qpropertyanimation.h>
#include<qtimer.h>
#include<QSet>
#include<QRandomGenerator>
#include<algorithm>
QT_BEGIN_NAMESPACE
namespace Ui {
class Client;
}
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT
    QGraphicsScene* scene;
    QMap<QString,QPixmap> all_card; // map for saving pic

public:
    Client(QWidget *parent = nullptr);
    ~Client();
    void read_cards(); // func to read and save pic
private:
    Ui::Client *ui;
};
#endif // CLIENT_H
