// ****************************************************************************
// INTELIGENCIA ARTIFICIAL E INGENIERIA DEL CONOCIMIENTO 2
// Trabajo final de Vision y Reconocimiento
//
// - Autores: Adam Barreiro (597688) y Carlos Bello (589262)
// - Descripcion: Programa con menu interactivo que genera un panorama
//                con varias imagenes introducidas a mano o fotografiadas
//                con la camara en vivo.
// - Fecha: 30/05/2012
//
// ****************************************************************************

// ----------------------------------------------------------------------------
// Librerias
// ----------------------------------------------------------------------------
#include <iostream>
#include "panorama.hpp"

// ----------------------------------------------------------------------------
// Definiciones y espacios de nombres
// ----------------------------------------------------------------------------
using namespace std;

// ----------------------------------------------------------------------------
// Declaraciones
// ----------------------------------------------------------------------------
void mostrarMenu();

// ----------------------------------------------------------------------------
// Programa principal
// ----------------------------------------------------------------------------
int main(){

	Panorama p;
	string opcion, nombreImagen, guarda;
	int contador;
	bool fin_programa, fin_opcion;

	p.initFlagParejas();
	mostrarMenu();
	fin_programa = false;
	while (!fin_programa){
		fin_opcion = false;
		cout << ">> ";
		cin >> opcion;

		// ------------------------------------------------------------------------------
		// Se parsean los comandos
		// ------------------------------------------------------------------------------

		// Opcion 1: Introducir nombre de fotos
		if (!opcion.compare("1")){
			p.setCamara(false);
			contador = 1;
			cout << "\t(i) INFO: Introduzca el nombre de una foto y pulse enter.\n\t          Escriba START para comenzar" << endl;
			while(!fin_opcion){
				cout << "  - Foto " << contador << ": " ;
				cin >> nombreImagen;
				if (!nombreImagen.compare("START") || !nombreImagen.compare("start")) fin_opcion = true;
				else{
					if (p.importarImagen(nombreImagen)) contador++;
					else cerr << "\t(!) ERROR: No se ha encontrado la imagen especificada." << endl;
				}
			}
			cout << "\t(i) INFO: Realizando el panorama, por favor, espere..." << endl;
			p.ejecutarPanoramaDisco();
			cout << "\t(i) INFO: Panorama realizado con exito." << endl;
			cout << endl << "\t(*) Escriba 'GUARDAR' para guardar la imagen en disco: ";
			cin >> guarda;
			if (!guarda.compare("GUARDAR") || !guarda.compare("guardar")){
				cout << "\t          Introduzca nombre de archivo (sin extension): ";
				cin >> guarda;
				if (guarda.compare(""))
					p.guardarPanorama(guarda);
				else{
					cerr << "\t(!) ERROR: Nombre invalido. No se ha guardado el panorama." << endl;
				}
			}
		}
		// Opcion 2: Realizar fotos con camara
		else if (!opcion.compare("2")){
			p.setCamara(true);
			cout << "\t(i) INFO: Iniciando camara, espere por favor..." << endl;
			if (!p.tomarFotos()) cerr << "\t(!) ERROR: Ocurrio un error con la camara. Vuelva a intentarlo" << endl;
			else{
				cout << "\t(i) INFO: Panorama realizado con exito." << endl;
				cout << endl << "\t(*) Escriba 'GUARDAR' para guardar la imagen en disco: ";
				cin >> guarda;
				if (!guarda.compare("GUARDAR") || !guarda.compare("guardar")){
					cout << "\t          Introduzca nombre de archivo (sin extension): ";
					cin >> guarda;
					if (guarda.compare(""))
						p.guardarPanorama(guarda);
					else{
						cerr << "\t(!) ERROR: Nombre invalido. No se ha guardado el panorama." << endl;
					}
				}
			}
			fin_opcion = true;
		}
		// Opcion 3: Activar/Desactivar flags para mostrar emparejamientos
		else if (!opcion.compare("3")){
			fin_opcion = true;
			if (p.cambiarFlagParejas())
				cout << "\t(i) INFO: Muestras ACTIVADAS.\n\t          Cuando realice el panorama le seran mostrados los emparejamientos." << endl;
			else
				cout << "\t(i) INFO: Muestras DESACTIVADAS.\n\t          Cuando realice el panorama no se mostraran los emparejamientos." << endl;
		}
		// Opcion 4: Salir del programa
		else if (!opcion.compare("4")){
			fin_programa = true;
			cout << "\t(i) INFO: Saliendo... Vuelva pronto :)" << endl;
		}
		else{
			cerr << "\t(!) ERROR: Opcion incorrecta" << endl;
		}
	}
	exit(0);
}


// ----------------------------------------------------------------------------
void mostrarMenu(){
// ----------------------------------------------------------------------------
	cout << endl;
	cout << ".---------------------------------------------." << endl;
	cout << "|                 PANORAMA v1.1               |" << endl;
	cout << "| - - - - - - - - - - - - - - - - - - - - - - |" << endl;
	cout << "|  Adam Barreiro Costa - Carlos Bello Gimeno  |" << endl;
	cout << "|---------------------------------------------." << endl;
	cout << "|                     MENU                    |" << endl;
	cout << "| 1. Introducir fotos a mano una a una.       |" << endl;
	cout << "| 2. Sacar fotos en vivo.                     |" << endl;
	cout << "| 3. Activar/Desactivar la muestra de         |" << endl;
	cout << "|    emparejamientos (activada por defecto).  |" << endl;
	cout << "| 4. Salir.                                   |" << endl;
	cout << ".---------------------------------------------." << endl;
}
