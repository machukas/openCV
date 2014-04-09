//
//  main.cpp
//  Vision por computador
//
//  Nicolas Landa Tejero-Garces & Adrian Marin Colas
//  All rights reserved.
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

/**
 * Aplica el operador sobel a una imagen [image_gray] para obtener sus gradientes, horizontal, 
 * vertical, modulo y orientacion. Previamente se le aplica un GaussianBlur.
 */
void sobel_filtering(Mat image_gray, Mat &outputX, Mat &outputY, Mat &modulo, Mat &angle) {
    GaussianBlur(image_gray, image_gray, Size(3,3), 0);
    Sobel(image_gray, outputX, CV_32F, 1, 0, 3);
    Sobel(image_gray, outputY, CV_32F, 0, 1, 3);
    cartToPolar(-outputX, -outputY, modulo, angle);
}

/**
 * Guarda en un vector [rectas] de tamano el ancho de la imagen [src], el numero de votos
 * que recibe cada pixel del horizonte (y=src.rows/2)de las rectas perpendiculares a los 
 * gradientes de los contornos de la imagen.
 * El punto con mayor numero de votos correspondera con el punto de fuga de la imagen.
 */
void votarRecta(int rectas[], int x, int y, int i, int j, float theta, Mat src){
    if (fmod(theta,(CV_PI/2))>0,25) {
        float m = tan((CV_PI/2)-theta);
        float n = y-m*x;
        // interseccion de y=mx+n con y=0
        int corte = -n/m;
        corte = corte + src.cols/2;
        if (corte<src.cols && corte>=0) {
            rectas[corte] = rectas[corte] + 1;
            /*
             * Descomentar para visualizar las rectas perpendiculares al gradiente que
             * votan para el calculo del punto de fuga.
             *
             * line(src, Point(j,i), Point(corte,src.rows/2), CV_RGB(255,0,0));
             * imshow("Pasillo", src);
             * for (;;) {
             *   if (waitKey(30)>=0) { destroyAllWindows();  break; }
            }*/
        }
    }
}

int  main( int argc, char** argv ) {
    
    ///////////////////////////////////////////////////////////////////////////
    //////////////////////            Parte 1            //////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    Mat image_gray;
    Mat image = imread("/Users/amarincolas/Desktop/poster.pgm");
    //Mat image = imread("/Users/machukas/Desktop/poster.pgm");
    cvtColor(image, image_gray, CV_BGR2GRAY);
    Mat outputX = Mat::zeros(image_gray.size(),image_gray.type());
    Mat outputY = Mat::zeros(image_gray.size(),image_gray.type());
    Mat output = Mat::zeros(image_gray.size(),image_gray.type());
    Mat modulo = Mat::zeros(image_gray.size(),image_gray.type());
    Mat angle = Mat::zeros(image_gray.size(),image_gray.type());
    
    sobel_filtering(image_gray,outputX,outputY,modulo,angle);
    
    printf("%f %f\n",angle.at<float>(20,30),angle.at<float>(30.40));
    
    // Se modifican valores para su visualizacion
    angle = (angle/CV_PI)*128;
    outputX = (outputX/2)+128;
    outputY = (-outputY/2)+128;
    modulo = modulo/4;
    
    // Se muestran los gradientes, modulo y orientacion
    namedWindow("GradienteX");
    imshow("GradienteX", (outputX/255));
    namedWindow("GradienteY");
    imshow("GradienteY", (outputY/255));
    namedWindow("Orientacion");
    imshow("Orientacion", angle/255);
    namedWindow("Modulo");
    imshow("Modulo", (modulo/255));
    
    for (;;) {
        if (waitKey(30)>=0) { destroyAllWindows();  break; }
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //////////////////////            Parte 2            //////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    // Cargar imagen y calcular sus gradientes, orientacion ...
    Mat src = imread("/Users/amarincolas/Desktop/pasillo1.pgm");
    cvtColor(src, image_gray, CV_BGR2GRAY);
    sobel_filtering(image_gray,outputX,outputY,modulo,angle);
    
    // Recta de votacion inicializarla a 0
    int rectas[src.cols];
    for (int n=0; n<src.cols-1; ++n)
        rectas[n] = 0;
    
    // Algoritmo de votacion
    for (int i=1; i<src.rows-1; i++) {
        for (int j=1; j<src.cols-1; j++) {
            if (modulo.at<float>(i, j) >= 70) {
                int x = j-src.cols/2;       // eje x en funcion del centro
                int y = src.rows/2 -i;      // eje y en funcion del centro
                float theta = angle.at<float>(i,j);
                votarRecta(rectas, x, y, i, j, theta, src);
            }
        }
    }

    // Obtener la posicion mas votada por el algoritmo
    int best=0;
    for (int a=1; a<src.cols-1; a++) {
        if (rectas[a]>rectas[best]) { best=a; }
    }
    
    // Marcar el punto de fuga en la imagen
    circle(src, Point(best,src.rows/2), 4, CV_RGB(0,255,0));
    namedWindow("Pasillo");
    imshow("Pasillo", src);
    
    for (;;) {
        if (waitKey(30)>=0) { destroyAllWindows();  break; }
    }
    
}