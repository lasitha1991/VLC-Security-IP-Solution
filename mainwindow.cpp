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
    recording=false;
    streaming=false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}

void MainWindow::on_Btnplay_clicked()
{
    p->play();    
}

void MainWindow::on_BtnStream_clicked()
{
    setClient();
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    p->stream('0',st);
}

void MainWindow::on_BtnReceive_clicked()
{
    p->receiveStream(ui->BtnReceive);
}

void MainWindow::on_BtnWebCam_clicked()
{
    p->loadWebCam();
    ui->Btnplay->setText("Play Webcam");
    ui->BtnWebCam->setText("WebcamLoaded");
}


void MainWindow::on_BtnFromLastMinStream_clicked()
{
    if(!streaming){
        setClient();
        p->setStreaming(true);
        p->streamLastMinute();
        streaming=true;
        ui->BtnFromLastMinStream->setText("Stop Streaming");
    }else{
        //code to stop streaming
        p->setStreaming(false);
        streaming=false;
        ui->BtnFromLastMinStream->setText("Start Streaming From Last Minute");
    }
}



void MainWindow::setClient()
{
    QLineEdit *le=ui->txtClientAddr;
    QString addr=le->text();
    p->setClientAddress(addr);
}



void MainWindow::on_BtnStartCtsRecord_clicked()
{
    if(!recording){
        p->setRecording(true);
        p->saveWebcamToFile();
        recording=true;
        ui->BtnStartCtsRecord->setText("Stop Continuous Recording");
    }else{
        //qDebug("Already recording");
        //code to stop recording
        p->setRecording(false);
        recording=false;
        ui->BtnStartCtsRecord->setText("Start Continuous Recording");
    }
}

void MainWindow::on_BtnLiveStream_clicked()
{
    p->streamLive();
}
