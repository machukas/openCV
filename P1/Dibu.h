//
//  Dibu.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#ifndef __opencv__Dibu__
#define __opencv__Dibu__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

//Metodos

Mat metodoDibu(Mat frame, int div);

void color_reduce(cv::Mat &input, int div);

#endif /* defined(__opencv__Dibu__) */
