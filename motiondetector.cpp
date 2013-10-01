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
//    cv::Mat frame;
//    cv::Mat back;
//    cv::Mat fore;

//    cv::VideoCapture cap(filePath);
//    //cv::VideoCapture cap(0);
//    cv::BackgroundSubtractorMOG2 bg;
//    bg.nmixtures = 1;
//    bg.bShadowDetection = false;

//    std::vector<std::vector<cv::Point> > contours;
//    for(;;)
//    {
//        try{
//            //qDebug("loop");
//            cap >> frame;
//            bg.operator ()(frame,fore);
//            bg.getBackgroundImage(back);
//            cv::erode(fore,fore,cv::Mat());
//            cv::dilate(fore,fore,cv::Mat());
//            cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
//            cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);

//            cv::imshow("Frame",frame);
//            //cv::imshow("Background",back);
//            //cv::imshow("ForeGround",fore);
////            if(cv::waitKey(3) >= 0){
////                break;
////            }
//            //qDebug("draw");
//            if(contours.size()>senseLevel){
//                emit motionDetected();
//            }
//        }catch(cv::Exception e){
//            cap.release();
//            increaseClip();
//            cap.open(filePath);
//            qDebug(filePath.c_str());
//        }
//    }
    qDebug("start");
    CvCapture * camera = //cvCaptureFromFile(filePath.c_str());
            cvCaptureFromCAM(CV_CAP_ANY);
    cv::Mat original =cvQueryFrame(camera);
    cv::Mat next_frame = original;
    cv::Mat current_frame = cvQueryFrame(camera);
    cv::Mat prev_frame = cvQueryFrame(camera);
    cvtColor(current_frame, current_frame, CV_RGB2GRAY);
    cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);
    cvtColor(next_frame, next_frame, CV_RGB2GRAY);
    cv::Mat d1, d2, result;
    int window = 200;
    bool movement;
    while (true){        
        movement = false;
        absdiff(next_frame, current_frame, d1);
        absdiff(current_frame, prev_frame, d2);
        bitwise_xor(d1, d2, result);
        int middle_y = result.rows/2;
        int middle_x = result.cols/2;
        // Center window
        threshold(result, result, 140, 255, CV_THRESH_BINARY);
        for(int i = middle_x-window; i < middle_x+window; i++)
            for(int j = middle_y-window; j < middle_y+window; j++)
                if(result.at<int>(j,i)>0)
                {
                    movement = true;
                    break;
                }
        if(movement==true){
            qDebug("motion detected");
            emit motionDetected();
        }
        //imshow("Motion", result);
        prev_frame = current_frame;
        current_frame = next_frame;
        // get image from file
        next_frame = cvQueryFrame(camera);
        cvtColor(next_frame, next_frame, CV_RGB2GRAY);
        qDebug("loop");
    }

    qDebug("method end");
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
