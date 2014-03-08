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

Mat faceDetection(Mat frame);

Mat pruebaMediana(Mat src,Mat completa);

void kmedias(Mat src,Mat original);

#endif /* defined(__opencv__Alien__) */
