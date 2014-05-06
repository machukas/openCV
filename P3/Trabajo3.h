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
#include <opencv2/opencv.hpp>
#include <sys/types.h>
#include <dirent.h>

using namespace std;
using namespace cv;

/*
 *
 */
void showMenu();

/*
 *
 */
void getDir(char* directorio);

/*
 *
 */
vector<Mat> procesarDirectorio(char* directorio);

/*
 *
 */
void generarPanorama(vector<Mat> imagenes);


#endif /* defined(__Vision_por_computador__Trabajo3__) */
