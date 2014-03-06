//
//  Alien.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#include "Alien.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat faceDetection(Mat frame){
    Mat faceOutline = Mat::zeros(frame.size(), CV_8UC3);
    Scalar color = CV_RGB(255, 255, 0);
    int thickness = 4;
    int sw = frame.size().width;
    int sh = frame.size().height;
    int faceH = sh/2 * 70/100;
    int faceW = faceH * 72/100;
    ellipse(faceOutline, Point(sw/2, sh/2), Size(faceW, faceH), 0, 0, 360, color, thickness, CV_AA);
    addWeighted(frame, 1.0, faceOutline, 0.7, 0, frame, CV_8UC3);
    return frame;
}

Mat metodoAlien(Mat frame) {
    Mat destination;
    faceDetection(frame);
    /* Reduce the number of pixels by a factor of four*/
    Size size = frame.size();
    Size smallSize;
    smallSize.width = size.width/4;
    smallSize.height = size.height/4;
    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(frame, smallImg, smallSize, 0,0, INTER_LINEAR);
    
    Mat yuv = Mat(smallSize, CV_8UC3);
    cvtColor(smallImg, yuv, CV_BGR2YCrCb);
    int sw = smallSize.width;
    int sh = smallSize.height;
    Mat mask, maskPlusBorder;
    maskPlusBorder = Mat::zeros(sh+2, sw+2, CV_8UC1);
    mask = maskPlusBorder(Rect(1,1,sw,sh));
    resize(frame, mask, smallSize);
    
    const int EDGES_THRESHOLD = 80;
    threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
    dilate(mask,mask,Mat());
    erode(mask,mask,Mat());
    
    int const NUM_SKIN_POINTS = 6;
    Point skinPts[NUM_SKIN_POINTS];
    skinPts[0] = Point(sw/2, sh/2-sh/6);
    skinPts[1] = Point(sw/2-sw/11, sh/2-sh/6);
    skinPts[2] = Point(sw/2+sw/11, sh/2-sh/6);
    skinPts[3] = Point(sw/2, sh/2-sh/16);
    skinPts[4] = Point(sw/2-sw/9, sh/2+sh/16);
    skinPts[5] = Point(sw/2+sw/9, sh/2-sh/16);
    
    const int LOWER_Y = 60;
    const int UPPER_Y = 80;
    const int LOWER_Cr = 25;
    const int UPPER_Cr = 15;
    const int LOWER_Cb = 20;
    const int UPPER_Cb = 15;
    Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
    Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);
    
    const int CONNECTED_COMPONENTS = 4;
    const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY;
    Mat edgeMask = mask.clone();
    for (int i=0; i<NUM_SKIN_POINTS; i++) {
        floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), NULL, lowerDiff, upperDiff, flags);
    }
    mask -= edgeMask;
    int Red = 0;
    int Green = 70;
    int Blue = 0;
    Scalar color = CV_RGB(Red, Green, Blue);
    cv::add(smallImg, color, smallImg, mask);
    
    Mat bigImg;
    resize(smallImg, bigImg, size, 0, 0, INTER_LINEAR);
    destination.setTo(0);
    bigImg.copyTo(destination,mask);
    
    //cvtColor(frame, destination, "CV_BGR2HSV_FULL");
    
    return destination;
}
