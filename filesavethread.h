#ifndef FILESAVETHREAD_H
#define FILESAVETHREAD_H
#include <QtCore>
#include <vlc/vlc.h>

namespace Domain {
class fileSaveThread;
}

class fileSaveThread:public QThread
{
public:
    fileSaveThread();
    void run();
    void setInst(libvlc_instance_t *in,char *camPath,char *FileLocation);
};

#endif // FILESAVETHREAD_H
