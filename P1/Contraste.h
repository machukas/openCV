//
//  Contraste.h
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#ifndef __opencv__Contraste__
#define __opencv__Contraste__

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// Metodos

Mat metodoContraste(Mat frame,double alpha,int beta);

Mat HistogramaRGB(Mat frame);

Mat HistogramaBN(Mat frame);

Mat ecualizarHistograma(Mat frame);

#endif /* defined(__opencv__Contraste__) */
