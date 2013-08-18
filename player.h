#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <mainwindow.h>
#include <QSlider>
#include <QLabel>
#include <vlc/vlc.h>

namespace Ui {
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
    int getTime();
    int getLength();
    bool isPLay();
    void load(QWidget *dis);
    int getPosition();
    void changePosition(int pos);
    void changeVolume(int val);
    void mute(QSlider *sli);
    void stream(QPushButton *bu,QString txt);
    void receiveStream();
    libvlc_media_player_t *getMP();
};

#endif // PLAYER_H
