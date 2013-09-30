//Lasitha Weerasinghe
//100577A

#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H
#include <QtCore>

namespace Domain {
class MotionDetector;
}

class MotionDetector :public QThread
{
    Q_OBJECT
public:
    MotionDetector();
    void run();
    int exec();
    void setClip(char clip);
    void increaseClip();
    void setSenseLevel(int lev);
private:
    std::string filePath;
    char clipNo;
    int senseLevel;
signals:
    void motionDetected();
};

#endif // MOTIONDETECTOR_H