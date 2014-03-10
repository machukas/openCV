//
//  Dibu.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#include "Dibu.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
 * Metodo que reduce el numero de colores presentes en una imagen [image] a
 * partir de un factor de reducción [div] que opera entre 1 y 255.
 */
void color_reduce(cv::Mat &image, int div) {
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
 * [div]. Además, a la imagen con reduccion de colores se le añaden los 
 * contornos marcados, dando lugar a un efecto cartoon.
 */
Mat metodoDibu(Mat srcFrame, int div) {
    if (div==0) { div=1; }
    cv::Mat canny_output, output;    
    cv::Mat poster_output = srcFrame.clone();
    color_reduce(poster_output, div);
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