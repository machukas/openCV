//
//  Trabajo3.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 05/05/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include "Trabajo3.h"

// ----------------------------------------------------------------------------
Size2i obtenerTamano(Mat foto, Mat acoplada, const Mat Homografia, float *desp_x, float *desp_y) {
// ----------------------------------------------------------------------------
    
	Size tam_acoplada = acoplada.size();
	Size tam_referencia = foto.size();
    // acoplada es la foto panoramica
	// Obtenemos los puntos de la foto actual en escena
	vector<Point2f> esq_acoplada(4);
	esq_acoplada[0] = Point(0,0);
	esq_acoplada[1] = Point(tam_acoplada.width,0);
	esq_acoplada[2] = Point(tam_acoplada.width,tam_acoplada.height);
	esq_acoplada[3] = Point(0,tam_acoplada.height);
	// Matriz con el tamano de la panoramica
    const Mat mat_acoplada = Mat(esq_acoplada);
    
	// Se calculan los puntos transformados con la homografia
	Mat mat_transf;
	perspectiveTransform(mat_acoplada,mat_transf,Homografia);
    
    //imshow("mat_transf",mat_transf);
    //waitKey(0);
    
	// Obtenemos los puntos de la foto a colocar
	vector<Point2f> esq_transf(4);
	esq_transf = vector<Point2f>(mat_transf);
    
	// Suponemos por defecto que el tamano del lienzo
	// sera como minimo el de la imagen que tenemos
	float ancho_min=0.0, ancho_max=tam_referencia.width, alto_max=tam_referencia.height, alto_min=0.0;
	*desp_x = 0.0; *desp_y = 0.0;
    
	// Luego solo se comprueba el tamano de la imagen
	// a colocar
	for (unsigned int i=0; i<esq_transf.size(); i++)
	{
		if (esq_transf[i].x < ancho_min){ ancho_min = esq_transf[i].x; *desp_x = abs(ancho_min); }
		if (esq_transf[i].x > ancho_max){	ancho_max = esq_transf[i].x;}
		if (esq_transf[i].y < alto_min){	alto_min = esq_transf[i].y; *desp_y = abs(alto_min); }
		if (esq_transf[i].y > alto_max){ alto_max = esq_transf[i].y;}
	}
	return Size(ancho_max-ancho_min,alto_max-alto_min);
}

// ----------------------------------------------------------------------------
Mat unirImagenes(Mat img_1, Mat img_2, Mat Homografia) {
// ----------------------------------------------------------------------------

	Mat transformada;
	float desp_x, desp_y;
	// Se obtiene el tamano y se calcula el desplazamiento de las fotos
	Size2i tam = obtenerTamano(img_1,img_2,Homografia, &desp_x, &desp_y);
    
	// Se transforma la imagen mediante la homografia
	// y la matriz de desplazamiento M
	Mat M = (Mat_<double>(3,3) << 1, 0, desp_x, 0, 1, desp_y, 0, 0, 1);
	Homografia = M * Homografia;
	warpPerspective(img_2, transformada, Homografia, tam, INTER_CUBIC);
	warpPerspective(img_1, transformada, M, tam , INTER_CUBIC, BORDER_TRANSPARENT);
    
	return transformada;
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
    
    while ((ent = readdir (dir)) != NULL)
    {
        strcpy(ruta, directorio);
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
        {
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            Mat img = imread( strcat(ruta,ent->d_name),CV_LOAD_IMAGE_GRAYSCALE);
            resize(img, img, Size(256,192), 0, 0, INTER_CUBIC);
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
    Mat img_1,img_2,result;
    for (int i = 1; i<imagenes.size(); i++) {
        
        t = (double) getTickCount();
        
        img_1 = imagenes.at(0);
        img_2 = imagenes.at(i);
        
        //-- Step 1: Detect the keypoints using SURF Detector
        int minHessian = 400;
        SiftFeatureDetector detector( minHessian );
        
        vector<KeyPoint> keypoints_1, keypoints_2;
        
        detector.detect( img_1, keypoints_1 );
        detector.detect( img_2, keypoints_2 );
        
        //-- Step 2: Calculate descriptors (feature vectors)
        SiftDescriptorExtractor extractor;
        
        Mat descriptors_1, descriptors_2;
        
        extractor.compute( img_1, keypoints_1, descriptors_1 );
        extractor.compute( img_2, keypoints_2, descriptors_2 );
        
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
        Mat H = findHomography( obj, scene, CV_RANSAC,3,mask);
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
        
        result =unirImagenes(img_2,img_1,H);

        imagenes[0] = result;
        t = ((double)getTickCount()-t)/getTickFrequency();
        total+=t;
        printf("Tiempo de calculo = %f segundos.\n", (double)t);
    }
    return total;
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
            if (!strcmp(inlier, "s")) {
                flag = true;
            }
            vector<Mat> imagenes = procesarDirectorio(directorio);
            printf("Tiempo de calculo total = %f segundos.\n", (double)generarPanorama(imagenes,flag));
            waitKey(0);
            destroyAllWindows();
        } else if (!opcion.compare("2")) {
            // En vivo
        }
        showMenu();
        cin >> opcion;
    }
}
