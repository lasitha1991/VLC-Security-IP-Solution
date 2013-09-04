#ifndef FILESAVETHREAD_H
#define FILESAVETHREAD_H
#include <QtCore>
#include <vlc/vlc.h>
#include "broadcastthread.h"
namespace Domain {
class FilesaveThread;
}

class FilesaveThread:public BroadcastThread
{
public:
    FilesaveThread();

};

#endif // FILESAVETHREAD_H
