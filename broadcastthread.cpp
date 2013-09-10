#include "broadcastthread.h"
#include <QtCore>
#include <vlc/vlc.h>
#include <queue>


BroadcastThread::BroadcastThread()
{
    clipLength=20;
}
BroadcastThread::~BroadcastThread(){

}

void BroadcastThread::run(){    
    exec();

}

int BroadcastThread::exec(){

    mutex=new QMutex();
    QMutexLocker locker(mutex);


    libvlc_vlm_add_broadcast(instb, "video stream", filePathb.c_str(), clientAddressb.c_str(), 0,NULL, true, false);
    libvlc_vlm_play_media(instb, "video stream");

    sleep(clipLength); /* Let it play for sometime */

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
void BroadcastThread::setVLCInst(libvlc_instance_t *ins){
    instb=ins;
}
void BroadcastThread::setFilePath(char *sAddr){
    filePathb=sAddr;
}
void BroadcastThread::setClientAddr(char *cAddr){
    clientAddressb=cAddr;
}
