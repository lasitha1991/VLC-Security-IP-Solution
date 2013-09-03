#ifndef FILESAVETHREAD_H
#define FILESAVETHREAD_H
#include <QtCore>
#include <vlc/vlc.h>

namespace Domain {
class FilesaveThread;
}

class FilesaveThread:public QThread
{
public:
    FilesaveThread();
    void run();
    void setInst(libvlc_instance_t *in,char *videoLocation,char *fileLocation);
    int exec();
    mutable QMutex mutex2;
};

#endif // FILESAVETHREAD_H
