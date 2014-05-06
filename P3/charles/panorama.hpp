// ****************************************************************************
// INTELIGENCIA ARTIFICIAL E INGENIERIA DEL CONOCIMIENTO 2
// Trabajo final de Vision y Reconocimiento
//
// - Autores: Adam Barreiro (597688) y Carlos Bello (589262)
// - Descripcion: Cabeceras y declaraciones de las funciones necesarias para generar
//                una imagen panoramica de una coleccion de fotos.
// - Fecha: 30/05/2012
//
// ****************************************************************************

// ----------------------------------------------------------------------------
// Librerias
// ----------------------------------------------------------------------------
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>

// ----------------------------------------------------------------------------
// Definiciones y espacios de nombres
// ----------------------------------------------------------------------------
#ifndef PANORAMA_H_
#define PANORAMA_H_
#define SPACEBAR 32
#define ENTER 13
#define MIN_COINCIDENCIAS 24
#define RATIO_RANSAC 0.3
#define MIN_RANSAC 16
using namespace std;
using namespace cv;

// ----------------------------------------------------------------------------
// Codigo
// ----------------------------------------------------------------------------
class Panorama{

public:

	// ------------------------------------------------------------------------
	void setCamara(bool val);
	// ------------------------------------------------------------------------
	/* Establece el valor de 'desde_camara' al valor pasado como parametro
	 * para indicar con TRUE que las fotos son tomadas en vivo y con
	 * FALSE que las fotos estan guardadas en disco.
	 */

	// ------------------------------------------------------------------------
	bool importarImagen(string nombreArchivo);
	// ------------------------------------------------------------------------
	/* Importa una imagen procedente del disco duro a la coleccion de
	 * imagenes usadas para el panorama. Devuelve TRUE si hubo exito
	 * en la importacion
	 */

	// ------------------------------------------------------------------------
	bool tomarFotos();
	// ------------------------------------------------------------------------
	/* Toma fotos en vivo con la camara que este disponible y las importa
	 * a la coleccion de imagenes usadas para el panorama. Devuelve TRUE si
	 * la operacion ha sido exitosa.
	 */

	// ----------------------------------------------------------------------------
	void initFlagParejas();
	// ----------------------------------------------------------------------------
	/* Asigna TRUE por defecto al FLAG que hace que muestre los emparejamientos
	 * entre fotos.
	 */

	// ------------------------------------------------------------------------
	bool cambiarFlagParejas();
	// ------------------------------------------------------------------------
	/* Cambia el FLAG que hace que muestre los emparejamientos entre fotos
	 * al realizar el panorama. Devuelve su nuevo valor.
	 */

	// ------------------------------------------------------------------------
	void ejecutarPanoramaDisco();
	// ------------------------------------------------------------------------
	/* Realiza todas las operaciones necesarias sobre la coleccion de imagenes
	 * (importadas desde el disco duro) para fabricar un panorama y mostrarlo.
	 */

	// ------------------------------------------------------------------------
	void guardarPanorama(string nombre);
	// ------------------------------------------------------------------------
	/* Guarda la imagen panoramica en disco
	 */

private:

	// ------------------------------------------------------------------------
	// Atributos
	// ------------------------------------------------------------------------
	vector<Mat> coleccionImagenes;
	vector<vector <KeyPoint> > puntosClavePanorama;
	vector<Mat> descriptores;
	Mat panorama;
	bool FLAG_mostrar_parejas;
	bool desde_camara;

	// ------------------------------------------------------------------------
	Mat extraerDescriptoresSURF(unsigned int indice);
	// ------------------------------------------------------------------------
	/* Extrae puntos de interes en una imagen dada, y calcula sus
	 * descriptores mediante el algoritmo SURF.
	 */

	// ------------------------------------------------------------------------
	vector<DMatch> emparejarFLANN(int i, int j);
	// ------------------------------------------------------------------------
	/* Empareja los descriptores de dos imagenes usando el algoritmo FLANN.
	 */

	// ------------------------------------------------------------------------
	Mat calcularHomografia(vector<DMatch> coincidenciasUtiles, int i, int j , bool enVivo, bool* guay);
	// ------------------------------------------------------------------------
	/* Calcula la homografia de dos imagenes en base a sus coincidencias y
	 * sus puntos clave o de interes. Si enVivo es TRUE significa que las fotos
	 * son tomadas por camara y se actualizara el valor de guay a TRUE o FALSE
	 * si la homografia se ha realizado correctamente o no.
	 */

	// ------------------------------------------------------------------------
	Mat unirImagenes(Mat Homografia, int i, int j);
	// ------------------------------------------------------------------------
	/* Une dos imagenes dadas mediante la matriz de la homografia
	 */

	// ------------------------------------------------------------------------
	void ejecutarPanoramaVivo();
	// ------------------------------------------------------------------------
	/* Realiza todas las operaciones necesarias sobre la coleccion de imagenes
	 * (tomadas desde camara) para fabricar un panorama y mostrarlo.
	 * Devuelve el numero de imagenes que han intervenido.
	 */

	// ------------------------------------------------------------------------
	Size2i obtenerTamano(Mat foto, Mat acoplada, Mat H, float *desp_x, float *desp_y);
	// ------------------------------------------------------------------------
	/* Vacia todas las estructuras de imagenes
	 */

	// ------------------------------------------------------------------------
	void inicializarVectores();
	// ------------------------------------------------------------------------
	/* Amplia o reduce los vectores de descriptores y puntos clave
	 * dependiendo del tamano de la coleccion de imagenes.
	 */

	// ------------------------------------------------------------------------
	void limpiarTodo();
	// ------------------------------------------------------------------------
	/* Vacia todas las estructuras de imagenes
	 */
};

#endif
