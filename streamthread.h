#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H
#include <QtCore>
#include <vlc/vlc.h>

namespace Domain {
class StreamThread;
}

class StreamThread : public QThread
{
public:
    StreamThread();
    void run();
    void setInst(libvlc_instance_t *in,char *fname,char *cAddr);
    int exec();
    mutable QMutex mutex;
};

#endif // STREAMTHREAD_H
