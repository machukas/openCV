/*
 *  BIBLIOGRAFIA:
 *      - Tutoriales opencv.
 *      - Mastering opencv with practical computer vision projects (ISBN: 1849517827)
 *      - stackoverflow
 *
 * Created by Nicolas Landa Tejero-Garces & Adrián Marín Colás
 * Copyright (c) 2014. All rights reserved.
 */

#include <opencv2/opencv.hpp>
#include "Contraste.h"
#include "Alien.h"
#include "Poster.h"
#include "Dibu.h"
#include "Distorsion.h"
#include "Vuelta.h"
#include "Sepia.h"

using namespace cv;
using namespace std;

//Variables globales

int menu() {
    int opcion;
    printf("MENU\n");
    printf("Opcion 1: Contraste\n");
    printf("Opcion 2: Alien\n");
    printf("Opcion 3: Poster\n");
    printf("Opcion 4: Dibu\n");
    printf("Opcion 5: Distorsion\n");
    printf("Opcion 6: Vuelta 180 grados\n");
    printf("Opcion 7: Sepia\n");
    printf("Opcion 8: Tunel\n\n");
    printf("Ingrese una opcion (0 para salir) >> ");
    scanf("%d",&opcion);
    return opcion;
}


int inicializarVideo(int opcion) {
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    if (!cap.isOpened()) { return -1; }
    Mat srcFrame;
    namedWindow("Original");
    //Variables para el tiempo
    double t = 0.0;
    char tiempoEjecucion[255];
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 1;
    int thickness = 1;
    cv::Point textOrg(10, 450);
    if (opcion==1) {    // Contraste
        Mat contrastada,ecualizada;
        int alpha = 50, beta = 50;
        namedWindow("Contraste");
        createTrackbar("Ganancia", "Contraste", &alpha, 150);
        createTrackbar("Sesgo", "Contraste", &beta, 100);
        namedWindow("Histograma imagen original", CV_WINDOW_AUTOSIZE );
        namedWindow("Histograma imagen ecualizada", CV_WINDOW_AUTOSIZE );
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            // Se muestra la imagen contrastada con alpha y beta
            double gain = alpha / 50.0; // ganancia entre 0 y 3
            int bias = beta-50;         // sesgo entre 0 y 50
            t = (double) getTickCount();
            contrastada = metodoContraste(srcFrame,gain,bias);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(contrastada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Contraste", contrastada);
            // Se muestra la imagen con el histograma ecualizado
            t = (double) getTickCount();
            ecualizada = ecualizarHistograma(srcFrame);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(ecualizada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Ecualizada", ecualizada);
            // Se muestran los dos histogramas
            imshow("Histograma imagen original",HistogramaBN(srcFrame));
            imshow("Histograma imagen ecualizada",HistogramaBN(ecualizada));
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==2) {     // Alien
        Mat frameWithFace;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            // Se presenta una elipse para poner la geta.
            frameWithFace = faceDetection(srcFrame);
            imshow("Alien", frameWithFace);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
        namedWindow("Alien");
        int color = 0,rango = 30;
        createTrackbar("Color", "Alien", &color, 255);
        createTrackbar("Error", "Alien", &rango, 100);
        Mat alienizada;
        // Se obtiene la mediana del color de la piel de la cara
        Vec3b mediana = obtenerMediana(frameWithFace);
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            // Se pinta lo detectado como piel
            alienizada = pintarColorMediana(srcFrame, mediana,color,rango);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(alienizada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Alien",alienizada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
        //kmedias(frameWithFace, srcFrame);
    }
    else if (opcion==3) {     // Poster
        Mat posterizada;
        namedWindow("Poster");
        int div = 32;
        createTrackbar("color", "Poster", &div, 255, NULL);
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            posterizada = metodoPoster(srcFrame,div);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(posterizada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Poster",posterizada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==4) {     // Dibu
        Mat dibuizada;
        namedWindow("Dibu");
        int div = 32;
        createTrackbar("color", "Dibu", &div, 255, NULL);
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            dibuizada = metodoDibu(srcFrame,div);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(dibuizada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Dibu",dibuizada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==5) {     // Distorsion
        int k = 0;
        namedWindow("Distorsion");
        createTrackbar("k1", "Distorsion", &k, 20, NULL);
        Mat distorsionada;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            int k1 = k-10;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            distorsionada = metodoDistorsion(srcFrame,k1);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(distorsionada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Distorsion",distorsionada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==6) {     // Vuelta
        Mat espejada;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            espejada = metodoVuelta(srcFrame);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(espejada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Vuelta",espejada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==7) {     // Sepia
        Mat sepiada;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            sepiada = metodoSepia(srcFrame);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(sepiada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Sepia",sepiada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==8) {     // Tunel
        int k = -185;
        int a = 0,b = 64;
        namedWindow("Tunel");
        Mat distorsionada;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            if (a>=0 && a<64 && b==64) {
                k = k + 5;
                a++;
            } else if (a==64) {
                k = k + 5;
                a = -1;
            } else if (b<=64 && b>0 && a==-1) {
                k = k - 5;
                b--;
            } else if (b==0) {
                k = k - 5;
                b = 64;
                a = 0;
            }
            imshow("Original", srcFrame);
            t = (double) getTickCount();
            distorsionada = metodoDistorsion(srcFrame,k);
            t = ((double)getTickCount()-t)/getTickFrequency();
            sprintf(tiempoEjecucion, "Tiempo de calculo = %f segundos.", (double)t);
            putText(distorsionada, tiempoEjecucion, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
            imshow("Tunel",distorsionada);
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    return 0;
}

int opciones(int opcion) {
    if (opcion==1) { inicializarVideo(1); }         // Contraste
    else if (opcion==2) { inicializarVideo(2); }    // Alien
    else if (opcion==3) { inicializarVideo(3); }    // Poster
    else if (opcion==4) { inicializarVideo(4); }    // Dibu
    else if (opcion==5) { inicializarVideo(5); }    // Distorsion
    else if (opcion==6) { inicializarVideo(6); }    // Espejo
    else if (opcion==7) { inicializarVideo(7); }    // Sepia
    else if (opcion==8) { inicializarVideo(8); }    // Tunel
    else if (opcion==0) { return -1; }
    return 0;
}

int main( int argc, char** argv )
{
    while (opciones(menu())!=-1) {
        ;
    }
}

