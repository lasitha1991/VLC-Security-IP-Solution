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
    void playStream();
    void pause();
    void stop();
    void load(char* fileName);
    void stream(char sClip,StreamThread *st);
    void setClientAddress(QString addr);
    char* giveClientAddress();
    void receiveStream(QPushButton *bu);
    void loadStream(char *txt);
    void loadWebCam();
    void saveWebcamToFile();
    void streamLastMinute();
    void streamLive();
    void increaseClipNumber();
    void setStreaming(bool val);
    void setRecording(bool val);
    char giveClipNumber();
    void setDisplayWidget(QWidget *dis);
private:
    std::string clientAddress;
    bool boolrecord;
    bool boolstream;
    char clipNumber;
    libvlc_instance_t *inst;
    libvlc_instance_t *streamInst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
    QWidget *displayWid;
};

#endif // PLAYER_H
