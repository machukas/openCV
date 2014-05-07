//
//  Trabajo3.h
//  Vision por computador
//
//  Created by Adrián Marín Colás on 05/05/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#ifndef __Vision_por_computador__Trabajo3__
#define __Vision_por_computador__Trabajo3__

#include <iostream>
#include <stdio.h>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <sys/types.h>
#include <dirent.h>

using namespace std;
using namespace cv;

/*/Users/amarincolas/Developer/img/poster/
 * /Users/amarincolas/Developer/img/poster/
 */
Mat marcoPanorama(Mat foto, Mat acoplada, const Mat Homografia, Size2i *tam);

/*
 *
 */
Mat merge(Mat img_1, Mat img_2, Mat Homografia);

/*
 *
 */
void showMenu();

/*
 *
 */
void getDir(char* directorio);

void calibrar();

/*
 *
 */
vector<Mat> procesarDirectorio(char* directorio, char* inlier);

/*
 *
 */
double generarPanorama(vector<Mat> imagenes, bool flag);


#endif /* defined(__Vision_por_computador__Trabajo3__) */
