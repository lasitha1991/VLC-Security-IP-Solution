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

public slots:
    void play();
    void playStream(char StreamClip);
    void pause();
    void stop();
    void load(char* fileName);
    void stream(char sClip,StreamThread *st);
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
private:
    std::string clientAddress;
    bool boolrecord;
    bool boolstream;
    bool boolwebcamon;
    char clipNumber;
    libvlc_instance_t *inst;
    libvlc_instance_t *streamInst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    QWidget *displayWid;
    MotionDetector *mdetect;
};

#endif // PLAYER_H
