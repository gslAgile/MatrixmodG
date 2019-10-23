/* Esta libreria permite gestionar de manera adecuada el driver MatrixmodG.*/

#ifndef MATRIXMODG_LIB_H
#define MATRIXMODG_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> /* modos de apertura funciones ioctl*/
#include <errno.h>
#include <omp.h>
#include <time.h>
#include "MV_object.h"

#define USR_BUF_SIZE	16384 		/* 2 ^ 14 */
#define N_CMD 			256
#define N_BYTES			10			/* Numero de bytes configurables en una cadena que muestra enteros. */

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE TIPOS DE DATOS Y ESTRUCTURAS OPACAS MANEJADAS POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct DriverRDPG_struct DriverRDPG_o;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 *ENUMERACIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_COMPONENT: Enumerador de los identificadores de los archivos componentes (matrix_o) de una RDPG_o.
 *
 * @note 	Los componentes de una RDPG_o pueden ser vectores o matrices. Se normaliza los identificadores de componentes matriz y vector que 
 * comienzan con mX o vX donde:
 * m: es una letra fija que indica en el nombre de la enumeracion que se asocia a una matriz.
 * v: es una letra fija que indica en el nombre de la enumeracion que se asocia a un vector.
 * X: puede ser cualquier nombre para completar el nombre del componente de matriz o vector.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	ID_COMP_INIT = 0,	/**< Numero que indica el inicio de las enumeraciones en ID_COMPONENT.*/
	mII = 0,		/**< Identificador de la matriz de incidencia I. */
	mIH,			/**< Identificador de la matriz de incidencia H. */
	mIR,			/**< Identificador de la matriz de incidencia R. */
	mIRe,			/**< Identificador de la matriz de incidencia Re. */
	vMI, 			/**< Identificador del vector de marcodo inicial. */
	ID_COMP_END		/**< Numero que indica el fin de las enumeraciones en ID_COMPONENT.*/
}ID_COMPONENT;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_VIEWCOMPONENT: 	Enumerador de los identificadores de los comandos para visualizar informacion de la RDPG o de sus componentes.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	ID_VIEW_INIT = 0,	/**< Numero que indica el inicio de las enumeraciones en ID_COMPONENT.*/
	view_infoName=0,	/**< Identificador de comando que permite ver el nombre de la RDPG gestionada por driver.*/
	view_infoPlaces,	/**< Identificador de comando que permite ver el numero de plazas de la RDPG.*/
	view_infoTransitions,/**< Identificador de comando que permite ver el numero de transiciones. */
	view_infoShots,		/**< Identificador de comando que permite ver el numero de disparos.*/
	view_infoMemory,	/**< Identificador de comando que permite ver la memoria utilizada por RDPG. */
	view_infoComponent,	/**< Identificador de comando que permite ver informacion asociada a componente seleccionado. */
	view_mII,			/**< Identificador de comando que permite ver matriz de incidencia I. */
	view_mIH,			/**< Identificador de comando que permite ver la matriz de incidencia H. */
	view_mIR,			/**< Identificador de comando que permite ver la matriz de incidencia R. */
	view_mIRe,			/**< Identificador de comando que permite ver la matriz de incidencia Re. */
	view_mD, 			/**< Identificador de comando que permite ver la matriz de disparos D. */
	view_vMI, 			/**< Identificador de comando que permite ver el vector de marcodo inicial. */
	view_vMA,			/**< Identificador de comando que permite ver el vector de marcado actual. */
	view_vMN,			/**< Identificador de comando que permite ver el vector de marcado nuevo. */
	view_vQ,			/**< Identificador de comando que permite ver el vector Q. */
	view_vW,			/**< Identificador de comando que permite ver el vector W. */
	view_vE, 			/**< Identificador de comando que permite ver el vector E. */
	view_vB,			/**< Identificador de comando que permite ver e de comando que permite ver el vector B. */
	view_vL,			/**< Identificador de comando que permite ver el vector L. */
	view_vG,			/**< Identificador de comando que permite ver el vector G. */
	view_vA,			/**< Identificador de comando que permite ver el vector A. */
	view_vUDT,			/**< Identificador de comando que permite ver el vector UDT. */
	view_vEx, 			/**< Identificador de comando que permite ver el vector Ex. */
	get_Tokens,			/**< Identificador de comando que permite obtener el numero de tokens de una plaza de la RDPG del kernel. */
	ID_VIEW_END			/**< Numero que indica el fin de las enumeraciones en ID_VIEWCOMPONENT.*/
}ID_VIEWCOMPONENT;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ST_CONNECTION: 	Enumerador de los estados posibles que puede tener un objeto DriverRDPG_o al momento de establecer la coneccion con el driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	DISABLED_CONNECTION=0,	/* La conexion con el driver MatrixmodG no tuvo exito. */
	ENABLED_CONNECTION		/* La conexion con el driver MatrixmodG fue exitosa. */
}ST_CONNECTION;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE LOS OBJETOS MANEJADOS POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Estructura vtable para clase de objeto DriverRDPG_o */
struct DriverRDPG_methods
{
	void (*connect_driverMatrixmodG)(DriverRDPG_o *);
	void (*disconnect_driverMatrixmodG)(DriverRDPG_o *);
	void (*matrixmodG_createRDPG)(DriverRDPG_o *);
	void (*matrixmodG_addm)(DriverRDPG_o *,int);
	void (*matrixmodG_addv)(DriverRDPG_o *,int);
	void (*matrixmodG_delRDPG)(DriverRDPG_o *);
	void (*matrixmodG_confirmRDPG)(DriverRDPG_o *);
	void (*view_RDPGinfo)(DriverRDPG_o *);
	void (*view_allCompRDPG)(DriverRDPG_o *);
	void (*view_compRDPG)(DriverRDPG_o *,int,char *);
	void (*view_allcmdRDPG)(void);
	void (*shoot_RDPG)(DriverRDPG_o *,int);
	void (*delete_MatrixmodGDriver)(DriverRDPG_o *);
	void (*set_vG)(DriverRDPG_o *, char *);
	int (*get_TokensPlace)(DriverRDPG_o *, char *);

