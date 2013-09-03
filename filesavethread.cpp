#include "filesavethread.h"
#include <QtCore>
#include <vlc/vlc.h>

libvlc_instance_t *inst3;
char *filePath3;
char *clientAddress3;
FilesaveThread::FilesaveThread()
{
}


void FilesaveThread::run(){
    exec();
}

int FilesaveThread::exec(){
    QMutexLocker locker(&mutex2);
    libvlc_vlm_add_broadcast(inst3, "video stream", filePath3, clientAddress3, 0, NULL, true, false);
    libvlc_vlm_play_media(inst3, "video stream");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst3, "video stream");
    libvlc_vlm_release(inst3);
    locker.unlock();
    //this->exit();
}

void FilesaveThread::setInst(libvlc_instance_t *in,char *videoLocation,char *fileLocation){
    inst3=in;
    filePath3=videoLocation;
    clientAddress3=fileLocation;
}

