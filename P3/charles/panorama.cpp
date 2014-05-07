// ****************************************************************************
// INTELIGENCIA ARTIFICIAL E INGENIERIA DEL CONOCIMIENTO 2
// Trabajo final de Vision y Reconocimiento
//
// - Autores: Adam Barreiro (597688) y Carlos Bello (589262)
// - Descripcion: Implementacion de las funciones necesarias para generar
//                una imagen panoramica de una coleccion de fotos.
// - Fecha: 30/05/2012
//
// ****************************************************************************

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "panorama.hpp"

// ----------------------------------------------------------------------------
// Definiciones y espacios de nombres
// ----------------------------------------------------------------------------
using namespace cv;
using namespace flann;

// ----------------------------------------------------------------------------
void Panorama::setCamara(bool val)
// ----------------------------------------------------------------------------
{
	desde_camara = val;
}

// ----------------------------------------------------------------------------
bool Panorama::importarImagen(string nombreArchivo)
// ----------------------------------------------------------------------------
{
	Mat ImgRGB = imread(nombreArchivo.c_str(), 1);

	if(!ImgRGB.data) return false;
	else{
		coleccionImagenes.push_back(ImgRGB);
		return true;
	}
}

// ----------------------------------------------------------------------------
bool Panorama::tomarFotos()
// ----------------------------------------------------------------------------
{
	bool fin = false;
	int numeroFotos = 0;
	int disparador;
	Mat ventana;
	coleccionImagenes.resize(2);

	// Se inicia la camara disponible. Si no se consigue, devuelve falso
	VideoCapture objetivo(0);
	if(!objetivo.isOpened()) return false;

	// Mensaje informativo de uso
	cout << "\t(i) INFO: Camara en funcionamiento." << endl;
	cout << "\t          Para tomar una foto pulse ESPACIO." << endl;
	cout << "\t          Para comenzar pulse ENTER." << endl;

	while (!fin ) {
		// Muestra el streaming en vivo de la camara de fotos
		objetivo >> ventana;
		imshow("Objetivo", ventana);

		disparador = waitKey(10);
		if (disparador == SPACEBAR){
			// Se toma la primera foto
			if (numeroFotos == 0){
				coleccionImagenes[0] = (ventana.clone());
				cout << "\t(i) INFO: Foto tomada e incorporada correctamente." << endl;
				numeroFotos++;
			}
			// Se toma el resto de fotos
			else{
				coleccionImagenes[1] = (ventana.clone());
				cout << "\t(i) INFO: Foto tomada e incorporada correctamente." << endl;
				ejecutarPanoramaVivo();
			}
		}
		else if (disparador == ENTER) fin = true;
	}
	// Si solo hay una foto, el panorama es esa foto
	if (numeroFotos == 1) panorama = coleccionImagenes[0];
	// Si no hay fotos no se muestra nada
	else if (numeroFotos == 0){
		panorama = Mat::zeros(Size(600,480), CV_8UC1);
	}
	// Se muestra el panorama resultante
	destroyWindow("Objetivo");
	objetivo.release();
	imshow("Panorama resultante", panorama);
	waitKey(0);
	destroyWindow("Panorama resultante");
	limpiarTodo();
	return true;
}

// ----------------------------------------------------------------------------
void Panorama::initFlagParejas()
// ----------------------------------------------------------------------------
{
	FLAG_mostrar_parejas = true;
}

// ----------------------------------------------------------------------------
bool Panorama::cambiarFlagParejas()
// ----------------------------------------------------------------------------
{
	FLAG_mostrar_parejas = !(FLAG_mostrar_parejas);
	return FLAG_mostrar_parejas;
}

// ----------------------------------------------------------------------------
void Panorama::ejecutarPanoramaDisco()
// ----------------------------------------------------------------------------
 {
	bool aux;

	if (coleccionImagenes.size() != 0){

		// Se reserva el espacio para vectores
		inicializarVectores();
		// Se construye el panorama
		descriptores.at(0) = extraerDescriptoresSURF(0);
		for (unsigned int i=1; i < coleccionImagenes.size(); i++){
			descriptores.at(i) = extraerDescriptoresSURF(i);
			vector<DMatch> coincidencias = emparejarFLANN(0, i);
			Mat Homografia = calcularHomografia(coincidencias, 0, i,false,&aux);
			coleccionImagenes.at(0) = unirImagenes(Homografia, 0, i);
			descriptores.at(0) = extraerDescriptoresSURF(0);
		}
		// Se asigna y se muestra
		panorama = coleccionImagenes.at(0);
		imshow("Panorama resultante", panorama);
		waitKey(0);
		destroyWindow("Panorama resultante");
		limpiarTodo();
	}
	else cerr << "\t(!) ERROR: No hay imagenes disponibles." << endl;
}

