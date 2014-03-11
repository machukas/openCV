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

/**
 * Metodo que reduce el numero de colores presentes en una imagen [image] a 
 * partir de un factor de reducción [div] que opera entre 1 y 255.
 */
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

/**
 * Devuelve una imagen identica a [srcFrame] pero con una reduccion del 
 * numero de colores de la imagen que viene dada por el factor de reduccion
 * [div].
 */
Mat metodoPoster(Mat srcFrame, int div) {
    if (div==0) { div=1; }
    cv::Mat output = srcFrame.clone();
    reduce_color(output, div);
    return output;
}
