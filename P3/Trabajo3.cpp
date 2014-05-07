//
//  Trabajo3.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 05/05/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include "Trabajo3.h"

// ----------------------------------------------------------------------------
Mat marcoPanorama(Mat foto, Mat acoplada, const Mat Homografia, Size2i *tam) {
// ----------------------------------------------------------------------------
    // Obtenemos los puntos de la foto actual en escena
	vector<Point2f> esq_acoplada(4);
	esq_acoplada[0] = Point(0,0);
	esq_acoplada[1] = Point(acoplada.size().width,0);
	esq_acoplada[2] = Point(acoplada.size().width,acoplada.size().height);
	esq_acoplada[3] = Point(0,acoplada.size().height);
	// Matriz con el tamano de la panoramica
    const Mat mat_acoplada = Mat(esq_acoplada);
    
	// Se calculan los puntos transformados con la homografia
	Mat mat_transf;
	perspectiveTransform(mat_acoplada,mat_transf,Homografia);

	// Obtenemos los puntos de la foto a colocar
	vector<Point2f> esq_transf(4);
	esq_transf = vector<Point2f>(mat_transf);
    
	// Marco minimo es el panorama de momento
	float ancho_min=0.0, ancho_max=foto.size().width;
    float alto_max=foto.size().height, alto_min=0.0;
	float desp_x = 0.0;
    float desp_y = 0.0;
    
	// Calculan los desplazamientos si son necesarios
	for (unsigned int i=0; i<esq_transf.size(); i++)
	{
		if (esq_transf[i].x < ancho_min) {
            ancho_min = esq_transf[i].x;
            desp_x = abs(ancho_min);
        }
		if (esq_transf[i].x > ancho_max) {
            ancho_max = esq_transf[i].x;
        }
		if (esq_transf[i].y < alto_min){
            alto_min = esq_transf[i].y;
            desp_y = abs(alto_min);
        }
		if (esq_transf[i].y > alto_max){
            alto_max = esq_transf[i].y;
        }
	}
    *tam = Size(ancho_max-ancho_min,alto_max-alto_min);
    Mat matrizDesplazamiento = (Mat_<double>(3,3) << 1, 0, desp_x, 0, 1, desp_y, 0, 0, 1);
    return matrizDesplazamiento;
}

// ----------------------------------------------------------------------------
Mat merge(Mat img_1, Mat img_2, Mat Homografia) {
// ----------------------------------------------------------------------------
	Mat panoramica;
    Size2i tam;
	// Se obtiene el tamano y se calcula el desplazamiento de las fotos
	Mat matrizDesplazamiento = marcoPanorama(img_1,img_2,Homografia, &tam);
    // Recolocas el panorama dentro del marco de la imagen
	warpPerspective(img_1, panoramica, matrizDesplazamiento, tam, INTER_CUBIC);
    // Pegas el panorama con la imagen
	warpPerspective(img_2, panoramica, matrizDesplazamiento*Homografia, tam , INTER_CUBIC, BORDER_TRANSPARENT);
	return panoramica;
}

// -----------------------------------------------------------------------------
void showMenu() {
// -----------------------------------------------------------------------------
	cout << endl;
	cout << "                    TRABAJO 3                  " << endl;
	cout << "  ===========================================  " << endl;
	cout << "                                               " << endl;
	cout << "  1. Introducir fotos.                         " << endl;
	cout << "  2. Sacar fotos en vivo.                      " << endl;
	cout << "                                               " << endl;
	cout << "  4. Salir.                                    " << endl;
    cout << "                                               " << endl;
	cout << "  ============================================." << endl;
    cout << "                                               " << endl;
    cout << "  >> ";
}

// -----------------------------------------------------------------------------
void getDir(char* directorio, char* inlier) {
// -----------------------------------------------------------------------------
    cout << "Introduzca un directorio: >> ";
    cin >> directorio;
    cout << "inliers? [s/n] >> ";
    cin >> inlier;
}

// -----------------------------------------------------------------------------
vector<Mat> procesarDirectorio(char * directorio){
// -----------------------------------------------------------------------------
    vector<Mat> fotos;
    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent;
    
    /* Empezaremos a leer en el directorio actual */
    dir = opendir (directorio);
    
    /* Miramos que no haya error */
    if (dir == NULL) {
        printf("dir==NULL");
        getchar();
        return fotos;
    }
    
    /* Leyendo uno a uno todos los archivos que hay */
    char ruta[50];
    while ((ent = readdir (dir)) != NULL) {
        strcpy(ruta, directorio);
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ) {
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            Mat img = imread( strcat(ruta,ent->d_name),CV_LOAD_IMAGE_GRAYSCALE);
            resize(img, img, Size(512,384), 0, 0, INTER_CUBIC);
            fotos.push_back(img);
            printf("Se anyade %s\n",ruta);
        }
    }
    closedir (dir);
    return fotos;
}