	/* Llamadas al sistema sobre driver matrixmodG. */
	int (*write_matrixmodG)(DriverRDPG_o *,char *);
	int (*read_matrixmodG)(DriverRDPG_o *,char *);
};


/* Estructura de objeto DriverRDPG_o utilizado por libreria */
struct DriverRDPG_struct
{
	int my_fd;								/* numero entero que almacena numero de file descriptor del archivo de dispositivo del driver /proc/matrixmod. */
	int connect_driver;
	matrix_o lib_mII;						/* Matriz de incidencia. */
	matrix_o lib_mIH; 						/* Matriz de incidencia H de brazos inhibidores. */
	matrix_o lib_mIR;						/* Matriz de incidencia R de brazos lectores. */
	matrix_o lib_mIRe;						/* Matriz de incidencia Re de transiciones con arcos reset. */
	matrix_o lib_vMI;						/* Marcado inicial actual. */
	OBJ_CREATION_MARK mc; 					/* Marca de creacion de objeto matrix_o. 1: indica una matriz creada. 0: indica que la matriz no se creo. */
	matrix_o *component[ID_COMP_END]; 		/* Tabla indice de todos los componentes matrix_o del objeto DriverRDPG_o. */
	int system_test_mode;
	int LIBMATRIXMODG_MESSAGES; 			/* Mensajes de libreria. Ver valores desde enum ST_MESSAGES de MV_object.h. */
	int LIBMATRIXMODG_MSG_TIMEOP;	 		/* Mensajes de tiempos de operaciones. Ver valores desde enum ST_MESSAGES de MV_object.h. */

	/* Variables para medicion de tiempos. */
	double timeop_omp; 		/* tiempo de operacion desde API omp. */
	double timeop_ltime; 	/* tiempo de operacion desde libreria time. */

	/* Funciones de DriverRDPG_o. */
	const struct DriverRDPG_methods *methods;
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES ASOCIADAS A OBJETO DriverRDPG_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void new_MatrixmodGDriver(DriverRDPG_o *p_DriverObj, char *p_mII, char *p_mIH, char *p_mIR, char *p_mIRe, char *p_vMI);
void delete_MatrixmodGDriver(DriverRDPG_o *p_DriverObj);
void importFiles_RDPG(DriverRDPG_o *p_DriverObj, char *p_mII, char *p_mIH, char *p_mIR, char *p_mIRe, char *p_vMI);
void connect_driverMatrixmodG(DriverRDPG_o *p_DriverObj);
void disconnect_driverMatrixmodG(DriverRDPG_o *p_DriverObj);
void matrixmodG_createRDPG(DriverRDPG_o *p_DriverObj);
void matrixmodG_addm(DriverRDPG_o *p_DriverObj, int p_mcomp);
void matrixmodG_addv(DriverRDPG_o *p_DriverObj, int p_vcomp);
void matrixmodG_delRDPG(DriverRDPG_o *p_DriverObj);
void matrixmodG_confirmRDPG(DriverRDPG_o *p_DriverObj);
void view_RDPGinfo(DriverRDPG_o *p_DriverObj);
void view_allCompRDPG(DriverRDPG_o *p_DriverObj);
void view_compRDPG(DriverRDPG_o *p_DriverObj,int cmd_INFO_ID, char *cadena);
void view_allcmdRDPG(void);
void shoot_RDPG(DriverRDPG_o *p_DriverObj, int p_transicion);
void delete_driverComponent(matrix_o *p_mo);
void index_components(DriverRDPG_o *p_DriverObj);
void set_vG(DriverRDPG_o *p_DriverObj, char *p_entrada);
int get_TokensPlace(DriverRDPG_o *p_DriverObj, char *p_entrada);
void itoa(int p_entero, char *p_str, size_t p_len);


/* Llamadas al sistema sobre driver matrixmodG. */
int write_matrixmodG(DriverRDPG_o *p_DriverObj, char *cadena);
int read_matrixmodG(DriverRDPG_o *p_DriverObj, char *cadena);


#endif /* MATRIXMODG_LIB_H*/
