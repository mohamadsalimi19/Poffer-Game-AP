#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qdebug.h>
#include<ostream>
#include<QSettings>
#include<socketmanager.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QSettings settings("MyCompany", "MyApp");
   //  signedIn = settings.value("signed_in", false).toBool()
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    SocketManager* ms = new SocketManager();
    ms->connectToServer("example.com", 80);
    QString request = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
    ms->sendData(request.toUtf8());


/*
    if(!signedIn){
        this->close();
        signup* sp = new signup();
        sp->show();
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("signed_in", true);
        settings.sync();
    }

    else{
        Login* lp = new Login();
        this->close();
        lp->show();


    }





*/



}

