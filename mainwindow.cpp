//Lasitha Weerasinghe
//100577A

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
    p->setDisplayWidget(ui->display);
    p->load("0capture.mp4");

    ui->cmbClipLength->addItem("5");
    ui->cmbClipLength->addItem("20");
    ui->cmbClipLength->addItem("30");
    ui->cmbClipLength->addItem("10");
    ui->statusBar->showMessage("Welcome to VLC Security Camera Solution",4000);
}

MainWindow::~MainWindow()
{
    p->deleteTempFile();
    delete ui;
    delete p;
}



void MainWindow::setClient()
{
    QLineEdit *le=ui->txtClientAddr;
    QString addr=le->text();
    p->setClientAddress(addr);
}



void MainWindow::on_BtnStartCtsRecord_clicked()
{
    setClient();
    if(!p->isRecording()){
        p->setRecording(true);
        p->saveWebcamToFile();
        ui->BtnStartCtsRecord->setText("Exit");
        ui->statusBar->showMessage("Started",1000);
    }else{
        //code to stop recording
        p->setRecording(false);
        p->deleteTempFile();
        ui->BtnStartCtsRecord->hide();
        ui->statusBar->showMessage("wait until current stream finishes",1000);
    }
}




void MainWindow::on_cmbClipLength_currentIndexChanged(const QString &arg1)
{
    int l=arg1.toInt();
    p->setClipLength(l);
}
