//
//  Sepia.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 07/03/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include "Sepia.h"

using namespace cv;
using namespace std;

Mat metodoSepia(Mat srcFrame) {
    cv::Mat copy = srcFrame.clone();
    cv::Mat image_copy = srcFrame.clone();
    
    cv::Mat kern = (cv::Mat_<float>(4,4) <<  0.272, 0.534, 0.131, 0,
                    0.349, 0.686, 0.168, 0,
                    0.393, 0.769, 0.189, 0,
                    0, 0, 0, 1);
    
    cv::transform(image_copy, copy, kern);
    return copy;
}
