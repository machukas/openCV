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

void gradiente(Mat image_gray, Mat &outputX, Mat &outputY) {
    
    GaussianBlur(image_gray, image_gray, Size(3,3), 0);
    Sobel(image_gray, outputX, CV_32F, 1, 0, 3, 0.75);
    Sobel(image_gray, outputY, CV_32F, 0, 1, 3, 0.75);
    
    double min, max;
    minMaxLoc(outputX, &min, &max);
    //outputX.convertTo(outputX, CV_32F, 0.5, 128);
    printf("%2.2f %2.2f\n",min,max);
    minMaxLoc(outputY, &min, &max);
    //outputY.convertTo(outputY, CV_32F, 0.5, 128);
    printf("%2.2f %2.2f\n",min,max);
}



void sobel_filtering(Mat image_gray, Mat &outputX, Mat &outputY, Mat &output) {
    
    Mat modulo;
    gradiente(image_gray,outputX,outputY);
    
    for (int x=0; x<image_gray.rows; x++) {
        for (int y=0; y<image_gray.cols; y++) {
            modulo = sqrt(pow(outputX.at<float>(x,y),2) + pow(outputY.at<float>(x,y),2));
        }
    }
    
    convertScaleAbs(outputX, outputX,0.5,128);
    convertScaleAbs(outputY, outputY);
    
    
    
}


int main( int argc, char** argv )
{
    Mat image_gray;
    Mat image = imread("/Users/amarincolas/Desktop/poster.pgm");
    cvtColor(image, image_gray, CV_BGR2GRAY);
    Mat outputX = Mat::zeros(image_gray.size(),image_gray.type());
    Mat outputY = Mat::zeros(image_gray.size(),image_gray.type());
    Mat output = Mat::zeros(image_gray.size(),image_gray.type());
    
    sobel_filtering(image_gray,outputX,outputY,output);
    
    namedWindow("test");
    imshow("test", outputX);
    namedWindow("test2");
    imshow("test2", outputY);
    namedWindow("test3");
    imshow("test3", output);
    
    //addWeighted(outputX, 0.5, outputY, 0.5, 0, output);
    //namedWindow("test3");
    //imshow("test3", output);
    
    
    for (;;) {
        if (waitKey(30)>=0) { destroyAllWindows();  break; }
    }
    
}