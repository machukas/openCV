//
//  Distorsion.cpp
//  Vision por computador
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#include "Distorsion.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
 * Devuelve una imagen identica a [srcFrame] que dispone de una distorsion de 
 * barril o cojin en funcion del parametro [k1]. En caso de que k1 sea negativo
 * generara el efecto barril y en caso contrario el efecto cojin.
 */
Mat metodoDistorsion(Mat srcFrame, int k1) {
    
    Mat map_x, map_y, output;
    double Cy = (double)srcFrame.cols/2;
    double Cx = (double)srcFrame.rows/2;
    map_x.create(srcFrame.size(), CV_32FC1);
    map_y.create(srcFrame.size(), CV_32FC1);
    
    for (int x=0; x<map_x.rows; x++) {
        for (int y=0; y<map_y.cols; y++) {
            double r2 = (x-Cx)*(x-Cx) + (y-Cy)*(y-Cy);
            map_x.at<float>(x,y) = (double) ((y-Cy)/(1 + double(k1/1000000.0)*r2)+Cy); // se suma para obtener la posicion absoluta
            map_y.at<float>(x,y) = (double) ((x-Cx)/(1 + double(k1/1000000.0)*r2)+Cx); // la posicion relativa del punto al centro
        }
    }
    remap(srcFrame, output, map_x, map_y, CV_INTER_LINEAR);
    return output;
}
