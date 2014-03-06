//
//  Distorsion.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 06/03/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include "Distorsion.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat metodoDistorsion(Mat srcFrame) {
    
    Mat dst;
    Mat map_x, map_y;
    int x,y,k1;
    k1 = 0;
    
    double Cx = (double)srcFrame.cols/2;
    double Cy = (double)srcFrame.rows/2;
    
    map_x.create(srcFrame.size(), CV_32FC1);
    map_y.create(srcFrame.size(), CV_32FC1);
    
    for (x=0; x<map_x.rows; x++) {
        double ty = x-Cy;
        for (y=0; y<map_x.cols; y++) {
            double tx=(double)y-Cx;
            double rt = tx*tx + ty*ty;
            
            k1=-10;
            map_x.at<float>(x,y) = (double) (tx/(1 + double(k1/1000000.0)*rt)+Cx);
            map_y.at<float>(x,y) = (double) (ty/(1 + double(k1/1000000.0)*rt)+Cy);
        }
    }
    remap(srcFrame, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT);
    
    return dst;
}
