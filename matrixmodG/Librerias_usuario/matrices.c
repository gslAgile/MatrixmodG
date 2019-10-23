#include "matrices.h"


/* Implementacion de funciones */

/*
* Descripcion: Se libera memoria de una matriz, en base al parametro recibido.
* Parametros:
* @param *x: puntero de una estructura de tipo matriz que contiene el dato de las filas
* 			 y columnas de matriz a crear.
*
* @return: la direccion del doble puntero donde se encuentra la matriz creada.
*/
void liberar_mem(struct matriz *x)
{	
	int i = 0;
	if(x->filas > 0 || x->columnas > 0) {
	// Reserva de memoria para columnas de m
	for (i = 0; i<x->filas; i++) 
		free(x->matriz[i]);

	// Liberar memoria para filas m
	free(x->matriz);
	printf("Se libero memoria utilizada en matriz.\n");
	x->filas = 0;
	x->columnas = 0;
	}
	else
		printf("No se pudo liberar memoria. La matriz que desea eliminar no fue creada.\n");
}

/*
* Descripcion de funcion: 
* Parametros:
* @param *x:
*/
void leer_fc_matriz(struct matriz *x, char *pfname)
{
	FILE *fp;
    char buffer[TAM]; // Arreglo que almacenara los datos del archivo leido (fn)
	char *fname = pfname;
	int filas = 0; //
	int columnas = 0; //
	char *aux;
    	
    fp= fopen(fname,"r");// se abre archivo de nombre fname
   	if( !fp ){
      	printf("\n   --> Error al intetar abrir %s \n\n   --> Finalizando programa.\n\n", fname);
      	exit(1);
	}
	
   	printf( "\n   --> Leyendo filas y columnas de matriz de incidencia (%s)...\n", fname);
	
	/* Leer linea por linea del contenido del archivo(fn) en el buffer */
   	while (fgets(buffer, TAM, fp) != NULL)
		filas++;

   	fclose(fp); // cierra archivo	
	
	aux = strtok(buffer, " "); // guardamos primera linea
	columnas = 1;

	while( (aux = strtok( NULL, " ")) != NULL ) // Posteriores llamadas
	 columnas++;
	
	x->filas = filas;
	x->columnas = columnas;
}


/*
* Descripcion de funcion: 
* Parametros:
* @param **x: Doble puntero de matriz de incidencia.
*/
void cargar_matriz(struct matriz *x, char *pfname)
{
	// Se determinan filas columnas de matriz desde archivo	
	leer_fc_matriz(x, pfname);
	
	// Con las filas y columnas leidas se crea matriz alojando memoria dinamicamente
	x->matriz = crear_matriz(x);
	
	FILE *fp;
    char buffer[TAM]; // Arreglo que almacenara los datos del archivo leido (fn)
	char *fname = pfname;
	int filas = 0; //
	int columnas = 0; //
	char *aux;
  	
    fp= fopen(fname,"r");// se abre archivo de nombre fname
    if( !fp ) {
	   	printf("Error al intetar abrir %s \n", fname);
	   	exit(1);
	}
	
	/*Leer linea por linea del contenido del archivo(fn) en el buffer */
   	while (fgets(buffer, TAM, fp) != NULL) {
		aux = strtok(buffer, " "); // guardamos primera linea
		x->matriz[filas][columnas] = atoi(aux);
		columnas = 1;
		
		while((aux = strtok( NULL, " ")) != NULL){   // Posteriores llamadas
			x->matriz[filas][columnas] = atoi(aux);
			columnas++;
		}
   		columnas = 0;
		filas++;
   	}
   	fclose(fp); // cierra archivo
	
   	printf("\n   --> Se cargo exitosamente la matriz de incidencia desde (%s).\n", fname);
}


/*
* Descripcion de funcion: 
* Parametros:
* @param **x: Doble puntero de matriz de incidencia.
*/
void cargar_matriz_cero(struct matriz *x, int filas, int columnas)
{
	int i, j;
	// Se determinan filas columnas de matriz desde matriz *y
	x->filas = filas;
	x->columnas = columnas;
	
	// Con las filas y columnas leidas de matriz y, alojando memoria dinamicamente
	x->matriz = crear_matriz(x);
	
	for(i = 0; i<x->filas; i++) {
		for(j = 0; j<x->columnas; j++)
			x->matriz[i][j] = 0;
	}
}


/*
* Descripcion: Se aloja memoria para crear una matriz, en base al parametro recibido.
* Parametros:
* @param *x: puntero de una estructura de tipo matriz que contiene el dato de las filas
* 			 y columnas de matriz a crear.
*
* @return: la direccion del doble puntero donde se encuentra la matriz creada.
*/
int **crear_matriz(struct matriz *x)
{	
	int **m;
	
	int i = 0;
	int j = 0;

	// Reserva de Memoria para filas m
	m = (int **)malloc(x->filas*sizeof(int*));
	
	// Reserva de memoria para columnas de m
	for(i = 0; i<x->filas; i++) 
		m[i] = (int*)malloc(x->columnas*sizeof(int));

	return m;
}