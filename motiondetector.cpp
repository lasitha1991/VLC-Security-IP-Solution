#include "motiondetector.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>


MotionDetector::MotionDetector()
{
    filePath="_capture.mp4";
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
            cap >> frame;
            bg.operator ()(frame,fore);
            bg.getBackgroundImage(back);
            cv::erode(fore,fore,cv::Mat());
            cv::dilate(fore,fore,cv::Mat());
            cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
            //cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);

            //cv::imshow("Frame",frame);
            //cv::imshow("Background",back);
            //cv::imshow("ForeGround",fore);
            if(cv::waitKey(30) >= 0) break;
            if(contours.size()>senseLevel){
                printf("motion detected:: %d\n",contours.size());
                emit motionDetected();
            }
        }catch(cv::Exception e){
            cap.release();
            increaseClip();
            cap(filePath);
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