// ----------------------------------------------------------------------------
void Panorama::ejecutarPanoramaVivo()
// ----------------------------------------------------------------------------
 {
	bool bien = false;
	if (coleccionImagenes.size() > 1){
		// Se reserva el espacio para vectores
		inicializarVectores();

		// Se construye el panorama
		descriptores.at(0) = extraerDescriptoresSURF(0);
		descriptores.at(1) = extraerDescriptoresSURF(1);
		vector<DMatch> coincidencias = emparejarFLANN(0,1);
		Mat Homografia = calcularHomografia(coincidencias, 0, 1,true,&bien);
		if (bien) coleccionImagenes.at(0) = unirImagenes(Homografia, 0, 1);
	}

	// La foto esta bien tomada y se muestra
	if (bien){
			panorama = coleccionImagenes.at(0);
			imshow("Panorama", panorama);
			waitKey(0);
			destroyWindow("Panorama");
	}
	// La foto no esta bien tomada y se muestra la anterior
	// ya que no se ejecuto unirImagenes.
	else{
		imshow("Panorama hasta ahora", coleccionImagenes.at(0));
		waitKey(0);
		destroyWindow("Panorama hasta ahora");
	}
 }



// ----------------------------------------------------------------------------
Mat Panorama::extraerDescriptoresSURF(unsigned int indice)
// ----------------------------------------------------------------------------
{
	// Imagen
	Mat sinDistorsion;
	Mat imagenGris;
	vector<KeyPoint> puntosClave;
	Mat descriptores;

	/*
	// Manejo de la distorsion de la camara
	Mat Intrinsic, Distortion;
	FileStorage fs("CameraParameters.yml", FileStorage::READ);

	// Leyendo parametros de la camara
	fs["Intrinsic"] >> Intrinsic;
	fs["Distortion"] >> Distortion;

	if(!Intrinsic.data || !Distortion.data)
	{
		cerr << "(!!) ERROR FATAL: Parametros de la camara no encontardos. Saliendo..." << endl;
		exit(-1);
	}

	// Quitando distorsion y convirtiendo a niveles de gris
	if(desde_camara)
	{
		undistort(coleccionImagenes.at(indice),sinDistorsion,Intrinsic,Distortion);
		cvtColor(sinDistorsion,imagenGris,CV_RGB2GRAY);
	}
	else
	*/

	cvtColor(coleccionImagenes.at(indice),imagenGris,CV_RGB2GRAY);

	// Se crea el detector de puntos de interes
	SurfFeatureDetector detector(500); // Se usa una matriz Hessiana de 500

	// Se detectan los puntos
	detector.detect(imagenGris, puntosClave);

	// Se introducen
	puntosClavePanorama[indice] = puntosClave;

	// Se crea el extractor de descriptores
	SurfDescriptorExtractor extractor;

	// Se extraen los descriptores de los puntos.
	extractor.compute(imagenGris,puntosClave, descriptores);

	return descriptores;
}

// ----------------------------------------------------------------------------
vector<DMatch> Panorama::emparejarFLANN(int i, int j)
// ----------------------------------------------------------------------------
{
	int k;

	// Usamos FLANN porque usa kd-trees para emparejar descriptores
	// coincidentes, siendo mas eficiente que una simple fuerza bruta.
	FlannBasedMatcher emparejador;

	// Se emparejan los descriptores de ambas fotos y se guardan en un
	// vector de coincidencias
	vector<DMatch> coincidencias;
	emparejador.match(descriptores.at(j), descriptores.at(i), coincidencias);

	// Calculo rapido de distancia maxima y minima entre puntos clave
	double distancia_max = 0, distancia_min = 100;
	for (k=0; k< descriptores.at(j).rows; k++)
	{
		double distancia = coincidencias[k].distance;
		if (distancia < distancia_min) distancia_min = distancia;
		if (distancia > distancia_max) distancia_max = distancia;
	}

	// De las coincidencias devueltas por el emparejador FLANN, nos quedamos
	// con las buenas haciendo un calculo.
	vector<DMatch> coincidenciasUtiles;
	for(k=0; k<descriptores.at(j).rows; k++)
	{
		if( coincidencias[k].distance <	 3*distancia_min)
		{
			coincidenciasUtiles.push_back( coincidencias[k]);
		}
	}

	return coincidenciasUtiles;
}

