//
//  Contraste.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#include "Contraste.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat HistogramaBN(Mat frame){
    Mat bnFrame;
    cvtColor(frame, bnFrame, CV_BGR2GRAY);
    
    // Numero de valores
    int histSize = 256;
    
    // Rango de valores
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat bn_hist;
    
    /// Calcula el histograma
    calcHist( &bnFrame, 1, 0, Mat(), bn_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Dibuja el histograma
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normaliza los resultados a [ 0, histImage.rows ]
    normalize(bn_hist, bn_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(bn_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(bn_hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
    }
    
    return histImage;
}

Mat HistogramaRGB(Mat frame){
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( frame, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat b_hist, g_hist, r_hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
    }
    
    return histImage;
}

Mat ecualizarHistograma(Mat frame){
    vector<Mat> channels;
    Mat img_hist_equalized;
    
    cvtColor(frame, img_hist_equalized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format
    
    split(img_hist_equalized,channels); //split the image into channels
    
    equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)
    
    merge(channels,img_hist_equalized); //merge 3 channels including the modified 1st channel into one image
    
    cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)
    
    return img_hist_equalized;
}

Mat metodoContraste(Mat frame,double alpha,int beta) {
    Mat output;
    frame.copyTo(output);
    for( int y = 0; y < output.rows; y++ ){ //Para cada fila
        for( int x = 0; x < output.cols; x++){ //Para cada columna
            for( int c = 0; c < 3; c++ ){ //Para cada canal
                output.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( frame.at<Vec3b>(y,x)[c] ) + beta );
                //saturate_cast es para prevenir valores fuera de rango
            }
        }
    }
    /*
     * Similar a frame.convertTo(output, -1, alpha, beta);
     */
    return output;
}
