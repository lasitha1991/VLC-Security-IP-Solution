#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <mainwindow.h>
#include <QSlider>
#include <QLabel>
#include <vlc/vlc.h>

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
    void play(QPushButton *bu);
    void pause();
    void stop();
    void load(QWidget *dis);
    void stream(char* file);
    void setClientAddress(QString addr);
    void receiveStream(QWidget *dis,QPushButton *bu);
    void loadStream(QWidget *dis,char *txt);
    void loadWebCam(QWidget *dis,QPushButton *bu);
    void saveWebcamToFile();
    void recordOneMin();
    void streamLastMinute();
    void streamCaptureClip(char clip);

};

#endif // PLAYER_H
