//
//  Alien.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#ifndef __opencv__Alien__
#define __opencv__Alien__

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// Metodos

Mat getSkin(cv::Mat input);

Mat faceDetection(Mat frame);

Mat metodoAlien(Mat frame);

Mat pruebaAlien(Mat src);

void kmedias(Mat src);

#endif /* defined(__opencv__Alien__) */
