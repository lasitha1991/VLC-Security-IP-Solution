//Lasitha Weerasinghe
//100577A


#include "motiondetector.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>


MotionDetector::MotionDetector()
{
    clipNo='4';
    filePath="4capture.mp4";
    senseLevel=500;
}


void MotionDetector::run(){    
    exec();
}

int MotionDetector::exec(){
    cv::Mat frame;
    cv::Mat back;
    cv::Mat fore;

    cv::VideoCapture cap(filePath);
    //cv::VideoCapture cap(0);
    cv::BackgroundSubtractorMOG2 bg;
    bg.nmixtures = 1;
    bg.bShadowDetection = false;

    std::vector<std::vector<cv::Point> > contours;
    for(;;)
    {
        try{
            //qDebug("loop");
            cap >> frame;
            bg.operator ()(frame,fore);
            bg.getBackgroundImage(back);
            cv::erode(fore,fore,cv::Mat());
            cv::dilate(fore,fore,cv::Mat());
            cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
            cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);

            cv::imshow("Frame",frame);
            //cv::imshow("Background",back);
            //cv::imshow("ForeGround",fore);
//            if(cv::waitKey(3) >= 0){
//                break;
//            }
            //qDebug("draw");
            if(contours.size()>senseLevel){                
                emit motionDetected();
            }
        }catch(cv::Exception e){            
            cap.release();
            increaseClip();
            cap.open(filePath);
            qDebug(filePath.c_str());
        }
    }
}

void MotionDetector::setClip(char clip){
    clipNo=clip;
    filePath[0]=clipNo;
}
void MotionDetector::increaseClip(){
    if(clipNo!='4'){
        clipNo++;
    }else{              //if clipnumber is 4 set it to 0
        clipNo='0';
    }
    filePath[0]=clipNo;
}
void MotionDetector::setSenseLevel(int lev){
    senseLevel=100+lev*10;
}
