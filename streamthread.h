#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H
#include <QtCore>
#include <vlc/vlc.h>
#include "broadcastthread.h"
namespace Domain {
class StreamThread;
}

class StreamThread : public BroadcastThread
{
public:
    StreamThread();
    void setInst(libvlc_instance_t *in,char clip,char *cAddr);
    void setClip(char clip);
    void setMode(int mode);
    int exec();
    char clip;
    int mode;

};

#endif // STREAMTHREAD_H
