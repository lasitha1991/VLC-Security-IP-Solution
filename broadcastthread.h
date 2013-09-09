#ifndef BROADCASTTHREAD_H
#define BROADCASTTHREAD_H
#include <QtCore>
#include <vlc/vlc.h>

namespace Domain {
class BroadcastThread;
}

class BroadcastThread: public QThread
{
public:
    BroadcastThread();
    ~BroadcastThread();
    void run();
    void setInst(libvlc_instance_t *in,char *sourceAddr,char *destAddr);
    int exec();
private:
    mutable QMutex *mutex;
    libvlc_instance_t *instb;
    char *filePathb;
    char *clientAddressb;    
    bool stateChanged;
};

#endif // BROADCASTTHREAD_H
