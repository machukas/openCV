//
//  Alien.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces on 26/02/14.
//  Copyright (c) 2014 Nicolas Landa Tejero-Garces. All rights reserved.
//

#include "Alien.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat faceDetection(Mat frame){
    Mat faceOutline = Mat::zeros(frame.size(), CV_8UC3);
    Scalar color = CV_RGB(255, 255, 255);
    int thickness = 4;
    int sw = frame.size().width;
    int sh = frame.size().height;
    int faceH = sh/2 * 70/100;
    int faceW = faceH * 72/100;
    // El parametro -1 indica que la elipse está rellena.
    ellipse(faceOutline, Point(sw/2, sh/2), Size(faceW, faceH), 0, 0, 360, color, -1, CV_AA);
    Mat dst;
    frame.copyTo(dst,faceOutline);
    return dst;
}

Mat metodoAlien(Mat frame) {
    Mat destination;
    /*faceDetection(frame);
    /* Reduce the number of pixels by a factor of four
    Size size = frame.size();
    Size smallSize;
    smallSize.width = size.width/4;
    smallSize.height = size.height/4;
    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(frame, smallImg, smallSize, 0,0, INTER_LINEAR); */
    
    //Mat yuv = Mat(frame.size(), CV_8UC3);
    //cvtColor(frame, yuv, CV_BGR2YCrCb);
    int sw = frame.size().width;
    int sh = frame.size().height;
    Mat mask, maskPlusBorder;
    maskPlusBorder = Mat::zeros(sh+2, sw+2, CV_8UC1);
    mask = maskPlusBorder(Rect(1,1,sw,sh));
    //resize(edge, mask, frame);
    
    
    //imshow("edgeMask", edgeMask);
    
    const int EDGES_THRESHOLD = 80;
    threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
    dilate(mask,mask,Mat());
    erode(mask,mask,Mat());
    
    int const NUM_SKIN_POINTS = 6;
    Point skinPts[NUM_SKIN_POINTS];
    skinPts[0] = Point(sw/2, sh/2-sh/6);
    skinPts[1] = Point(sw/2-sw/11, sh/2-sh/6);
    skinPts[2] = Point(sw/2+sw/11, sh/2-sh/6);
    skinPts[3] = Point(sw/2, sh/2-sh/16);
    skinPts[4] = Point(sw/2-sw/9, sh/2+sh/16);
    skinPts[5] = Point(sw/2+sw/9, sh/2-sh/16);
    
    const int LOWER_Y = 60;
    const int UPPER_Y = 80;
    const int LOWER_Cr = 25;
    const int UPPER_Cr = 15;
    const int LOWER_Cb = 20;
    const int UPPER_Cb = 15;
    Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
    Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);
    
    const int CONNECTED_COMPONENTS = 4;
    const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY;
    Mat edgeMask = mask.clone();
    for (int i=0; i<NUM_SKIN_POINTS; i++) {
        floodFill(frame, maskPlusBorder, skinPts[i], Scalar(), NULL, lowerDiff, upperDiff, flags);
    }
    mask -= edgeMask;
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    Scalar color = CV_RGB(Red, Green, Blue);
    
    add(frame, color, frame, mask);
    // Ahora en frame está la region de la elipse pintada de verde
    imshow("prueba", frame);
    destination.setTo(0);
    frame.copyTo(destination,mask);
    
    //cvtColor(frame, destination, "CV_BGR2HSV_FULL");
    
    return destination;
}
void kmedias(Mat src)
{
    //step 1 : map the src to the samples
    Mat samples(src.total(), 3, CV_32F);
    auto samples_ptr = samples.ptr<float>(0);
    for( int row = 0; row != src.rows; ++row){
        auto src_begin = src.ptr<uchar>(row);
        auto src_end = src_begin + src.cols * src.channels();
        //auto samples_ptr = samples.ptr<float>(row * src.cols);
        while(src_begin != src_end){
            samples_ptr[0] = src_begin[0];
            samples_ptr[1] = src_begin[1];
            samples_ptr[2] = src_begin[2];
            samples_ptr += 3; src_begin +=3;
        }
    }
    
    //step 2 : apply kmeans to find labels and centers
    int clusterCount = 3;
    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, clusterCount, labels,
                TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,
                                10, 0.01),
               attempts,  KMEANS_PP_CENTERS, centers);
    
    //step 3 : map the centers to the output
    Mat new_image(src.size(), src.type());
    for( int row = 0; row != src.rows; ++row){
        auto new_image_begin = new_image.ptr<uchar>(row);
        auto new_image_end = new_image_begin + new_image.cols * 3;
        auto labels_ptr = labels.ptr<int>(row * src.cols);
        
        while(new_image_begin != new_image_end){
            int const cluster_idx = *labels_ptr;
            auto centers_ptr = centers.ptr<float>(cluster_idx);
            new_image_begin[0] = centers_ptr[0];
            new_image_begin[1] = centers_ptr[1];
            new_image_begin[2] = centers_ptr[2];
            new_image_begin += 3; ++labels_ptr;
        }
    }
     imshow("original", src);
     imshow( "clustered image", new_image );
    
     cout << "R (default) = " << endl <<        centers           << endl << endl;
    
    
    Mat covarianza, convarianzaInvertida,media;
    calcCovarMatrix(centers, covarianza, media, CV_COVAR_NORMAL+CV_COVAR_COLS, -1);
    invert(covarianza, convarianzaInvertida);
    cout << "cov: " << endl;
    cout << covarianza << endl;
    cout << "med: " << endl;
    cout << media << endl;
    Vec3f pixel;
    pixel[0] = (double)src.at<Vec3f>(300,200)[0];
    pixel[1] = (double)src.at<Vec3f>(300,200)[1];
    pixel[2] = (double)src.at<Vec3f>(300,200)[2];
    
    cout << "src.at<Vec3f>(400,200): " << endl;
    cout << src.at<Vec3f>(400,200) << endl;
    
    cout << "pixel: " << endl;
    cout << pixel << endl;
    
    cout << "center: " << endl;
    cout << centers.at<cv::Vec3f>(1) << endl;
    
    cout << "icov: " << endl;
    cout << convarianzaInvertida << endl;
    
    double res = Mahalanobis(pixel, centers.at<cv::Vec3f>(0), convarianzaInvertida);
    double res1 = Mahalanobis(pixel, centers.at<cv::Vec3f>(1), convarianzaInvertida);
    double res2 = Mahalanobis(pixel, centers.at<cv::Vec3f>(2), convarianzaInvertida);
    
    printf("res=%f res1=%f res2=%f",res,res1,res2);
     waitKey();
}
Mat pruebaAlien(Mat src, Mat centers,Mat labels){
    //step 1 : map the src to the samples
    Mat samples(src.total(), 3, CV_32F);
    auto samples_ptr = samples.ptr<float>(0);
    for( int row = 0; row != src.rows; ++row){
        auto src_begin = src.ptr<uchar>(row);
        auto src_end = src_begin + src.cols * src.channels();
        //auto samples_ptr = samples.ptr<float>(row * src.cols);
        while(src_begin != src_end){
            samples_ptr[0] = src_begin[0];
            samples_ptr[1] = src_begin[1];
            samples_ptr[2] = src_begin[2];
            samples_ptr += 3; src_begin +=3;
        }
    }
    //int distance = find_EucledianDist(samples_ptr, centers);
    
    //double res = cv::norm(samples_ptr-centers.at<cv::Vec3b>(1)= 255);
    
    //step 3 : map the centers to the output
    Mat new_image( src.size(), src.type() );
    for( int y = 0; y < src.rows; y++ )
        for( int x = 0; x < src.cols; x++ )
        {
            double res = cv::norm(src.at<Vec3b>(y,x)-centers.at<cv::Vec3b>(1));

            int cluster_idx = labels.at<int>(y + x*src.rows,0);
            new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
            new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
            new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
        }
    
    return new_image;
}
