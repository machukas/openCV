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
    //cvtColor(output, output, CV_RGB2HLS); //CV_XYZ2RGB CV_RGB2HSV CV_RGB2HLS
    reduce_color(output, 32);
    //cvtColor(output, output, CV_XYZ2RGB);
    return output;
}
