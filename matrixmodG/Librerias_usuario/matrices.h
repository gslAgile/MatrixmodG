#ifndef MV_OBJLIB_H
#define MV_OBJLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM (1024*10 -1)

// Estructura utilizada por libreria
struct matriz
{
	int **matriz;
	int filas;
	int columnas;
};

// Funciones de libreria
int **crear_matriz(struct matriz *x);
void liberar_mem(struct matriz *x);
void leer_fc_matriz(struct matriz *x, char *pfname);
void cargar_matriz(struct matriz *x, char *pfname);
void cargar_matriz_cero(struct matriz *x, int filas, int columnas);

#endif

