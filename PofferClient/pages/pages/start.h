#ifndef GAME_H
#define GAME_H
#include <QWidget>
#include<socketmanager.h>
namespace Ui {
class start;
}

class start : public QWidget
{
    Q_OBJECT
    SocketManager* client_socket;
    QByteArray jsonData;
    QString username;
public:
   QByteArray make_requset_json();
    bool get_start_response();
   explicit start(const QString& un, SocketManager* c, QWidget* parent = nullptr);
    ~start();
    void set_username(QString un , SocketManager* client);
    void start_game();

public slots:

private:
    Ui::start *ui;
};

#endif // GAME_H
