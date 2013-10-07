//Lasitha Weerasinghe
//100577A

#include "streamthread.h"
#include <QtCore>
#include <vlc/vlc.h>
#include <fstream>

StreamThread::StreamThread()
{
}

void StreamThread::setClip(char clip){
    if(clip!='-'){              //- is for live streaming
        std::string fileName="_capture.mp4";  //make a local clip name
        fileName[0]=clip;
        setFilePath((char*)fileName.c_str());

        std::ifstream ifs;
        ifs.open (fileName.c_str(), std::ifstream::in);
        clipAvailable= ifs.is_open();
        ifs.close();

    }else{
        setFilePath("v4l2:///dev/video0");
        setClientAddr("#transcode{vcodec=mp4v,vb=0,scale=0}:udp{dst=127.0.0.1}");  //this has to be corrected.. currently only goes to localhost
    }
}

void StreamThread::setInst(libvlc_instance_t *in,char clip,char *cAddr){
    setVLCInst(in);
    setClientAddr(cAddr);
    setClip(clip);
}
bool StreamThread::isClipAvailable(){
    return clipAvailable;
}

