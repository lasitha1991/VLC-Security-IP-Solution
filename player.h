//Lasitha Weerasinghe
//100577A

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <mainwindow.h>
#include <QSlider>
#include <QLabel>
#include <vlc/vlc.h>
#include "streamthread.h"
#include "filesavethread.h"
#include "motiondetector.h"

namespace Domain {
class player;
}

class player : public QObject
{
    Q_OBJECT
public:
    explicit player(QObject *parent = 0);

signals:
    void statusUpdated();
    void clipNotAvailable();

public slots:
    void play();
    void playStream(char StreamClip);
    void pause();
    void stop();
    void load(char* fileName);
    void stream(char sClip);
    void setClientAddress(QString addr);
    char* giveClientAddress();
    void receiveStream();
    void loadWebCam();
    void saveWebcamToFile();
    void streamLastMinute();
    void streamLive();
    void increaseClipNumber();
    void setStreaming(bool val);
    void setRecording(bool val);
    void setWebcam(bool val);
    bool isRecording();
    bool isStreaming();
    bool isWebcamOn();
    char giveClipNumber();
    void setDisplayWidget(QWidget *dis);
    void releaseDisplay();

    void deleteTempFile();

    void startVideoProcess();
    void processMotionDetected();
    void processMotionNotDetected();
    bool MotionLastMin();
    bool isMotionClipAvailable();

    void setClipLength(QString length);
    void resetAll();
private:
    std::string clientAddress;
    bool boolrecord;
    bool boolstream;
    bool boolwebcamon;
    bool motionClipAvailable;
    char clipNumber;
    int clipLength;
    char maxNoOfClips;
    int timeSinceLastMotion;
    int maxNoMotionDuration;
    libvlc_instance_t *inst;
    libvlc_instance_t *streamInst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    QWidget *displayWid;
    MotionDetector *mdetect;
    StreamThread *sThread;
    FilesaveThread *fThread;

};

#endif // PLAYER_H
