#include "broadcastthread.h"
#include <QtCore>
#include <vlc/vlc.h>



BroadcastThread::BroadcastThread()
{

}
BroadcastThread::~BroadcastThread(){

}

void BroadcastThread::run(){
    exec();
}

int BroadcastThread::exec(){
    mutex=new QMutex();
    QMutexLocker locker(mutex);
    libvlc_vlm_add_broadcast(instb, "video stream", filePathb, clientAddressb, 0, NULL, true, false);
    libvlc_vlm_play_media(instb, "video stream");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(instb, "video stream");
    libvlc_vlm_release(instb);
    locker.unlock();
    this->exit();
}

void BroadcastThread::setInst(libvlc_instance_t *in,char *fname,char *cAddr){
    instb=in;
    filePathb=fname;
    clientAddressb=cAddr;
}

