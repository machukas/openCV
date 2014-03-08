//
//  Poster.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#include "Poster.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*void reduce_color(cv::Mat &input, cv::Mat &output, size_t div) {
    if(input.data != output.data){
        output.create(input.size(), input.type());
    }
    
    uchar buffer[256];
    for(size_t i = 0; i != 256; ++i){
        buffer[i] = i / div * div + div / 2;
    }
    cv::Mat table(1, 256, CV_8U, buffer, sizeof(buffer));
    cv::LUT(input, table, output);
}*/

void reduce_color(cv::Mat &image, int div) {
    int nl = image.rows;
    int nc = image.cols*image.channels();
    for (int j=0; j<nl; j++) {
        uchar* data = image.ptr<uchar>(j);
        for (int i=0; i<nc; i++) {
            data[i] = data[i]/div*div + div/2;
        }
    }
}

Mat metodoPoster(Mat srcFrame) {
    cv::Mat output = srcFrame.clone();
    reduce_color(output, 32);
    return output;
}
