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
private:
    std::string filePath;
    char clipNo;
signals:
    void motionDetected();
    void motionNotDetected();
};

#endif // MOTIONDETECTOR_H
