//Lasitha Weerasinghe
//100577A

#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include "ui_mainwindow.h"
#include "streamthread.h"
#include "filesavethread.h"
#include "motiondetector.h"

std::string soutraw="#transcode{vcodec=mp4v,vb=0,scale=0}:file{dst=_capture.mp4}";
//char *serverAddress="udp://127.0.0.1:1234";


player::player(QObject *parent) :    QObject(parent)
{
    inst = libvlc_new(0, NULL);
    streamInst=libvlc_new(0,NULL);
    clipNumber='0';
    boolstream=false;
    boolrecord=false;
    clientAddress="udp://127.0.0.1";
    mdetect=new MotionDetector();
    //sThread=new StreamThread();
    connect(mdetect,SIGNAL(motionDetected()),this,SLOT(processMotionDetected()));
}

void player::play()
{
    if (!mp)
        return;

    if (libvlc_media_player_is_playing(mp))
    {
        /* Stop */
        //libvlc_media_player_pause(mp);
        stop();
    }
    else
    {
        /* Play again */
        libvlc_media_player_play(mp);
    }
}
void player::playStream(char StreamClip){
    std::string ccname="_capture.mp4";
    ccname[0]=StreamClip;
    load((char*)ccname.c_str());
    play();
}

void player::stop()
{
    if(mp){
        // stop playing
        libvlc_media_player_stop(mp);

        // free the media_player
        libvlc_media_player_release(mp);

        libvlc_release(inst);
        mp=NULL;
    }
}
void player::setDisplayWidget(QWidget *dis){
    displayWid=dis;
}

void player::load(char* fileName){
    // create a new item
    m = libvlc_media_new_path(inst, fileName);
    if(!m){
        exit(EXIT_FAILURE);
    }
    // create a media play playing environment

    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);
    // play the media_player

#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(mp, (void *)displayWid->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mp,displayWid->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, (HWND)displayWid->winId());
#endif
}




void player::pause(){    
    if(libvlc_media_player_can_pause(mp)){
        libvlc_media_player_pause(mp);
    }else{
        exit(EXIT_FAILURE);
    }
}



void player::receiveStream(){
    //load(serverAddress);
}
void player::loadWebCam(){
    libvlc_media_player_release(mp);
    // create a new item
    m = libvlc_media_new_path(inst, "v4l2:///dev/video0");
    if(!m)
        exit(EXIT_FAILURE);
    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);
    // play the media_player

#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(mp, (void *)displayWid->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mp,displayWid->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, displayWid->winId());
#endif
}


void player::setClientAddress(QString addr){
    addr=addr.toLower();
    if((addr.mid(0,3).compare("udp"))){
        addr="udp://"+addr;
    }
    QString ipPart=addr.mid(6,addr.length());
    QRegExp rx("*.*.*.*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    if(!(rx.exactMatch(ipPart))){  // if the format is incorrect in given string it is set to a default value
        addr="udp://127.0.0.1";
    }
    clientAddress=addr.toStdString();
}
char* player::giveClientAddress(){    
    return (char*)clientAddress.c_str();
}

void player::stream(char StreamClip){ //starts a unicast stream thread
    //st->setInst(streamInst,StreamClip,this->giveClientAddress()); //streams thread sets data for streaming
    sThread->setInst(streamInst,StreamClip,this->giveClientAddress());
    connect(sThread,SIGNAL(finished()),this,SLOT(releaseDisplay()));  //after finishing stream release the display to start a new media
    sThread->start();  //streaming starts
    qDebug("Streaming clip:%c",StreamClip);
}

