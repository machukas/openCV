//
//  Poster.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __opencv__Poster__
#define __opencv__Poster__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

//Metodos

Mat metodoPoster(Mat src, int div);

void reduce_color(cv::Mat &input, int div);

#endif /* defined(__opencv__Poster__) */
