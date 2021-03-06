//
//  Alien.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __opencv__Alien__
#define __opencv__Alien__

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// Metodos

Mat faceDetection(Mat frame);

// Este metodo
Vec3b obtenerMediana(Mat srcFrame);

Mat pintarColorMediana(Mat srcFrame,Vec3b mediana,int color,int rango);

void kmedias(Mat src,Mat original);

#endif /* defined(__opencv__Alien__) */
