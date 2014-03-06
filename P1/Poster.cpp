//
//  Poster.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#include "Poster.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void reduce_color(cv::Mat &input, cv::Mat &output, size_t div) {
    if(input.data != output.data){
        output.create(input.size(), input.type());
    }
    
    uchar buffer[256];
    for(size_t i = 0; i != 256; ++i){
        buffer[i] = i / div * div + div / 2;
    }
    cv::Mat table(1, 256, CV_8U, buffer, sizeof(buffer));
    cv::LUT(input, table, output);
}

Mat metodoPoster(Mat srcFrame) {
    cv::Mat output;
    reduce_color(srcFrame, output, 32);
    return output;
}
