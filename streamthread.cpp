#include "streamthread.h"
#include <QtCore>
#include <vlc/vlc.h>

StreamThread::StreamThread()
{
}

void StreamThread::setClip(char clip){
    char *fileName="capture.mp4";
    this->clip=clip;
    char file[12];
    file[0]=clip;
    for(int i=1;i<12;i++){
        file[i]=fileName[i-1];
    }
    filePathb=file;
}
void StreamThread::setMode(int mode){
    this->Smode=mode;
}
void StreamThread::setInst(libvlc_instance_t *in,char clip,char *cAddr){
    instb=in;
    clientAddressb=cAddr;
    setClip(clip);
}

int StreamThread::exec(){    
    mutex=new QMutex();
    QMutexLocker locker(mutex);

    if(this->Smode==0){
        libvlc_vlm_add_broadcast(instb, "video stream", filePathb.c_str(), clientAddressb.c_str(), 0,NULL, true, false);
        libvlc_vlm_play_media(instb, "video stream");

        sleep(20); /* Let it play for sometime */

        libvlc_vlm_stop_media(instb, "video stream");
        libvlc_vlm_release(instb);
    }
    else if(this->Smode==1){
        libvlc_vlm_add_broadcast(instb, "video stream", filePathb.c_str(), clientAddressb.c_str(), 0,NULL, true, false);
        libvlc_vlm_play_media(instb, "video stream");

        sleep(20); /* Let it play for sometime */
        libvlc_vlm_stop_media(instb, "video stream");
        libvlc_vlm_release(instb);


        if(this->clip!='4'){
            this->clip++;
        }else{
            this->clip='0';
        }
        setClip(this->clip);

        libvlc_vlm_add_broadcast(instb, "video stream", filePathb.c_str(), clientAddressb.c_str(), 0,NULL, true, false);
        libvlc_vlm_play_media(instb, "video stream");

        sleep(20); /* Let it play for sometime */

        libvlc_vlm_stop_media(instb, "video stream");
        libvlc_vlm_release(instb);


        if(this->clip!='4'){
            this->clip++;
        }else{
            this->clip='0';
        }
        setClip(this->clip);


        libvlc_vlm_add_broadcast(instb, "video stream", filePathb.c_str(), clientAddressb.c_str(), 0,NULL, true, false);
        libvlc_vlm_play_media(instb, "video stream");

        sleep(20); /* Let it play for sometime */
        libvlc_vlm_stop_media(instb, "video stream");
        libvlc_vlm_release(instb);
    }
    locker.unlock();
    this->exit();
}

void StreamThread::run(){
    exec();
}
