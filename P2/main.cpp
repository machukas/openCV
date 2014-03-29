//
//  main.cpp
//  Vision por computador
//
//  Created by Nicolas Landa Tejero-Garces on 19/03/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include "main.h"

using namespace cv;
using namespace std;

/*void sobel_filtering(Mat image_gray, Mat &outputX, Mat &outputY) {
    
    int sobelX[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
    int sobelY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
    
    for (int x=1; x<image_gray.rows-1; x++) {
        for (int y=1; y<image_gray.cols-1; y++) {
            for (int i=-1; i<2; i++) {
                for (int j=-1; j<2; j++) {
                    outputX.at<int>(x,y) += sobelX[i+1][j+1] * image_gray.at<int>(x+i,y+j);
                    outputY.at<int>(x,y) += sobelY[i+1][j+1] * image_gray.at<int>(x+i,y+j);
                }
            }
            outputX.at<int>(x,y) = outputX.at<int>(x,y);
            //outputY.at<int>(x,y) = outputY.at<int>(x,y)/4;
            //outputX.at<int>(x,y) = sqrt(outputX.at<int>(x,y)^2 + outputY.at<int>(x,y)^2);
            //outputX.at<int>(x,y) = outputX.at<int>(x,y)/2 + 128;
        }
    }
}*/

void sobel_filtering(Mat image_gray, Mat &outputX, Mat &outputY, Mat &modulo, Mat &angle) {
    
    GaussianBlur(image_gray, image_gray, Size(3,3), 0);
    Sobel(image_gray, outputX, CV_32F, 1, 0, 3);
    Sobel(image_gray, outputY, CV_32F, 0, 1, 3);
    
    cartToPolar(outputX, outputY, modulo, angle);
    
}

int main( int argc, char** argv )
{
    Mat image_gray;
    Mat image = imread("/Users/machukas/Desktop/poster.pgm");
    cvtColor(image, image_gray, CV_BGR2GRAY);
    Mat outputX = Mat::zeros(image_gray.size(),image_gray.type());
    Mat outputY = Mat::zeros(image_gray.size(),image_gray.type());
    Mat output = Mat::zeros(image_gray.size(),image_gray.type());
    Mat modulo = Mat::zeros(image_gray.size(),image_gray.type());
    Mat angle = Mat::zeros(image_gray.size(),image_gray.type());;
    
    sobel_filtering(image_gray,outputX,outputY,modulo,angle);
    
    angle = (angle/3.14)*0.5;
    outputX = (outputX/2)+128;
    outputY = (outputY/2)+128;
    modulo = modulo/4;
    
    namedWindow("GradienteX");
    imshow("GradienteX", (outputX/255));
    namedWindow("GradienteY");
    imshow("GradienteY", (outputY/255));
    namedWindow("Orientacion");
    imshow("Orientacion", angle);
    namedWindow("Modulo");
    imshow("Modulo", (modulo/255));
    
    for (;;) {
        if (waitKey(30)>=0) { destroyAllWindows();  break; }
    }
    
    // Parte 2
    
    Mat src = image.clone();
    
    for (int i=1; i<src.rows-1; i++) {
        for (int j=1; j<src.cols-1; j++) {
            if (modulo.at<float>(i, j) >= 40) {
                int x = j-src.cols/2;
                int y = src.rows/2 -i;
                float theta = angle.at<float>(i,j);
                float ro = x*cos(theta) + y*sin(theta);
                
            }
        }
    }
    
}