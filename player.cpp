#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include "ui_mainwindow.h"
#include "streamthread.h"
#include "filesavethread.h"


libvlc_instance_t *inst;
libvlc_instance_t *streamInst;
libvlc_media_player_t *mp;
libvlc_media_t *m;
//char *filePath = "../Thank_you.flv";
char *filePath = "../Bolt.avi";
char soutraw[60]="#transcode{vcodec=mp4v,vb=0,scale=0}:file{dst=_capture.mp4}";
char *fileName="capture.mp4";
//char *clientAddress="udp://127.0.0.1";
char *serverAddress="udp://127.0.0.1:1234";
char clipNumber='0';



player::player(QObject *parent) :    QObject(parent)
{
    inst = libvlc_new(0, NULL);
    streamInst=libvlc_new(0,NULL);    
}

void player::play(QPushButton *bu)
{
    if (!mp)
        return;

    if (libvlc_media_player_is_playing(mp))
    {
        /* Pause */
        libvlc_media_player_pause(mp);
        bu->setText("Play");
    }
    else
    {
        /* Play again */
        libvlc_media_player_play(mp);
        bu->setText("Pause");
    }
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

void player::load(QWidget *dis){
    // create a new item
    m = libvlc_media_new_path(inst, filePath);
    if(!m)
        exit(EXIT_FAILURE);
    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);
    // play the media_player

#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(mp, (void *)dis->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mp,dis->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, dis->winId());
#endif
}
void player::loadStream(QWidget *dis,char *streamName){
    // create a new item
    m = libvlc_media_new_location(inst, streamName);
    if(!m)
        exit(EXIT_FAILURE);
    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);
    // play the media_player

#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(mp, (void *)dis->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mp,dis->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, dis->winId());
#endif
}


void player::pause(){    
    if(libvlc_media_player_can_pause(mp)){
        libvlc_media_player_pause(mp);
    }else{
        exit(EXIT_FAILURE);
    }
}



void player::receiveStream(QWidget *dis,QPushButton *bu){
    //code to recieve the stream
	//stream must be unicast
    bu->setText("Receiving Started");
    //libvlc_vlm_add_input 	( 	inst, "InputStream", "udp://10.8.98.1:1234");
    loadStream(dis,serverAddress);
    //libvlc_vlm_play_media(inst, "InputStream");
    //play(bu);
    //sleep(25); /* Let it play for sometime */
    //stop();
    //libvlc_vlm_stop_media(inst, "InputStream");
    //libvlc_vlm_release(inst);
    bu->setText("Receiving ended");

}
void player::loadWebCam(QWidget *dis,QPushButton *bu){
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
    libvlc_media_player_set_nsobject(mp, (void *)dis->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(mp,dis->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(mp, dis->winId());
#endif
    bu->setText("WebCamLoaded");
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
    st->setMode(1);
    st->start();  //streaming starts
    sleep(1);
}

void player::saveWebcamToFile(){    
    char sout[60];
    sout[46]=clipNumber;
    for(int i=0;i<60;i++){
        if(i!=46){
            sout[i]=soutraw[i];
        }
    }    
    FilesaveThread *ft=new FilesaveThread();    
    ft->setInst(inst,"v4l2:///dev/video0",sout);
    //connect(ft,SIGNAL(finished()),ft,SLOT(deleteLater()));    
    ft->start();

    sleep(1);
    if(clipNumber!='4'){
        clipNumber++;
    }else{
        clipNumber='0';
    }
}
void player::recordOneMin(){
    saveWebcamToFile();
    saveWebcamToFile();
    saveWebcamToFile();
}

void player::streamLastMinute(){  //if player is at one instance stream last three instances    
    if(clipNumber=='0'){
        streamCaptureClip('2');
        streamCaptureClip('3');
        streamCaptureClip('4');
    }
    else if(clipNumber=='1'){
        streamCaptureClip('3');
        streamCaptureClip('4');
        streamCaptureClip('0');
    }else if(clipNumber=='2'){
        streamCaptureClip('4');
        streamCaptureClip('0');
        streamCaptureClip('1');
    }else if(clipNumber=='3'){
        streamCaptureClip('0');
        streamCaptureClip('1');
        streamCaptureClip('2');
    }else if(clipNumber=='4'){
        streamCaptureClip('1');
        streamCaptureClip('2');
        streamCaptureClip('3');
    }
}

void player::streamCaptureClip(char clip){ //starts a unicast stream of a single 20 sec clip    
    StreamThread *st=new StreamThread(); //creating an object instance prevents destroying thread while running
    stream(clip,st);
}


