/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria MV_object.h (archivo cabecera - header file).
 * 
 * 
 * Esta libreria contiene las declaraciones y definiciones de las entidades matrices y vectores (matrix_o y vector_o) y sus metodos asociados. 
 * Estas entidades permiten gestionar las matrices y vectores como objetos en el kernel de Linux. 
 * 
 * La libreria gestiona memoria dinamica de manera manual haciendo uso del conjunto de funciones de la familia KMALLOC y VMALLOC, de acuerdo a la
 * configuracion previamente establecida sobre el objeto se utiliza un mecanismo en particular.
 * 
 * La libreria fue testeada por un conjunto de pruebas unitarias e integrales mediante el framework Kernel Test Framework (KTF). Es por lo cual la libreria 
 * durante un proceso de ejecucion de pruebas, hace uso de las librerias KTF y habilita todas las funcionalidades del framework, en caso contrario por
 * defecto, en proceso de ejecucion estandar, se ignoran todas las definiciones de KTF.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef MATRICES_LIB_H
#define MATRICES_LIB_H

#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#define MATRIXMODG_DB_MSG  	0				/* Macro de mensajes de debug para de driver MatrixmodG. 0: Deshailitada, 1: Habilitada. */
#define TAM 				(1024*10 -1)
#define ALLOC_OK 			1
#define ALLOC_ERROR 		-1
#define FREE_OK 			1
#define FREE_ERROR 			-1
#define CREATE_OK 			1
#define CREATE_ERROR 		-1
#define VALID_MODE 			1
#define INVALID_MODE 		-1
#define N_NAME				128		/* Numero de bytes configurables en una cadena que muestra nombres. */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE TIPOS DE DATOS Y ESTRUCTURAS OPACAS MANEJADAS POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct matrix_struct matrix_o;
typedef struct vector_struct vector_o;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * MEM_ALLOC_MODE: Enumerador de los modos en que se puede reservar memoria dinamicamente para una matriz matrix_o.
 * Por defecto el modo de reserva de memoria es kmalloc().
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	M_INIT = 1,		/**< Indica el numero inicial de los modos posibles en la enumeracion. Coincide siempre con el primer modos de la enum. */
	M_KMALLOC = 1, 	/**< Indica que la reserva de memoria se realiza por la funcion kmalloc(). */
	M_KZALLOC,		/**< Indica que la reserva de memoria se realiza por la funcion kzalloc(). */
  	M_KCALLOC,		/**< Indica que la reserva de memoria se realiza por la funcion kcalloc(). */
  	M_VMALLOC,	   	/**< Indica que la reserva de memoria se realiza por la funcion vmalloc(). */
  	M_VZALLOC,	   	/**< Indica que la reserva de memoria se realiza por la funcion vzalloc(). */
  	M_END 			/**< Indica el fin de los modos posibles en la enumeracion. */

}MEM_ALLOC_MODE;


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
	size_t realTam_bytes; 	/* Tamaño real en bytes */
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
	size_t realTam_bytes; 	/* Tamaño real en bytes */
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
