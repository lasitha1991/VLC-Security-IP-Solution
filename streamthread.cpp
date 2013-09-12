#include "streamthread.h"
#include <QtCore>
#include <vlc/vlc.h>

StreamThread::StreamThread()
{
}

void StreamThread::setClip(char clip){
    if(clip!='9'){
        char *fileName="capture.mp4";
        this->clip=clip;
        char file[12];
        file[0]=clip;
        for(int i=1;i<12;i++){
            file[i]=fileName[i-1];
        }
        setFilePath(file);
    }else{
        setFilePath("v4l2:///dev/video0");
        //setClientAddr("#transcode{vcodec=mp4v,vb=0,scale=0}:std{access=udp{ttl=1},dst=127.0.0.1:1234}");
        setClientAddr("#transcode{vcodec=mp4v,vb=0,scale=0}:udp{dst=127.0.0.1}");
    }
}

void StreamThread::setInst(libvlc_instance_t *in,char clip,char *cAddr){
    setVLCInst(in);
    setClientAddr(cAddr);
    setClip(clip);
}