void player::saveWebcamToFile(){
    qDebug("recored clip:%c",clipNumber);
    std::string sout;
    sout=soutraw;
    sout[46]=clipNumber;

    FilesaveThread *ft=new FilesaveThread();
    connect(ft,SIGNAL(finished()),this,SLOT(increaseClipNumber()));  //when filesave thread is finished increase the clip by one
    connect(ft,SIGNAL(finished()),this,SLOT(saveWebcamToFile()));    //when filesave is finished start it again

    /////////////////////////////////////
    connect(ft,SIGNAL(finished()),this,SLOT(startVideoProcess()));
    ////////////////////////////////////


    ft->setInst(inst,"v4l2:///dev/video0",(char*)sout.c_str());

    ft->start();

    if(!boolrecord){  //if set to false disconnect the connections
        disconnect(ft,SIGNAL(finished()),this,SLOT(increaseClipNumber()));
        disconnect(ft,SIGNAL(finished()),this,SLOT(saveWebcamToFile()));
        /////
        disconnect(ft,SIGNAL(finished()),this,SLOT(startVideoProcess()));
        /////
        ft->terminate();
    }        
}
void player::streamLive(){
    releaseDisplay();
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    connect(st,SIGNAL(finished()),this,SLOT(streamLive()));
    stream('9');         //9 to represent live stream
    if(!(this->isStreaming())){
        disconnect(st,SIGNAL(finished()),this,SLOT(streamLive()));
        st->terminate();
    }
}

void player::increaseClipNumber(){
    if(clipNumber!='5'){
        clipNumber++;
    }else{              //if clipnumber is 5 set it to 0
        clipNumber='0';
    }
    if(!MotionLastMin()){
        boolstream=false;
    }
    if(boolstream){
        streamLastMinute();
    }
}

void player::streamLastMinute(){  //if player is at one instance stream last three instances
    releaseDisplay();
     //creating an object instance prevents destroying thread while running
    sThread=new StreamThread();
    char StreamClip;
    if(clipNumber=='0'){
        //stream the clip which is 1 minute before processed clip
        StreamClip='1';//'2';
    }
    else if(clipNumber=='1'){
        StreamClip='2';
    }else if(clipNumber=='2'){
        StreamClip='3';
    }else if(clipNumber=='3'){
        StreamClip='4';
    }else if(clipNumber=='4'){
        StreamClip='5';
    }else if(clipNumber=='5'){
        StreamClip='0';
    }
    stream(StreamClip);
    playStream(StreamClip);
    //QThread::connect(sThread,SIGNAL(finished()),this,SLOT(streamLastMinute()));

    //if(!boolstream){
      //  QThread::disconnect(sThread,SIGNAL(finished()),this,SLOT(streamLastMinute()));
      //  sThread->terminate();
    //}

    /////////////
    //increaseClipNumber();
    qDebug("clip:%c MotionClip:%c",clipNumber,motionClipNumber);
    ////////////////
}


void player::setRecording(bool val){
    boolrecord=val;
}
void player::setStreaming(bool val){
    boolstream=val;
}
void player::setWebcam(bool val){
    boolwebcamon=val;
}

bool player::isRecording(){
    return boolrecord;
}
bool player::isStreaming(){
    return boolstream;
}
bool player::isWebcamOn(){
    return boolwebcamon;
}

char player::giveClipNumber(){
    return clipNumber;
}

void player::releaseDisplay(){
    if(mp){
        libvlc_media_player_release(mp);
        //qDebug("media player releasing code");
    }
}

void player::startVideoProcess(){    
    char processClip=clipNumber;
    if(processClip!='0'){
        processClip--;
    }else{
        processClip='5';
    }
    qDebug("process clip: %c Clip:%c",processClip,clipNumber);
    mdetect->setClip(processClip);
    mdetect->start();

}
void player::processMotionDetected(){
    //qDebug("Signal received");
    if(clipNumber!='0'){
        motionClipNumber=clipNumber-1;
    }else{
        motionClipNumber='5';
    }
    if(!isStreaming()){
        setStreaming(true);
        //streamLastMinute();
        //qDebug("Start Stream");
    }
}
bool player::MotionLastMin(){
    if(motionClipNumber=='0'){
        if(clipNumber=='5'){
            return false;
        }
    }else if(motionClipNumber=='1'){
        if(clipNumber=='0'){
            return false;
        }
    }else if(motionClipNumber=='2'){
        if(clipNumber=='1'){
            return false;
        }
    }else if(motionClipNumber=='3'){
        if(clipNumber=='2'){
            return false;
        }
    }else if(motionClipNumber=='4'){
        if(clipNumber=='3'){
            return false;
        }
    }else if(motionClipNumber=='5'){
        if(clipNumber=='4'){
            return false;
        }
    }
    return true;
}
