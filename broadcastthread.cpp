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


    sleep(clipLength); /* Let it play for cliplength time  */


    libvlc_vlm_stop_media(instb, "video stream");
    libvlc_vlm_release(instb);
    locker.unlock();
    this->exit();
}
void BroadcastThread::setInst(libvlc_instance_t *in,char *fname,char *cAddr){
    setVLCInst(in);
    setFilePath(fname);
    setClientAddr(cAddr);
}
void BroadcastThread::setVLCInst(libvlc_instance_t *ins){
    instb=ins;  //set vlc player instance in thread
}
void BroadcastThread::setFilePath(char *sAddr){
    filePathb=sAddr;  //set the source address to broadcast in thread
}
void BroadcastThread::setClientAddr(char *cAddr){
    clientAddressb=cAddr;   //set the destination address in thread
}
