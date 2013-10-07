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
    int optionCount=4;
    std::string lenghtArray[optionCount];
    lenghtArray[0]="15 seconds";
    lenghtArray[1]="30 seconds";
    lenghtArray[2]="60 seconds";
    lenghtArray[3]="90 seconds";


    for(int i=0;i<optionCount;i++){
        ui->cmbClipLength->addItem(lenghtArray[i].c_str());
    }
    clearStatusBar();
    connect(p,SIGNAL(statusUpdated()),this,SLOT(updateStatusBar()));
    connect(p,SIGNAL(clipNotAvailable()),this,SLOT(updateStatusBar()));
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
        ui->txtClientAddr->setEnabled(false);
        ui->cmbClipLength->setEnabled(false);
        ui->BtnStartCtsRecord->setText("Stop");
        ui->statusBar->showMessage("Started",3000);
    }else{
        //code to stop recording
        ui->BtnStartCtsRecord->setText("Please wait..");
        ui->statusBar->showMessage("wait until current process finishes",3000);
        p->setRecording(false);
        p->setStreaming(false);
        p->deleteTempFile();
        p->resetAll();
        ui->txtClientAddr->setEnabled(true);
        ui->cmbClipLength->setEnabled(true);
        ui->BtnStartCtsRecord->setText("Start");

    }
}




void MainWindow::on_cmbClipLength_currentIndexChanged(const QString &arg1)
{    
    p->setClipLength(arg1);
}

void MainWindow::updateStatusBar(){
    std::string msg="Now recording: _";
    std::string msg2=" :Motion detected:";
    std::string msg3=" :Clip unavailable:";
    msg[msg.length()-1]=p->giveClipNumber();
    if(p->MotionLastMin()){
        msg.append(msg2);
    }    
    if(!p->isMotionClipAvailable()){
        msg.append(msg3);
    }
    ui->statusBar->showMessage(msg.c_str());
}

void MainWindow::clearStatusBar(){
    ui->statusBar->showMessage("Welcome to VLC Security Camera Solution",3000);
}
