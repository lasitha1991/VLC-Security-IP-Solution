#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include "ui_mainwindow.h"
#include "streamthread.h"
#include "filesavethread.h"


std::string soutraw="#transcode{vcodec=mp4v,vb=0,scale=0}:file{dst=_capture.mp4}";
char *serverAddress="udp://127.0.0.1:1234";


player::player(QObject *parent) :    QObject(parent)
{
    inst = libvlc_new(0, NULL);
    streamInst=libvlc_new(0,NULL);    
    clipNumber='0';
    boolstream=false;
    boolrecord=false;
}

void player::play()
{
    if (!mp)
        return;

    if (libvlc_media_player_is_playing(mp))
    {
        /* Pause */
        libvlc_media_player_pause(mp);
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
    libvlc_media_player_set_hwnd(mp, displayWid->winId());
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
    load(serverAddress);
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
    if(!(rx.exactMatch(ipPart))){
        addr="udp://127.0.0.1";
    }
    clientAddress=addr.toStdString();
}
char* player::giveClientAddress(){    
    return (char*)clientAddress.c_str();
}

void player::stream(char StreamClip,StreamThread *st){ //starts a unicast stream thread
    st->setInst(streamInst,StreamClip,this->giveClientAddress()); //streams thread sets data for streaming   
    connect(st,SIGNAL(finished()),this,SLOT(releaseDisplay()));
    st->start();  //streaming starts

}

void player::saveWebcamToFile(){    
    std::string sout;
    sout=soutraw;
    sout[46]=clipNumber;

    FilesaveThread *ft=new FilesaveThread();
    connect(ft,SIGNAL(finished()),this,SLOT(increaseClipNumber()));  //when filesave thread is finished increase the clip by one
    connect(ft,SIGNAL(finished()),this,SLOT(saveWebcamToFile()));    //when filesave is finished start it again
    ft->setInst(inst,"v4l2:///dev/video0",(char*)sout.c_str());

    ft->start();

    if(!boolrecord){  //if set to false disconnect the connections
        disconnect(ft,SIGNAL(finished()),this,SLOT(increaseClipNumber()));
        disconnect(ft,SIGNAL(finished()),this,SLOT(saveWebcamToFile()));
        ft->terminate();
    }

}
void player::streamLive(){
    releaseDisplay();
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    connect(st,SIGNAL(finished()),this,SLOT(streamLive()));
    stream('9',st);
    if(!(this->isStreaming())){
        disconnect(st,SIGNAL(finished()),this,SLOT(streamLive()));
        st->terminate();
    }
}

void player::increaseClipNumber(){
    if(clipNumber!='4'){
        clipNumber++;
    }else{              //if clipnumber is 4 set it to 0
        clipNumber='0';
    }
}

void player::streamLastMinute(){  //if player is at one instance stream last three instances
    releaseDisplay();
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    char StreamClip;
    if(clipNumber=='0'){
         //stream the clip which is 1 minute before
        StreamClip='2';
    }
    else if(clipNumber=='1'){
        StreamClip='3';
    }else if(clipNumber=='2'){
        StreamClip='4';
    }else if(clipNumber=='3'){
        StreamClip='0';
    }else if(clipNumber=='4'){
        StreamClip='1';
    }   
    stream(StreamClip,st);
    playStream(StreamClip);
    QThread::connect(st,SIGNAL(finished()),this,SLOT(streamLastMinute()));    
    if(!boolstream){
        QThread::disconnect(st,SIGNAL(finished()),this,SLOT(streamLastMinute()));
        st->terminate();
    }
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
