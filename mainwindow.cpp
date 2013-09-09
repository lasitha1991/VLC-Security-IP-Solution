#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <player.h>


bool isplay=true;
player *p;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    p=new player();

    ui->setupUi(this);

    ui->display->setAutoFillBackground( true );
    QPalette plt = palette();
    plt.setColor( QPalette::Window, Qt::black );
    ui->display->setPalette( plt );

    p->load(ui->display);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

void MainWindow::on_Btnplay_clicked()
{
    p->play(ui->Btnplay);
}

void MainWindow::on_BtnStream_clicked()
{
    setClient();
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    p->stream('0',st);
}

void MainWindow::on_BtnReceive_clicked()
{
    p->receiveStream(ui->display,ui->BtnReceive);
}

void MainWindow::on_BtnWebCam_clicked()
{
    p->loadWebCam(ui->display,ui->BtnWebCam);
}

void MainWindow::on_BtnSaveWebCam_clicked()
{
    p->saveWebcamToFile();    
}

void MainWindow::on_BtnLastMinStream_clicked()
{
    setClient();
    p->streamLastMinute();
}

void MainWindow::on_BtnRecoreOneMin_clicked()
{
    p->recordOneMin();
}

void MainWindow::setClient()
{
    QLineEdit *le=ui->txtClientAddr;
    QString addr=le->text();
    p->setClientAddress(addr);
}
