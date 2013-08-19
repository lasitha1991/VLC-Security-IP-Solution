#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <string.h>

libvlc_instance_t *inst;
libvlc_media_player_t *mp;
libvlc_media_t *m;
//char *filePath = "../Thank_you.flv";
char *filePath = "../Bolt.avi";
char fileName[12]="capture.avi";
char *clientAddress="udp://10.8.98.1";
char *serverAddress="udp://10.8.98.2";
char clipNumber='0';


player::player(QObject *parent) :    QObject(parent)
{
    inst = libvlc_new(0, NULL);
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


int player::getTime()
{
    return libvlc_media_player_get_time(mp);
}

int player::getLength()
{
    return libvlc_media_player_get_length(mp);
}


bool player::isPLay(){
    if(mp==NULL){
        return false;
    }else{
        return libvlc_media_player_is_playing(mp);
    }
}

void player::pause(){
    //    libvlc_media_player_set_pause(mp,pause());
    //  pause();
    if(libvlc_media_player_can_pause(mp)){
        libvlc_media_player_pause(mp);
    }else{
        exit(EXIT_FAILURE);
    }
}

int player::getPosition()
{
    float pos = libvlc_media_player_get_position(mp);
    return (int)(pos*1000.0);
}

void player::changePosition(int pos) { /* Called on position slider change */

    if (mp)
        libvlc_media_player_set_position(mp, (float)pos/1000.0);
}

void player::changeVolume(int val)
{
    if (mp)
        libvlc_audio_set_volume (mp,val);
}

void player::mute(QSlider *sli) {
    if(mp) {
        if(sli->value() == 0) { //if already muted...

            this->changeVolume(80);
            sli->setValue(80);

        } else { //else mute volume

            this->changeVolume(0);
            sli->setValue(0);

        }
    }
}

void player::stream(QPushButton *bu){ //must be a unicast stream
    bu->setText("Started");
    //libvlc_vlm_add_broadcast(inst, "Thank_you.flv", filePath, "#transcode{acodec=mp4a,ab=128,channels=2," \
    //                             "samplerate=44100}:rtsp{dst=:8090/go.mp3}", 0, NULL, true, false);
    libvlc_vlm_add_broadcast(inst, "video stream", filePath, clientAddress, 0, NULL, true, false);
    libvlc_vlm_play_media(inst, "video stream");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst, "video stream");
    libvlc_vlm_release(inst);    
    bu->setText(clientAddress);
}
void player::setClientAddress(QString addr){    
    //check with unit testing

    //QByteArray ba=addr.toLocal8Bit();
    //ba.append(addr);
    //char client[addr.count()+1];
    //for(int i=0;i<addr.count();i++){
    //    client[i]=addr.at(i);
    //}
    //client[addr.count()]="\0";
    //clientAddress=client;
}

void player::receiveStream(QWidget *dis,QPushButton *bu){
    //code to recieve the stream
	//stream must be unicast
    bu->setText("Receiving Started");
    //libvlc_vlm_add_input 	( 	inst, "InputStream", "udp://10.8.98.1:1234");
    loadStream(dis,serverAddress);
    //libvlc_vlm_play_media(inst, "InputStream");
    play(bu);
    sleep(25); /* Let it play for sometime */
    stop();
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
void player::saveWebcamToFile(QPushButton *bu){
    bu->setText("Started");
    char file[12];
    file[0]=clipNumber;
    for(int i=1;i<12;i++){
        file[i]=fileName[i-1];
    }
    libvlc_vlm_add_broadcast(inst, "videosave", "v4l2:///dev/video0", file, 0, NULL, true, false);
    libvlc_vlm_play_media(inst, "videosave");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst, "videosave");
    libvlc_vlm_release(inst);
    bu->setText("Saved "+clipNumber);
    if(clipNumber!='3'){
        clipNumber++;
    }else{
        clipNumber='0';
    }
}
void player::streamLastMinute(){
    if(clipNumber=='0'){
        streamCaptureClip('3');
        streamCaptureClip('2');
        streamCaptureClip('1');
    }
    else if(clipNumber=='1'){
        streamCaptureClip('0');
        streamCaptureClip('3');
        streamCaptureClip('2');
    }else if(clipNumber=='2'){
        streamCaptureClip('1');
        streamCaptureClip('0');
        streamCaptureClip('3');
    }else if(clipNumber=='3'){
        streamCaptureClip('2');
        streamCaptureClip('1');
        streamCaptureClip('0');
    }
}
void player::streamCaptureClip(char clip){ //must be a unicast stream

    char file[12];
    file[0]=clip;
    for(int i=1;i<12;i++){
        file[i]=fileName[i-1];
    }
    libvlc_vlm_add_broadcast(inst, "video stream", file, clientAddress, 0, NULL, true, false);
    libvlc_vlm_play_media(inst, "video stream");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst, "video stream");
    libvlc_vlm_release(inst);

}



libvlc_media_player_t* player::getMP(){
    return mp;
}


