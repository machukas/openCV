//
//  Dibu.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#include "Dibu.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void color_reduce(cv::Mat &input, cv::Mat &output, size_t div)
{
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

Mat metodoDibu(Mat srcFrame) {
    int i,a;
    cv::Mat poster_output,canny_output, output;
    color_reduce(srcFrame, poster_output, 32);
    Canny(srcFrame, canny_output, 35, 90);
    for (i=0; i<canny_output.rows; i++) {
        uchar* data= canny_output.ptr<uchar>(i);
        for (a=0; a<canny_output.cols; a++) {
            if (data[a]==0) {
                data[a] = 255;
            } else {
                data[a] = 0;
            }
        }
    }
    poster_output.copyTo(output, canny_output);
    return output;
}