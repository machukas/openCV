//
//  Poster.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#ifndef __opencv__Poster__
#define __opencv__Poster__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

//Metodos

Mat metodoPoster(Mat src);

void reduce_color(cv::Mat &input, cv::Mat &output, size_t div);

#endif /* defined(__opencv__Poster__) */