// -----------------------------------------------------------------------------
double generarPanorama(vector<Mat> imagenes, bool flag) {
// -----------------------------------------------------------------------------

    double t = 0.0,total=0.0;
    double a = 0.0;
    Mat img_1,img_2,result;
    
    for (int i = 1; i<imagenes.size(); i++) {
        
        t = (double) getTickCount();
        img_1 = imagenes.at(0);
        img_2 = imagenes.at(i);
        
        
        //a = (double) getTickCount();
        //-- Step 1: Detect the keypoints using SURF Detector
        int minHessian = 400;
        SurfFeatureDetector detector( minHessian );
        
        vector<KeyPoint> keypoints_1, keypoints_2;
        
        detector.detect( img_1, keypoints_1 );
        detector.detect( img_2, keypoints_2 );
        
        //-- Step 2: Calculate descriptors (feature vectors)
        SurfDescriptorExtractor extractor;
        
        Mat descriptors_1, descriptors_2;
        
        extractor.compute( img_1, keypoints_1, descriptors_1 );
        extractor.compute( img_2, keypoints_2, descriptors_2 );
        
        //a = ((double)getTickCount()-a)/getTickFrequency();
        //printf("Tiempo = %f segundos.\n", (double)a);
        
        //-- Step 3: Matching descriptor vectors with a brute force matcher
        BFMatcher matcher(NORM_L2);
        std::vector< std::vector<DMatch> > matches;
        matcher.knnMatch(descriptors_1, descriptors_2, matches, 2);
        
        vector<cv::DMatch> good_matches;
        for (int i = 0; i < matches.size(); ++i)
        {
            const float ratio = 0.8; // As in Lowe's paper; can be tuned
            if (matches[i][0].distance < ratio * matches[i][1].distance)
            {
                good_matches.push_back(matches[i][0]);
            }
        }
        
        //-- Draw matches
        Mat img_matches;
        drawMatches( img_1, keypoints_1, img_2, keypoints_2, good_matches, img_matches );
        
        imshow("a",img_matches);
        waitKey();
        
        //-- Localize the object from img_1 in img_2
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt );
        }
        
        Mat mask;
        Mat H = findHomography( scene, obj, CV_RANSAC,3,mask);
        vector<cv::DMatch> inliers;
        // Somewhere before I did: findHomography(points1, points2, CV_RANSAC, 3, mask)
        for (int i = 0; i < obj.size(); i++)
        {
            // Select only the inliers (mask entry set to 1)
            if ((int)mask.at<uchar>(i, 0) == 1)
            {
                inliers.push_back(good_matches[i]);
            }
        }
        if (flag==true) {
            drawMatches( img_1, keypoints_1, img_2, keypoints_2, inliers, img_matches );
            imshow("img_matches", img_matches);
            waitKey(0);
        }
        
        result =merge(img_1,img_2,H);

        imagenes[0] = result;
        t = ((double)getTickCount()-t)/getTickFrequency();
        total+=t;
        printf("Tiempo de calculo = %f segundos.\n", (double)t);
    }
    //cv::medianBlur(result, result, 3);
    imshow("resultado",result);
   
    return total;
}

// -----------------------------------------------------------------------------
void calibrar() {
// -----------------------------------------------------------------------------
    Mat temp = imread("/Users/amarincolas/Developer/img/test1/IMG_1015.JPG",CV_LOAD_IMAGE_COLOR);
    Size s = Size(512,384);
    resize(temp, temp, Size(512,384), 0, 0, INTER_CUBIC);
    // Obtener parametros del XML
    Mat cameraMatrix, distCoeffs;
    FileStorage fs("/Users/amarincolas/Developer/out_camera_data.xml",FileStorage::READ);
    fs["Camera_Matrix"] >> cameraMatrix;
    fs["Distortion_Coefficients"] >> distCoeffs;
    fs.release();
    Mat view = temp.clone();
    Mat map1,map2;
    // Corregir distorsion
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                            getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, temp.size(), 1, temp.size(), 0),
                            temp.size(), CV_16SC2, map1, map2);
    
    remap(temp, view, map1, map2, INTER_LINEAR);
    resize(temp,temp,s,0,0);
    resize(view,view,s,0,0);
    imshow("antes",temp);
    imshow("despues",view);
    waitKey();
    destroyAllWindows();
}

// ----------------------------------------------------------------------------
vector<Mat> capturarFotos()
// ----------------------------------------------------------------------------
{
    bool fin = false;
    int tecla;
    Mat ventana;
    vector<Mat> fotos;
    VideoCapture objetivo(0);
    if(!objetivo.isOpened()) return fotos;
    
    // Mensaje informativo de uso
    cout << " Capturando de la camara: f para tomar foto, s para salir.\n";
    
    while (!fin ) {
        // Muestra el streaming en vivo de la camara de fotos
        objetivo >> ventana;
        imshow("Objetivo", ventana);
        
        tecla = waitKey(30);
        if (tecla == 'f'){
            Mat img = ventana.clone();
            resize(img, img, Size(512,384), 0, 0, INTER_CUBIC);
            fotos.push_back(img);
            cout << " Foto tomada correctamente.\n";
        }
        else if (tecla == 's') fin = true;
    }
    destroyAllWindows();
    return fotos;
}

// -----------------------------------------------------------------------------
int main() {
// -----------------------------------------------------------------------------
    showMenu();
    string opcion;
    char directorio[50],inlier[2];
    bool flag = false;
    
    cin >> opcion;
    while (opcion.compare("4")) {
        if (!opcion.compare("1")) {
            getDir(directorio,inlier);
            //calibrar();
            if (!strcmp(inlier, "s")) {
                flag = true;
            }
            vector<Mat> imagenes = procesarDirectorio(directorio);
            printf("Tiempo de calculo total = %f segundos.\n", (double)generarPanorama(imagenes,flag));
            waitKey(0);
            destroyAllWindows();
        } else if (!opcion.compare("2")) {
            vector<Mat> imagenes = capturarFotos();
            printf("Tiempo de calculo total = %f segundos.\n", (double)generarPanorama(imagenes,flag));
            waitKey(0);
            destroyAllWindows();
        }
        showMenu();
        cin >> opcion;
    }
}
