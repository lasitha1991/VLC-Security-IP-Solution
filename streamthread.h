#ifndef STREAMTHREAD_H
#define STREAMTHREAD_H
#include <QtCore>
#include <vlc/vlc.h>
class StreamThread :public QThread
{
public:
    StreamThread();
    void run();
    void setInst(libvlc_instance_t *in);
    void setFileName(char *fname);
    void setclientAddress(char *cAddr);
};

#endif // STREAMTHREAD_H
