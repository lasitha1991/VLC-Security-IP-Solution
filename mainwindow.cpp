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
    p->receiveStream();
}

void MainWindow::on_BtnWebCam_clicked()
{
    if(!p->isWebcamOn()){
        p->setWebcam(true);
        p->loadWebCam();
        ui->BtnWebCam->setText("WebcamLoaded");
        p->play();
    }else{
        p->setWebcam(false);
        p->stop();
        p->releaseDisplay();
        ui->BtnWebCam->setText("Webcam");
    }
}


void MainWindow::on_BtnFromLastMinStream_clicked()
{
    if(!p->isStreaming()){
        setClient();
        p->setStreaming(true);
        p->streamLastMinute();
        ui->BtnFromLastMinStream->setText("Stop Streaming");
    }else{
        //code to stop streaming
        p->setStreaming(false);
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
    setClient();
    if(!p->isRecording()){
        p->setRecording(true);
        p->saveWebcamToFile();
        ui->BtnStartCtsRecord->setText("Stop Continuous Recording");
    }else{
        //code to stop recording
        p->setRecording(false);
        ui->BtnStartCtsRecord->setText("Start Continuous Recording");
    }
}

void MainWindow::on_BtnLiveStream_clicked()
{
    if(!(p->isStreaming())){
        p->setStreaming(true);
        p->streamLive();
        ui->BtnLiveStream->setText("Stop Live Stream");
    }else{
        p->setStreaming(false);
        ui->BtnLiveStream->setText("Live Stream");
    }
}

void MainWindow::on_BtnProcessVid_clicked()
{

    p->startVideoProcess();
}
