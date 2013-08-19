#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <player.h>

bool isplay=true;
player *p=new player();


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->Btnplay,SIGNAL(clicked()),this,SLOT(on_Btnplay_clicked()));


    ui->display->setAutoFillBackground( true );
    QPalette plt = palette();
    plt.setColor( QPalette::Window, Qt::black );
    ui->display->setPalette( plt );

    p->load(ui->display);

    ui->Btnplay->setText("play");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btnplay_clicked()
{
    p->play(ui->Btnplay);
}

void MainWindow::on_BtnStream_clicked()
{
    QLineEdit *le=ui->txtClientAddr;
    QString addr=le->text();    
    p->setClientAddress(addr);
    p->stream(ui->BtnStream);
}

void MainWindow::on_BtnReceive_clicked()
{
    p->receiveStream(ui->display,ui->BtnReceive);
}
