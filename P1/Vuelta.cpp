//
//  Vuelta.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include "Vuelta.h"

using namespace cv;
using namespace std;

/**
 * Devuelve una imagen identica a [srcFrame] pero girada 180 grados.
 */
Mat metodoVuelta(Mat srcFrame) {
    cv::Mat dst;
    cv::flip(srcFrame,dst,0);
    return dst;
}