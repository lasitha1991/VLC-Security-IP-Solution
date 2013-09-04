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
    int exec();
};

#endif // STREAMTHREAD_H
