//Lasitha Weerasinghe
//100577A

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
    bool isClipAvailable();
private:
    void setClip(char clip);
    bool clipAvailable;
};

#endif // STREAMTHREAD_H
