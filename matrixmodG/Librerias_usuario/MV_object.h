#ifndef MATRICES_LIB_H
#define MATRICES_LIB_H

#include <stdio.h> 
#include <stdlib.h>
#include <linux/string.h>
//#include <linux/vmalloc.h>
//#include <linux/slab.h>

#define FILE_SIZE_MAX 	(2000*5) /* 5 cifras por 2000 caracteres (1000 de espacios y 1000 de datos de gasta 5 cifras). Se considera una linea de archivo.*/
#define ALLOC_OK 		1
#define ALLOC_ERROR 	-1
#define FREE_OK 		1
#define FREE_ERROR 		-1
#define CREATE_OK 		1
#define CREATE_ERROR 	-1
#define VALID_MODE 		1
#define INVALID_MODE 	-1
#define N_NAME			128		/* Numero de bytes configurables en una cadena que muestra nombres. */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE TIPOS DE DATOS Y ESTRUCTURAS OPACAS MANEJADAS POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct matrix_struct matrix_o;
typedef struct vector_struct vector_o;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * MEM_ALLOC_MODE: Enumerador de los modos en que se puede reservar memoria dinamicamente para una matriz matrix_o.
 * Por defecto el modo de reserva de memoria es malloc().
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	M_INIT = 1,		/**< Indica el numero inicial de los modos posibles en la enumeracion. Coincide siempre con el primer modos de la enum. */
	M_MALLOC = 1, 	/**< Indica que la reserva de memoria se realiza por la funcion malloc(). */
  	M_CALLOC,		/**< Indica que la reserva de memoria se realiza por la funcion calloc(). */
  	M_END 			/**< Indica el fin de los modos posibles en la enumeracion. */

}MEM_ALLOC_MODE;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ST_MESSAGES: Enumerador de los estados de los mensajes de detalle brindados por libreria MV_object.h
 * Por defecto se inicia la variable asociada MV_OBJECT_MESSAGES en estado MESSAGES_OFF.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	MESSAGES_OFF = 0,	/**< Indica que los mensajes brindados por la libreria estan deshabilitados. */
	MESSAGES_ON = 1 	/**< Indica que los mensajes brindados por la libreria estan habilitados. */
}ST_MESSAGES;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE LOS OBJETOS MANEJADOS POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * OBJ_CREATION_MARK: Enumerador de los estados de un objeto al momento de su creacion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
   OBJ_NO_CREATED = 0, 	/**< El objeto no esta creado.*/
   OBJ_CREATED 		  	/**< El objeto esta creado.*/

} OBJ_CREATION_MARK;


/* Estructura vtable para clase de objeto matrix_o */
struct matrix_methods
{
	int (*alloc_matrix_mem)(matrix_o *);
	int (*free_matrix_mem)(matrix_o *);
	void (*set_mem_alloc_mode)(matrix_o *, int);
	int (*create_zero_matrix)(matrix_o *,int,int);
	int (*create_one_matrix)(matrix_o *,int,int);
	int (*create_identity_matrix)(matrix_o *,int);
	void (*clean_matrix)(matrix_o *);
	void (*negate_matrix)(matrix_o *);
	void (*import_matrix)(matrix_o *,char *);
};


/* Estructura vtable para clase de objeto vector_o */
struct vector_methods
{
	int (*alloc_vector_mem)(vector_o *);
	int (*free_vector_mem)(vector_o *);
	void (*set_mem_alloc_mode)(vector_o *, int);
	int (*create_zero_vector)(vector_o *,int);
	int (*create_one_vector)(vector_o *,int);
	void (*clean_vector)(vector_o *);
	void (*negate_vector)(vector_o *);
	void (*LoadLiteral_vector)(vector_o *,int);
};


/* Estructura de objeto matrix_o utilizado por libreria */
struct matrix_struct
{
	int **matriz;
	int filas;
	int columnas;
	short int obj_id;
	size_t tam_bytes; 		/* Tamaño en bytes */
	char name[N_NAME];
	OBJ_CREATION_MARK mc; 	/* Marca de creacion de objeto matrix_o. 1: indica una matriz creada. 0: indica que la matriz no se creo. */
	MEM_ALLOC_MODE alloc_mode;

	/* Funciones de matrix_o*/
	const struct matrix_methods *mo_methods;
};


/* Estructura de objeto vector_o utilizado por libreria */
struct vector_struct
{
	int *vector;
	int elements;
	short int obj_id;
	size_t tam_bytes; 		/* Tamaño en bytes */
	char name[N_NAME];
	OBJ_CREATION_MARK mc; 	/* Marca de creacion de objeto vector_o. 1: indica un vector creado. 0: indica que el vector no se creo. */
	MEM_ALLOC_MODE alloc_mode;

	/* Funciones de vector_o*/
	const struct vector_methods *vo_methods;
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES ASOCIADAS A OBJETO matrix_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void new_matrix(matrix_o *p_mo, char *p_name);
static int alloc_matrix_mem(matrix_o *p_mo);
int free_matrix_mem(matrix_o *p_mo);
void set_Mmem_alloc_mode(matrix_o *p_mo, int p_mode);
static int valid_alloc_mode(int p_mode);
static void rollback_alloc(int **p_m, size_t pos_fail, int p_mode);
int crear_matriz_cero(matrix_o *p_mo, int p_filas, int p_columnas);
int crear_matriz_uno(matrix_o *p_mo, int p_filas, int p_columnas);
int crear_matriz_identidad(matrix_o *p_mo, int p_dimension);
void limpiar_matriz(matrix_o *p_mo);
void negar_matriz(matrix_o *p_mo);
void importar_matriz(matrix_o *p_mo, char *pfname);
void leer_fc_matriz(matrix_o *p_mo, char *pfname);
static void rellenar_matriz(int **p_m, int p_filas, int p_columnas, int p_valor);

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES ASOCIADAS A OBJETO vector_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void new_vector(vector_o *p_vo, char * p_name);
static int alloc_vector_mem(vector_o *p_vo);
int free_vector_mem(vector_o *p_vo);
void set_Vmem_alloc_mode(vector_o *p_vo, int p_mode);
int crear_vector_cero(vector_o *p_vo, int p_elements);
int crear_vector_uno(vector_o *p_vo, int p_elements);
void limpiar_vector(vector_o *p_vo);
void negar_vector(vector_o *p_vo);
extern void rellenar_vector(int *p_vo, int p_elements, int p_valor);
void CargarLiteral_vector(vector_o *p_vo, int p_literal);


#endif /* MATRICES_LIB_H */
