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
    cvtColor(image, image, CV_BGR2HSV);
    int nl = image.rows;
    int nc = image.cols;
    vector<Mat> HSVChannels;
    split(image, HSVChannels);
    for (int j=0; j<nl; j++) {
        uchar* data = HSVChannels[2].ptr<uchar>(j);
        for (int i=0; i<nc; i++) {
            data[i] = data[i]/div*div;
        }
    }
    merge(HSVChannels, image);
    cvtColor(image, image, CV_HSV2BGR);
}

Mat metodoPoster(Mat srcFrame, int div) {
    if (div==0) { div=1; }
    cv::Mat output = srcFrame.clone();
    reduce_color(output, div);
    return output;
}
