#include "streamthread.h"
#include <QtCore>
#include <vlc/vlc.h>

libvlc_instance_t *inst2;
char *filePath2;
char *clientAddress2;
StreamThread::StreamThread()
{
}

void StreamThread::run(){
    libvlc_vlm_add_broadcast(inst2, "video stream", filePath2, clientAddress2, 0, NULL, true, false);
    libvlc_vlm_play_media(inst2, "video stream");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst2, "video stream");
    libvlc_vlm_release(inst2);
}

void StreamThread::setInst(libvlc_instance_t *in,char *fname,char *cAddr){
    inst2=in;
    filePath2=fname;
    clientAddress2=cAddr;
}