// ----------------------------------------------------------------------------
Mat Panorama::calcularHomografia(vector<DMatch> coincidenciasUtiles, int i, int j, bool enVivo, bool *guay)
// ----------------------------------------------------------------------------
{
	unsigned int k;
	vector<Point2f> foto;
	vector<Point2f> referencia;
	*guay = false;

	for(k = 0; k < coincidenciasUtiles.size(); k++ )
	{
		// Obtener los keypoints de las coincidencias utiles
		foto.push_back( puntosClavePanorama.at(j).at(coincidenciasUtiles.at(k).queryIdx).pt );
		referencia.push_back( puntosClavePanorama.at(i).at(coincidenciasUtiles.at(k).trainIdx).pt );
	}

	Mat H;
	if (coincidenciasUtiles.size() > MIN_COINCIDENCIAS)
	{
		// Se crea el vector de estado que contendra los puntos robustos
		vector<uchar> status(coincidenciasUtiles.size());
		H = cv::findHomography( cv::Mat(foto), cv::Mat(referencia), status, CV_RANSAC, 3.0);

		// Se computa el ratio de puntos robustos frente a puntos totales
		int num_inliers = 0;
		unsigned int m;
		for (m = 0; m < coincidenciasUtiles.size(); m++)
		{
				if (status[m] == 1) num_inliers++;
		}
		double inlier_ratio = (double)num_inliers / (double)coincidenciasUtiles.size();
		cout << "\t**** Coincidencias: " << coincidenciasUtiles.size() << ", robustas: " << num_inliers << ". Ratio: " << inlier_ratio << endl;
		// Si el FLAG esta activo, muestra los resultados graficamente. Si no, no.
		if (FLAG_mostrar_parejas){
			vector<DMatch> muestras;
			unsigned int m=0;
			while (m < status.size()){
				if (status.at(m) == 1){
					muestras.push_back(coincidenciasUtiles.at(m));
				}
				m++;
			}
			coincidenciasUtiles.clear();

			Mat img_matches;
			drawMatches(coleccionImagenes.at(j), puntosClavePanorama.at(j), coleccionImagenes.at(i), puntosClavePanorama.at(i),
						muestras, img_matches, Scalar::all(-1), Scalar::all(-1),
						vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

			imshow("Emparejamientos encontrados", img_matches );
			waitKey(0);
			destroyWindow("Emparejamientos encontrados");
		}

		// Si el ratio o el numero de puntos robustos no supera los umbrales,
		// la homografia se descarta.
		if ((inlier_ratio < (double) RATIO_RANSAC) || (num_inliers <= MIN_RANSAC))
		{
			if (enVivo){
				cerr << "\t(!) ERROR: No hay puntos ROBUSTOS suficientes para calcular el panorama." << endl;
				cerr << "\t           Retome la fotografia de nuevo." << endl;
				*guay = false;
			}
			else{
				cerr << "\t(!!) ERROR FATAL: No hay puntos ROBUSTOS suficientes para calcular el panorama." << endl;
				cerr << "\t                  Compruebe la imagen numero " << j+1 << "." << endl;
				exit(-1);
			}
		}
		else
			if (enVivo) *guay = true;

	}
	else
	{
		if (enVivo){
			cerr << "\t(!) ERROR: No hay puntos suficientes para calcular el panorama." << endl;
			cerr << "\t           Retome la fotografia de nuevo." << endl;
			*guay = false;
		}
		else{
			cerr << "\t(!!) ERROR FATAL: No hay puntos suficientes para calcular el panorama." << endl;
			cerr << "\t                  Compruebe la imagen numero " << j+1 << "." << endl;
			exit(-1);
		}
	}
	return H;
}

// ----------------------------------------------------------------------------
Mat Panorama::unirImagenes(Mat Homografia, int i, int j)
// ----------------------------------------------------------------------------
{
	Mat transformada;
	float desp_x, desp_y;
	// Se obtiene el tamano y se calcula el desplazamiento de las fotos
	Size2i tam = obtenerTamano(coleccionImagenes.at(i),coleccionImagenes.at(j),Homografia, &desp_x, &desp_y);

	// Se transforma la imagen mediante la homografia
	// y la matriz de desplazamiento M
	Mat M = (Mat_<double>(3,3) << 1, 0, desp_x, 0, 1, desp_y, 0, 0, 1);
	Homografia = M * Homografia;
	warpPerspective(coleccionImagenes.at(i), transformada, M, tam, INTER_CUBIC);
	warpPerspective(coleccionImagenes.at(j), transformada, Homografia, tam , INTER_CUBIC, BORDER_TRANSPARENT);

	return transformada;
}

// ----------------------------------------------------------------------------
Size2i Panorama::obtenerTamano(Mat foto, Mat acoplada, const Mat Homografia, float *desp_x, float *desp_y)
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
void Panorama::inicializarVectores()
// ----------------------------------------------------------------------------
{
	descriptores.resize(coleccionImagenes.size());
	puntosClavePanorama.resize(coleccionImagenes.size());
}

// ----------------------------------------------------------------------------
void Panorama::limpiarTodo()
// ----------------------------------------------------------------------------
{
	coleccionImagenes.clear();
	for (unsigned int i; i<puntosClavePanorama.size(); i++){
		puntosClavePanorama.at(i).clear();
	}
	puntosClavePanorama.clear();
	descriptores.clear() ;
	FLAG_mostrar_parejas = false;
}

// ----------------------------------------------------------------------------
void Panorama::guardarPanorama(string nombre)
// ----------------------------------------------------------------------------
{
	nombre.append(".jpg");
	imwrite(nombre, panorama);
	cout << "\t(i) INFO: Imagen guardada con exito." << endl;
}
