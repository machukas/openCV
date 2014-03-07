/*
 *  BIBLIOGRAFIA:
 *      - Tutoriales opencv.
 *      - Mastering opencv with practical computer vision projects (ISBN: 1849517827)
 */

#include <opencv2/opencv.hpp>
#include "Contraste.h"
#include "Alien.h"
#include "Poster.h"
#include "Dibu.h"
#include "Distorsion.h"

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
    printf("Opcion 5: Distorsion\n\n");
    printf("Ingrese una opcion (0 para salir) >> ");
    scanf("%d",&opcion);
    return opcion;
}


int inicializarVideo(int opcion) {
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    if (!cap.isOpened()) { return -1; }
    Mat srcFrame,dstFrame,auxFrame;
    namedWindow("Original");
    
    //cvtColor(frame, edges);
    //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
    //Canny(edges, edges, 0, 30, 3);
    if (opcion==1) {    // Contraste
        int alpha = 50, beta = 50;
        namedWindow("Contraste");
        createTrackbar("Alpha", "Contraste", &alpha, 100, NULL);
        createTrackbar("Beta", "Contraste", &beta, 100, NULL);
        namedWindow("Histograma imagen original", CV_WINDOW_AUTOSIZE );
        namedWindow("Histograma imagen ecualizada", CV_WINDOW_AUTOSIZE );
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            // Se muestra la imagen contrastada con alpha y beta
            double gain = alpha / 50.0; // ganancia entre 0 y 2
            int bias = beta-50;         // sesgo entre 0 y 50
            imshow("Contraste", metodoContraste(srcFrame,gain,bias));
            // Se muestra la imagen con el histograma ecualizado
            dstFrame = ecualizarHistograma(srcFrame);
            imshow("Ecualizada", dstFrame);
            // Se muestran los dos histogramas
            imshow("Histograma imagen original",Histograma(srcFrame));
            imshow("Histograma imagen ecualizada",Histograma(dstFrame));
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==2) {     // Alien
        Mat frameWithFace;
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            frameWithFace = faceDetection(srcFrame);
            imshow("Alien", metodoAlien(frameWithFace));
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==3) {     // Poster
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            imshow("Poster",metodoPoster(srcFrame));
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==4) {     // Dibu
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            imshow("Dibu",metodoDibu(srcFrame));
            if (waitKey(30)>=0) { destroyAllWindows();  break; }
        }
    }
    else if (opcion==5) {     // Distorsion
        for (; ; ) {
            // Se extrae un nuevo fotograma
            cap >> srcFrame;
            imshow("Original", srcFrame);
            imshow("Distorsion",metodoDistorsion(srcFrame));
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
    else if (opcion==0) { return -1; }
    return 0;
}

int main( int argc, char** argv )
{
    while (opciones(menu())!=-1) {
        ;
    }
}

