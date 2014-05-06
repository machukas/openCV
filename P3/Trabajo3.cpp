//
//  Trabajo3.cpp
//  Vision por computador
//
//  Created by Adrián Marín Colás on 05/05/14.
//  Copyright (c) 2014 Adrian Marin Colas. All rights reserved.
//

#include "Trabajo3.h"

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
void getDir(char* directorio) {
// -----------------------------------------------------------------------------
    cout << "Introduzca un directorio: >> ";
    cin >> directorio;
}

// -----------------------------------------------------------------------------
vector<Mat> procesarDirectorio(char* directorio) {
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
            resize(img, img, Size(1024, 768), 0, 0, INTER_CUBIC);
            fotos.push_back(img);
            printf("Se anyade %s\n",ruta);
        }
    }
    closedir (dir);
    return fotos;
}

// -----------------------------------------------------------------------------
void generarPanorama(vector<Mat>* imagenes) {
// -----------------------------------------------------------------------------
    if (imagenes->size() != 0){
        
		for (int i=1; i < imagenes->size(); i++){
			
			
			//Mat Homografia = calcularHomografia(coincidencias, 0, i,false,&aux);
			//coleccionImagenes.at(0) = unirImagenes(Homografia, 0, i);
			
		}
		
    } else {
        cerr << "El directorio no contiene imagenes";
    }
}

// -----------------------------------------------------------------------------
int main() {
// -----------------------------------------------------------------------------
    showMenu();
    string opcion;
    char directorio[50];
    
    cin >> opcion;
    while (opcion.compare("4")) {
        if (!opcion.compare("1")) {
            getDir(directorio);
            vector<Mat> imagenes = procesarDirectorio(directorio);
            generarPanorama(&imagenes);
        } else if (!opcion.compare("2")) {
            // En vivo
        }
        showMenu();
        cin >> opcion;
    }
}
