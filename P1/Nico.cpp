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
int alpha = 1;
int beta = 0;

int menu() {
    // you
    int opcion;
    printf("MENU hola caracola\n");
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
    for (;;) {
        cap >> srcFrame;
        imshow("Original", srcFrame);
        //cvtColor(frame, edges);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        if (opcion==1) {    // Contraste
            mostrarHistograma(srcFrame,"Histograma imagen original");
            imshow("Contraste", metodoContraste(srcFrame,alpha,beta));
            /// Create Trackbars
            createTrackbar("Alpha", "Contraste", &alpha, 7, NULL);
            createTrackbar("Beta", "Contraste", &beta, 50, NULL);
            dstFrame = ecualizarHistograma(srcFrame);
            imshow("Ecualizada", dstFrame);
            
            mostrarHistograma(dstFrame, "Histograma imagen ecualizada");
        }
        else if (opcion==2) {     // Alien
            imshow("Alien", faceDetection(srcFrame));
            //imshow("Alien", metodoAlien(frame));
        }
        else if (opcion==3) {     // Poster
            imshow("Poster",metodoPoster(srcFrame));
        }
        else if (opcion==4) {     // Dibu
            imshow("Dibu",metodoDibu(srcFrame));
        }
        else if (opcion==5) {     // Distorsion
            imshow("Distorsion",metodoDistorsion(srcFrame));
        }
        if (waitKey(30)>=0) { destroyAllWindows();  break; }
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

