//
//  Alien.cpp
//  opencv
//
//  Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
//  Copyright (c) 2014. All rights reserved.
//

#include "Alien.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Variables globales

/*
 * Porporciona una elipse vertical en el centro de la imagen, el resto
 * sera de color negro.
 */
Mat faceDetection(Mat frame){
    Mat faceOutline = Mat::zeros(frame.size(), CV_8UC3);
    Scalar color = CV_RGB(255, 255, 255);
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

/*
 * Metodo no utilizado.
 */
void kmedias(Mat src,Mat original)
{
    cvtColor(src, src, CV_BGR2HLS);
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
    
     //imshow("original", src);
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
    
    
    cout << "src.at<Vec3f>(400,200): " << endl;
    cout << src.at<Vec3b>(400,200) << endl;
    
    cout << "pixel: " << endl;
    cout << pixel << endl;
    
    cout << "centro cluster 0: " << endl;
    cout << centers.at<cv::Vec3f>(0) << endl;
    cout << "centro cluster 1: " << endl;
    cout << centers.at<cv::Vec3f>(1) << endl;
    cout << "centro cluster 2: " << endl;
    cout << centers.at<cv::Vec3f>(2) << endl;
    
    cout << "icov: " << endl;
    cout << convarianzaInvertida << endl;
    /**
    for( int x = 0; x < src.rows; x++ ){
        for( int y = 0; y < src.cols; y++ )
        {
            pixel[0] = (double)src.at<Vec3b>(x,y)[0];
            pixel[1] = (double)src.at<Vec3b>(x,y)[1];
            pixel[2] = (double)src.at<Vec3b>(x,y)[2];
    double res = Mahalanobis(pixel, centers.at<cv::Vec3f>(0), convarianzaInvertida);
    double res1 = Mahalanobis(pixel, centers.at<cv::Vec3f>(1), convarianzaInvertida);
    double res2 = Mahalanobis(pixel, centers.at<cv::Vec3f>(2), convarianzaInvertida);
            /*if (res1<res&&res1<res2) {
                printf("res=%f res1=%f res2=%f\n",res,res1,res2);
                src.at<Vec3b>(x,y)[0]=0;
                src.at<Vec3b>(x,y)[1]=0;
                src.at<Vec3b>(x,y)[2]=0;            }
    
    
        }}*/

    pixel[0] = (double)src.at<Vec3b>(400,300)[0];
    pixel[1] = (double)src.at<Vec3b>(400,300)[1];
    pixel[2] = (double)src.at<Vec3b>(400,300)[2];
    double res = Mahalanobis(pixel, centers.at<cv::Vec3f>(0), convarianzaInvertida);
    double res1 = Mahalanobis(pixel, centers.at<cv::Vec3f>(1), convarianzaInvertida);
    double res2 = Mahalanobis(pixel, centers.at<cv::Vec3f>(2), convarianzaInvertida);
    printf("pixel = %f,%f,%f\n",pixel[0],pixel[1],pixel[2]);
    printf("Mahalanobis a cluster 0 = %f\n",res);
    printf("Mahalanobis a cluster 1 = %f\n",res1);
    printf("Mahalanobis a cluster 2 = %f\n",res2);
    


    int tipo0 = 0,tipo1=0,tipo2=0;
    for( int x = 0; x < src.rows; x++ ){
        for( int y = 0; y < src.cols; y++ ){
            if (labels.at<int>(x, y)==0) {
                tipo0++;
                src.at<Vec3b>(x,y)[0]=0;
                src.at<Vec3b>(x,y)[1]=0;
                src.at<Vec3b>(x,y)[2]=0;}
            else if (labels.at<int>(x, y)==1) {
                tipo1++;
                src.at<Vec3b>(x,y)[0]=0;
                src.at<Vec3b>(x,y)[1]=255;
                src.at<Vec3b>(x,y)[2]=0;}
            else if (labels.at<int>(x, y)==2) {
                tipo2++;
                src.at<Vec3b>(x,y)[0]=0;
                src.at<Vec3b>(x,y)[1]=0;
                src.at<Vec3b>(x,y)[2]=255;}
        }
    }
    
    printf("labels.size=%d\n",labels.rows*labels.cols);
    printf("pixeles totales = %d\n",640*480);
    printf("tipo0=%d\n",tipo0);
    printf("tipo1=%d\n",tipo1);
    printf("tipo2=%d\n",tipo2);
    printf("total=%d\n",tipo0+tipo1+tipo2);
    
    
    cout << "labels: " << endl;
    cout << labels.at<int>(400,300) << endl;
    
    cvtColor(src, src, CV_HLS2BGR);
    imshow("mierdapati", src);
    waitKey();
}

/*
 * Calcula la mediana de los pixeles no negros en el espacio de color
 * hsv, por separado.
 */
Vec3b obtenerMediana(Mat srcFrame){
    // Se convierte la imagen de entrada en hsv
    cvtColor(srcFrame, srcFrame, CV_BGR2HSV);
    
    Vec3b negro(0,0,0);
    Vec3b actual;
    vector<int> h,s,v;
    for( int x = 0; x < srcFrame.rows; x++ ){
        for( int y = 0; y < srcFrame.cols; y++ ){
            actual = srcFrame.at<Vec3b>(x,y);
            if (actual[0]!=negro[0]&&actual[1]!=negro[1]&&actual[2]!=negro[2]) {
                h.push_back(srcFrame.at<Vec3b>(x,y)[0]);
                s.push_back(srcFrame.at<Vec3b>(x,y)[1]);
                v.push_back(srcFrame.at<Vec3b>(x,y)[2]);
            }
        }
    }
    
    cvtColor(srcFrame, srcFrame, CV_HSV2BGR);
    
    sort(h.begin(),h.end());
    sort(s.begin(),s.end());
    sort(v.begin(),v.end());
    
    printf("mediana de h = %d\n", h.at(h.size()/2));
    printf("mediana de s = %d\n", s.at(s.size()/2));
    printf("mediana de v = %d\n", v.at(v.size()/2));
    
    Vec3b mediana(h.at(h.size()/2),s.at(s.size()/2),v.at(v.size()/2));
    return mediana;
}

/*
 * Anyade el tono proporcionado por parametro a los pixeles cuyos valores
 * del espacio hsv estan dentro del rango introducido mediante los
 * parametros rango y mediana:
 * Si |f(i,j)-mediana|<rango ==> f(i,j)[h] = f(i,j)[h] + color.
 */
Mat pintarColorMediana(Mat srcFrame,Vec3b mediana,int color,int rango){
    Mat dstFrame;
    cvtColor(srcFrame, dstFrame, CV_BGR2HSV);
    bool seParece[3];
    Vec3b aSumar(0,0,0);
    aSumar[0] = color;
    for( int x = 0; x < dstFrame.rows; x++ ){
        for( int y = 0; y < dstFrame.cols; y++ ){
            for (int c = 0; c < 3; c++) { // Se parecen los 3 valores?
                if (abs(dstFrame.at<Vec3b>(x,y)[c]-mediana[c])<rango) {
                    seParece[c] = true;
                }
            }
            if (seParece[0]&&seParece[1]&&seParece[2]){
                dstFrame.at<Vec3b>(x,y)[0] += aSumar[0];
                dstFrame.at<Vec3b>(x,y)[0] %= 255;
            }
            seParece[0] = false;seParece[1] = false;seParece[2] = false;
        }
    }
    // Se convierte la imagen de salida en rgb
    cvtColor(dstFrame, dstFrame, CV_HSV2BGR);
    return dstFrame;
}
