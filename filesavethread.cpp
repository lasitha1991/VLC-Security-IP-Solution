#include "filesavethread.h"
#include <QtCore>
#include <vlc/vlc.h>

libvlc_instance_t *inst3;
char *camPath2;
char *fileLoc2;

fileSaveThread::fileSaveThread()
{
}
void fileSaveThread::run(){
    libvlc_vlm_add_broadcast(inst3, "File Save", camPath2, fileLoc2, 0, NULL, true, false);
    libvlc_vlm_play_media(inst3, "File Save");

    //play(bu);
    sleep(20); /* Let it play for sometime */
    //stop();
    libvlc_vlm_stop_media(inst3, "File Save");
    libvlc_vlm_release(inst3);
}

void fileSaveThread::setInst(libvlc_instance_t *in,char *camPath,char *fileLoc){
    inst3=in;
    camPath2=camPath;
    fileLoc2=fileLoc;
}
