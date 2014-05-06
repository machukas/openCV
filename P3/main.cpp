//
//  main.cpp
//  Vision por computador
//
//  Created by Nicolas Landa Tejero-Garces on 05/05/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SURF_descriptor <img1> <img2>" << std::endl; }


// ----------------------------------------------------------------------------
Size2i obtenerTamano(Mat foto, Mat acoplada, const Mat Homografia, float *desp_x, float *desp_y)
// ----------------------------------------------------------------------------
{
	Size tam_acoplada = acoplada.size();
	Size tam_referencia = foto.size();
    
	// Obtenemos los puntos de la foto actual en escena
	vector<Point2f> esq_acoplada(4);
	esq_acoplada[0] = Point(0,0);
	esq_acoplada[1] = Point(tam_acoplada.width,0);
	esq_acoplada[2] = Point(tam_acoplada.width,tam_acoplada.height);
	esq_acoplada[3] = Point(0,tam_acoplada.height);
	const cv::Mat mat_acoplada = cv::Mat(esq_acoplada);
    
	// Se calculan los puntos transformados con la homografia
	cv::Mat mat_transf;
	cv::perspectiveTransform(mat_acoplada,mat_transf,Homografia);
    
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
Mat unirImagenes(Mat img_1, Mat img_2, Mat Homografia)
// ----------------------------------------------------------------------------
{
	Mat transformada;
	float desp_x, desp_y;
	// Se obtiene el tamano y se calcula el desplazamiento de las fotos
	Size2i tam = obtenerTamano(img_1,img_2,Homografia, &desp_x, &desp_y);
    
	// Se transforma la imagen mediante la homografia
	// y la matriz de desplazamiento M
	Mat M = (Mat_<double>(3,3) << 1, 0, desp_x, 0, 1, desp_y, 0, 0, 1);
	Homografia = M * Homografia;
	warpPerspective(img_1, transformada, M, tam, INTER_CUBIC);
	warpPerspective(img_2, transformada, Homografia, tam , INTER_CUBIC, BORDER_TRANSPARENT);
    
	return transformada;
}



/**
 *
 */
int main( int argc, char** argv )
{
    if( argc != 3 ) {
        readme();
        return -1;
    }
    
    Mat img_1 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
    Mat img_2 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	
    
    if( !img_1.data || !img_2.data )
    { readme(); return -1; }
	
    // Reduce las imagenes de tamano
    resize(img_1, img_1, Size(1024, 768), 0, 0, INTER_CUBIC);
    resize(img_2, img_2, Size(1024, 768), 0, 0, INTER_CUBIC);
    
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    
    SurfFeatureDetector detector( minHessian );
    
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    
    detector.detect( img_1, keypoints_1 );
    detector.detect( img_2, keypoints_2 );
    
    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    
    Mat descriptors_1, descriptors_2;
    
    extractor.compute( img_1, keypoints_1, descriptors_1 );
    extractor.compute( img_2, keypoints_2, descriptors_2 );
    
    //-- Step 3: Matching descriptor vectors with a brute force matcher
    BFMatcher matcher(NORM_L2);
    std::vector< std::vector<DMatch> > matches;
    matcher.knnMatch(descriptors_1, descriptors_2, matches, 2);
	
	/*
	 *An alternate method of determining high-quality feature matches is the ratio test proposed by David Lowe in his paper on SIFT (page 20 for an explanation). This test rejects poor matches by computing the ratio between the best and second-best match. If the ratio is below some threshold, the match is discarded as being low-quality.
	 */
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
    
    Mat H = findHomography( obj, scene, CV_RANSAC );
    
    Mat result =unirImagenes(img_2,img_1,H);
    /*
    Mat result;
    warpPerspective(img_1, result, H, Size(img_1.cols+img_2.cols,img_2.rows));
    Mat half(result,Rect(0,0,img_2.cols,img_2.rows));
    img_2.copyTo(half);
    */
    
    //-- Show detected matches
    namedWindow("Result");
    //resizeWindow("Result", 4000, 4000);
    imshow( "Result", result );

    waitKey(0);
    
    return 0;
}


