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

void color_reduce(cv::Mat &image, int div) {
    int nl = image.rows;
    int nc = image.cols*image.channels();
    for (int j=0; j<nl; j++) {
        uchar* data = image.ptr<uchar>(j);
        for (int i=0; i<nc; i++) {
            data[i] = data[i]/div*div + div/2;
        }
    }
}

Mat metodoDibu(Mat srcFrame) {
    cv::Mat canny_output, output;    
    cv::Mat poster_output = srcFrame.clone();
    color_reduce(poster_output, 32);
    Canny(srcFrame, canny_output, 35, 90);
    for (int i=0; i<canny_output.rows; i++) {
        uchar* data= canny_output.ptr<uchar>(i);
        for (int a=0; a<canny_output.cols; a++) {
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