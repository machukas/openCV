//
//  espejo.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 07/03/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include "espejo.h"

using namespace cv;
using namespace std;

Mat metodoEspejo(Mat srcFrame) {
    cv::Mat dst;
    cv::flip(srcFrame,dst,0);
    return dst;
}