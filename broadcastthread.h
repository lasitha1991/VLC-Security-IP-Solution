#ifndef BROADCASTTHREAD_H
#define BROADCASTTHREAD_H
#include <QtCore>
#include <vlc/vlc.h>
#include <queue>

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

    mutable QMutex *mutex;
    libvlc_instance_t *instb;

public:
    std::string filePathb;
    std::string clientAddressb;


};

#endif // BROADCASTTHREAD_H
