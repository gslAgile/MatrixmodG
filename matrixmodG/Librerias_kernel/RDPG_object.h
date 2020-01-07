/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria RDPG_object.h (archivo cabecera - header file).
 * 
 * 
 * Esta libreria contiene las declaraciones y definiciones de la entidad RDPG_o y sus metodos asociados. Esta entidad permite
 * gestionar las RDPG como objetos en el kernel de Linux. 
 * 
 * La libreria gestiona objetos matrices y vectores declarados y definidos en la libreria MV_object.h/.c, entidades que se utilizan como objetos que componen
 * una RDPG.
 * 
 * La libreria proporciona control de errores de datos y formatos de datos propio determinado por la libreria sformat_control.h/.c.
 * 
 * La libreria provee de un conjunto de metodos de codigo SMPs para proteger las RDPG de los problemas de concurrencia. El mecanismo utilizado para brindar
 * sincronizacion entre procesos concurrentes es mediante spinlocks lector-escritor. Se recomienda hacer uso de estos metodos para evitar errores de datos
 * desde las aplicaciones de usuario.
 * 
 * La libreria fue testeada por un conjunto de pruebas unitarias e integrales mediante el framework Kernel Test Framework (KTF). Es por lo cual la libreria 
 * durante un proceso de ejecucion de pruebas hace uso de las librerias KTF y habilita todas las funcionalidades del framework, en caso contrario por 
 * defecto, en proceso de ejecucion estandar, se ignoran todas las definiciones de KTF.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef RDPG_LIB_H
#define RDPG_LIB_H

#ifdef MT_TEST_ON                 /* Si driver esta en proceso de prueba, se incorporan las funciones de KTF. */
#include "ktf.h"                  /* Inclusion de libreria para la prueba del modulo desde KTF con el archivo de casos de pruebas matrixmod_tests.c */
#else                             /* Si driver no esta en proceso de prueba, se usan funciones copy desde uaccess.h */
#include <asm-generic/uaccess.h>  /* copy_to_user() copy_from_user()*/
#endif

#include "MV_object.h"  		/* Inclusion de libreria que provee el uso de objetos vectores y matrices (vector_o y matrix_o respectivamente). */
#include "sformat_control.h"  	/* Inclusion de libreria con funciones de control del formato de los string ingresador por el usuario. */
#include "mascii.h"  			/* Inclusion de libreria con funciones para gestionar cadenas de caracteres (string) y funciones utiles de conversion como itoa. */
#include <linux/spinlock.h>


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE MACROS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BUFFER_LENGTH_WR   1024 			/* 2 ^ 10 */
#define BUFFER_LENGTH_RD   7000 			/* 2 ^ 13 */
#define VALID_ID		1
#define INVALID_ID	    -1

#define MAX_PLACES		1000	/* Numero maximo de plazas soportadas por driver. */
#define MAX_TRANSITIONS 1000	/* Numero maximo de transiciones soportadas por driver. */
#define N_BYTES			10		/* Numero de bytes configurables en una cadena que muestra enteros. */
#define N_MCOMPONENTS	5  		/* Numero de componetes matrix_o que conforman una RDPG_o. */
#define N_VCOMPONENTS	14  	/* Numero de componetes vector_o que conforman una RDPG_o. */
#define N_HEADER		1024	/* Numero de bytes de las cabeceras de transiciones, plazas y cantidad de disparos. */
#define MAX_VDIM		35		/* Numero entero maximo de plazas y transiciones a visualizar en una lectura de componentes de una RDPG_o. */
#define MIN_VDIM		10		/* Numero entero maximo de plazas y transiciones a visualizar en una lectura de componentes de una RDPG_o. */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE TIPOS DE DATOS Y ESTRUCTURAS OPACAS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef struct RDPG_struct RDPG_o;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_READ_MODE: Enumerador de los modos de lectura de una RDPG_o, la RDP permite ser leida de diferentes formas.
 * Ante una solicitud de lectura al drive, este escribe al espacio usuario cualquiera informacion de la RDP referida a sus componentes o
 * a datos actuales de la RDPG_o cargada en el kernel. Para diferenciar entre las diferentes opciociones de lectura, se debe configurar el 
 * modo de lectura de acuerdo a la siguiente enumeracion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
  ID_MCOMPONENT_MODE = 0,	/**< Modo con el cual el drive escribe al espacio usuario un componente matriz (matrix_o) de la RDPG_o.*/
  ID_VCOMPONENT_MODE,		/**< Modo con el cual el drive escribe al espacio usuario un componente vector (vector_o) de la RDPG_o.*/
  ID_INFO_MODE, 			/**< Este es el modo de lectura de informacion. A partir de este id termina el modo de lectura de componentes.*/
  ID_INFO_NAME,				/**< Modo con el cual el drive escribe al espacio usuario el nombre configurado de la RDPG_o.*/
  ID_INFO_PLACES,			/**< Modo con el cual el drive escribe al espacio usuario el numero de plazas configurado en la RDPG_o.*/
  ID_INFO_TRANSITIONS,		/**< Modo con el cual el drive escribe al espacio usuario el numero de transiciones en la RDPG_o.*/
  ID_INFO_SHOTS,			/**< Modo con el cual el drive escribe al espacio usuario el numero de disparos en la RDPG_o.*/
  ID_INFO_MEMORY,			/**< Modo con el cual el drive escribe al espacio usuario la memoria reservada en la RDPG_o.*/
  ID_INFO_COMP,				/**< Modo con el cual el drive escribe al espacio usuario datos de submode de componente en la RDPG_o (ver ID_READ_SUBMODE).*/
  ID_INFO_SHOT_RESULT,		/**< Modo con el cual el drive escribe al espacio usuario el resultado del ultimo disparo realizado en la RDPG_o.*/
  ID_ERROR_MODE,			/**< Modo con el cual el drive escribe al espacio usuario una cadena de error de lectura en la RDPG_o.*/
  ID_RM_END					/**< Numero que indica el fin de modos para la enumeracion ID_READ_MODE.*/
}ID_READ_MODE;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_READ_SUBMODE: Enumerador de los sub-modos de lectura de una RDP. En el modo ID_INFO_COMP la RDP permite ser leida de diferentes formas.
 * Cuando se configura el modo ID_INFO_COMP en una RDPG_o, se permite dos sub-modos para los componentes a mostrar. Se utilizan los sub-modos
 * para diferenciar los componentes matrix_o de los componentes vector_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
  ID_RSM_INIT = 0,		/**< Numero que indica el comienzo de los submodos dentro de la enumeracion ID_READ_SUBMODE.*/
  ID_MCOMP_SMODE = 0,	/**< Sub-modo con el cual el drive escribe al espacio usuario los datos de un componente matrix_o de la RDPG_o.*/
  ID_VCOMP_SMODE,		/**< Sub-modo con el cual el drive escribe al espacio usuario los datos de un componente vector_o de la RDPG_o.*/
  ID_ERROR_SMODE,		/**< Sub-modo con el cual el drive escribe al espacio usuario una cadena de error de lectura en la RDPG_o.*/
  ID_RSM_END			/**< Numero que indica el fin de los submodos enumerados en ID_READ_SUBMODE.*/
}ID_READ_SUBMODE;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_MCOMPONENT: Enumerador de los identificadores de los componentes matrices (matrix_o) de una RDPG_o.
 *
 * @note 	Los componentes de una RDPG_o pueden ser vectores o matrices. Se normaliza los identificadores de componentes matriz que 
 * comienzan con mX donde:
 * m: es una letra fija que indica en el nombre de la enumeracion que se asocia a una matriz (matrix_o).
 * X: puede ser cualquier nombre para completar el nombre del componente matrix_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	ID_MC_INIT = 0,	/**< Numero que indica el inicio de las enumeraciones en ID_MCOMPONENT.*/
	mII = 0,		/**< Identificador de la matriz de incidencia I. */
	mIH,			/**< Identificador de la matriz de incidencia H. */
	mIR,			/**< Identificador de la matriz de incidencia R. */
	mIRe,			/**< Identificador de la matriz de incidencia Re. */
	mD, 			/**< Identificador de la matriz de disparos D. */
	ID_MC_END		/**< Numero que indica el fin de las enumeraciones en ID_MCOMPONENT.*/
}ID_MCOMPONENT;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_VCOMPONENT: Enumerador de los identificadores de los componentes vectores (vector_o) de una RDPG_o.
 *
 * @note 	Los componentes pueden ser vectores o matrices, Se normaliza los identificadores de componentes vector que 
 * comienzan con vX donde:
 * v: es una letra fija que indica en el nombre de la enumeracion que se asocia a un vector.
 * X: puede ser cualquier nombre para completar el nombre del componente vector_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	ID_VC_INIT = 0, /**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
	vMI = 0,		/**< Identificador del vector de marcado inicial. */
	vMA,			/**< Identificador del vector de marcado actual. */
	vMN,			/**< Identificador del vector de marcado nuevo. */
	vE, 			/**< Identificador del vector E. */
	vQ,				/**< Identificador del vector Q. */
	vW,				/**< Identificador del vector W. */
	vB,				/**< Identificador del vector B. */
	vL,				/**< Identificador del vector L. */
	vG,				/**< Identificador del vector G. */
	vA,				/**< Identificador del vector A. */
	vUDT,			/**< Identificador del vector UDT. */
	vEx, 			/**< Identificador del vector Ex. */
	vHQCV,			/**< Identificador del vector vHQCV . */
	vHD,			/**< Identificador del vector vHD. */
	ID_VC_END		/**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
}ID_VCOMPONENT;	


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * CAT_COMPONENTS: Enumerador de los componentes de una RDPG_o que se pueden leer.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum{
	CAT_INIT = 0, /**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
	cat_mII,			/**< Identificador de lectura de la matriz de incidencia I. */
	cat_mIH,			/**< Identificador de lectura de la matriz de incidencia H. */
	cat_mIR,			/**< Identificador de lectura de la matriz de incidencia R. */
	cat_mIRe,			/**< Identificador de lectura de la matriz de incidencia Re. */
	cat_mD, 			/**< Identificador de lectura de la matriz de disparos D. */
	cat_vMI,			/**< Identificador de lectura de vector de marcado inicial. */
	cat_vMA,			/**< Identificador de lectura de vector de marcado actual. */
	cat_vMN,			/**< Identificador de lectura de vector de marcado nuevo. */
	cat_vE, 			/**< Identificador de lectura de vector E. */
	cat_vQ,				/**< Identificador de lectura de vector Q. */
	cat_vW,				/**< Identificador de lectura de vector W. */
	cat_vB,				/**< Identificador de lectura de vector B. */
	cat_vL,				/**< Identificador de lectura de vector L. */
	cat_vG,				/**< Identificador de lectura de vector G. */
	cat_vA,				/**< Identificador de lectura de vector A. */
	cat_vUDT,			/**< Identificador de lectura de vector UDT. */
	cat_vEx, 			/**< Identificador de lectura de vector Ex. */
	cat_vHQCV, 			/**< Identificador de lectura de vector vHQCV. */
	cat_vHD, 			/**< Identificador de lectura de vector vHD. */
	CAT_END		/**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
}CAT_COMPONENTS;	


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * RDPG_SHOT_RESULT: Enumerador de los resultados que puede devolver el ultimo disparo realizado de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum {
	SHOT_INIT=0,	/* Resultado cuando no se realizaron disparos en una RDPG_o. */
	SHOT_OK = 1, 	/* Resultado cuando el ultimo disparo realizado de una RDPG_o fue exitoso. */
	SHOT_FAILED=-1	/* Resultado cuando el ultimo disparo realizado de una RDPG_o no fue exitoso. */
} RDPG_SHOT_RESULT;


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * SHOT_MODE: Enumerador de los modos en que se puede disparar las transiciones de una red RDPG_o afectada.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum {
	SHOT_MODE_E = 1, 	/* Modo de disparo explicito, con este modo seleccionado en la red RDPG_o afectada, impactan los cambios de un disparo. Los vectores de maracado MN y MA de la red se veran afectados.*/
	SHOT_MODE_I			/* Modo de disparo implicito, con este modo seleccionado en la red RDPG_o afectada, no impactan los cambios de un disparo. Se conoce que sucede al disparar una transicion de la red pero los vectores de maracado MN y MA no se veran afectados.*/
} SHOT_MODE;


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ST_TRANSITION: Enumerador de los estados en los que puede estar una transicion de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum {
	ST_NO_SENSITIZED=0,	/* Estado de transicion no sensibilizada.*/	
	ST_SENSITIZED = 1 	/* Estado de transicion sensibilizada.*/	
} ST_TRANSITION;

/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ERRORS_CODES: Enumerador de los codigos de eerores posibles de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum {
	EC_NULL = 0,				/* Codigo cuando no se produo ningun error. */
	EC_dobleCreacionDeRDPG,		/* Error que se ocaciona cuando se intenta crear dos o mas veces una misma RDPG_o.*/	
	EC_dobleEliminacionDeRDPG,	/* Error que se ocaciona cuando se produce la eliminacion de una RDPG que ya se elimino.*/	
	EC_transicionInexistente,
	EC_falloPosVP,
	EC_falloPosVT,
	EC_falloVdim,
	EC_extraccionDato,
	EC_datoIncorrecto,
	EC_componenteNoCreado,
	EC_CodigoCatComp,
	EC_falloADD
} ERRORS_CODES;


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * GUARD_CONFIG: Enumerador de las configuraciones de guardas en una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
//typedef enum {
//	DISABLE_GUARD=0,	/* Guardas deshabilitadas en RDPG_o.*/	
//	ENABLE_GUARD = 1 	/* Guardas habilitadas en RDPG_o.*/	
//} GUARD_CONFIG;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE ESTRUCTURAS DE RDPG_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Estructura vtable para clase de objetos RDPG_o */
struct RDPG_methods
{
	/* Punteros de funcion */
	void (*create_rdpg)(RDPG_o *,char *);
	void (*delete_rdpg)(RDPG_o *);
	int (*add_value_in_mcomponent)(matrix_o *,char *);
	int (*add_value_in_vcomponent)(vector_o *,char *);
	int (*add_value_in_mIRe)(RDPG_o *, char*);
	int (*add_value_vG)(RDPG_o *,char *);
	void (*update_work_components)(RDPG_o *);
	void (*update_vG)(RDPG_o *);
	void (*load_vcomp_with_vcomp)(RDPG_o *, int, int);
	int (*shoot_rdpg)(RDPG_o *,int, SHOT_MODE);
	int (*read_rdpg_component)(RDPG_o *,char *, int, int);
	int (*read_rdpg_info)(RDPG_o *,char *);
	int (*get_TokensPlace)(RDPG_o *, char *);
	int (*get_vHDelement)(RDPG_o *, char *);

	/* Puntero de funcion de funciones de seteo de campos. */
	void (*set_MemAllocMode)(RDPG_o *, char *);
	void (*set_select_comp)(RDPG_o *, int);
	void (*set_read_mode)(RDPG_o *,ID_READ_MODE);
	void (*set_read_smode)(RDPG_o *, ID_READ_SUBMODE);
	void (*set_posVP)(RDPG_o *,char *);
	void (*set_posVT)(RDPG_o *,char *);
	void (*set_vdim)(RDPG_o *,char *);
	void (*set_catComp)(RDPG_o *,int);
	void (*inc_vHQCV)(RDPG_o *,char *);
	void (*dec_vHQCV)(RDPG_o *,char *);

	/* Punteros de funcion para funciones que utilizan control de entrada sformat_control.h*/
	int (*shoot_rdpg_s)(RDPG_o *,char *, SHOT_MODE);

	/* Punteros de funcion para funciones SMP-secure de escritura a la RDPG: */
	int (*SMPs_shoot_rdpg_s)(RDPG_o *,char*, SHOT_MODE);
	int (*SMPs_shoot_rdpg)(RDPG_o *,int, SHOT_MODE);
	int (*SMPs_add_value_vG)(RDPG_o *,char *);
	void (*SMPs_update_work_components)(RDPG_o *);
	void (*SMPs_load_vcomp_with_vcomp)(RDPG_o *, int, int);
	int (*SMPs_get_TokensPlace)(RDPG_o *, char *);
	int (*SMPs_get_vHDelement)(RDPG_o *, char *);
	void (*SMPs_set_MemAllocMode)(RDPG_o *, char *);
	void (*SMPs_set_select_comp)(RDPG_o *, int);
	void (*SMPs_set_read_mode)(RDPG_o *,ID_READ_MODE);
	void (*SMPs_set_read_smode)(RDPG_o *, ID_READ_SUBMODE);
	void (*SMPs_set_posVP)(RDPG_o *,char *);
	void (*SMPs_set_posVT)(RDPG_o *,char *);
	void (*SMPs_set_vdim)(RDPG_o *,char *);
	void (*SMPs_set_catComp)(RDPG_o *,int);
	void (*SMPs_inc_vHQCV)(RDPG_o *,char *);
	void (*SMPs_dec_vHQCV)(RDPG_o *,char *);

	/* Punteros de funcion para funciones SMP-secure de lectura a la RDPG: */
	int (*SMPs_read_rdpg_component)(RDPG_o *,char *, int, int);
	int (*SMPs_read_rdpg_info)(RDPG_o *,char *);
	
};


/* Estructura de objeto RDPG_o utilizado por libreria */
struct RDPG_struct
{
  	/* Variables de objeto RDPG: identificaciones de RDPG_o y variables de estados de objeto RDPG_o */
	rwlock_t lock_RDPG;						/* Bloqueo de RDPG, variable spinlock proveedora de exclusión mutua a la RDPG. */
	char name[N_NAME];						/* Nombre del objeto RDPG_o. */
	int obj_id;								/* Identificador del objeto RDPG. */
	//int select_comp[ID_MC_END+ID_VC_END];	/* Numero de componente selecionado. (Pertenece a [ID_MCOMPONENT U ID_VCOMPONENT]). */
	int select_comp;						/* Numero de componente selecionado. (Pertenece a [ID_MCOMPONENT U ID_VCOMPONENT]). */
	int shot_result;						/* Resultado de ultimo disparo realizado en la RDPG_o. Ver enumeracion RDPG_SHOT_RESULT. */
	int error_code;							/* Codigo del ultimo error sucedido. */
	size_t posVP;							/* Posicion de la vista de plazas. */
	size_t posVT;							/* Posicion de la vista de transiciones. */
	size_t vdim;							/* Dimension de visualizacion de componentes. Es el numero de plazas y transiciones a visualizar en una lectura (read) al driver. */
	OBJ_CREATION_MARK mc; 					/* Marca de creacion de objeto RDPG_o. 1: indica una RDP creada. 0: indica que la RDP no se creo. */
	ID_READ_MODE read_mode; 				/* Numero identificador del modo de lectura que tiene la RDP en la funcion read */
	ID_READ_SUBMODE read_smode;         	/* Numero identificador del sub-modo de lectura que tiene la RDP en la funcion read, solo para los casos que lo requieren. */
	char s_plazas[N_BYTES];					/* Numero de plazas de la RDP en caracteres. */
	char s_transiciones[N_BYTES];			/* Numero de transiciones de la RDP en caracteres. */
	char s_size[N_BYTES];					/* Numero de bytes reservados por la RDP en caracteres. */
	char s_realSize[N_BYTES];				/* Numero real de bytes reservados por la RDP en caracteres. */
	char s_allocMode[N_NAME];				/* Nombre del modo de asignacion de memoria utilizado en la RDP. */
	char s_header_transiciones[N_HEADER]; 	/* Cadena de caracteres con la Cabecera de transiciones. */
	char s_header_plazas[N_HEADER];			/* Cadena de caracteres con la Cabecera de plazas. */
	char s_header_disparos[N_HEADER];		/* Cadena de caracteres con la Cabecera de disparos. */


	/* Variables de objeto RDPG: componentes matrices de RDPG_o */
	matrix_o mII;    /* Matriz de incidencia I. */
	matrix_o mIH;    /* Matriz de incidencia H asociada a los brazos inhibidores. */
	matrix_o mIR;    /* Matriz de incidencia R asociada a los brazos lectores. */
	matrix_o mIRe;   /* Matriz de incidencia Re asociada a los arcos reset. */
	matrix_o mD;     /* Matriz con cada uno de los vectores disparos. */
	

	/* Variables de objeto RDPG: componentes vector de RDPG_o */
	vector_o vMI;  	/* Vector de marcado inicial. */
	vector_o vMA;   /* Vector de marcado actual. */
	vector_o vMN;   /* Vector de marcado nuevo. */
	vector_o vE;    /* Vector de transiciones sensibilizadas. */
	vector_o vQ;    /* Vector asociado a la funcion cero. */
	vector_o vW;    /* Vector asociado a la funcion uno. */
	vector_o vL;    /* Vector de transiciones inhibidas por arco lector L. */
	vector_o vB;    /* Vector de transiciones inhibidas por arco inhibidor B. */
	vector_o vG;    /* Vector de transiciones des-sensibilizadas por guarda G. */
	vector_o vA;    /* Vector de transiciones reset A. */
	vector_o vUDT;  /* Vector de resultado de ultimo disparo de transiciones. */
	vector_o vEx;   /* Vector de transiciones sensibilizadas extendido Ex. */
	vector_o vHQCV;	/* Vector de Hilos en Cola de Variables de Condicion (Threads in condition variables queue). */
	vector_o vHD;	/* Vector de hilos a despertar. */
	

	matrix_o *mcomponent[ID_MC_END]; /* Tabla indice de todos los componentes matrix_o de la RDP. */
	vector_o *vcomponent[ID_VC_END]; /* Tabla indice de todos los componentes vector_o de la RDP. */

	/* Funciones de objeto RDPG_o*/
	const struct RDPG_methods *methods;
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

void RDPG_firm(void);

void new_RDPG(RDPG_o *p_rdp, char *p_name);
void create_rdpg(RDPG_o *p_rdp, char * p_entrada);
void delete_rdpg(RDPG_o *p_rdp);
void create_rdpg_mcomponent(matrix_o *p_mo, int p_filas, int p_columnas);
void create_rdpg_vcomponent(vector_o *p_vo, int p_elements);
void delete_rdpg_mcomponent(matrix_o *p_mo);
void delete_rdpg_vcomponent(vector_o *p_vo);
void create_rdpg_mdisparos(matrix_o *p_mo, int p_c);
int add_value_in_mcomponent(matrix_o *p_m, char *p_entrada);
int add_value_in_vcomponent(vector_o *p_vo, char *p_entrada);
int add_value_in_mIRe(RDPG_o *p_rdp, char *p_entrada);
int add_value_vG(RDPG_o *p_rdp, char *p_entrada);
int get_TokensPlace(RDPG_o *p_rdp, char *p_entrada);
int get_vHDelement(RDPG_o *p_rdp, char *p_entrada);

void calculate_sizes(RDPG_o *p_rdp);
static void index_components(RDPG_o *p_rdp);
void define_headers(RDPG_o *p_rdp);
void set_MemAllocMode(RDPG_o *p_rdp, char *c_mode);
void set_read_mode(RDPG_o *p_rdp, ID_READ_MODE p_mode);
void set_read_smode(RDPG_o *p_rdp, ID_READ_SUBMODE p_smode);
void set_select_comp(RDPG_o *p_rdp, int p_component);
void set_posVP(RDPG_o *p_rdp, char *p_entrada);
void set_posVT(RDPG_o *p_rdp, char *p_entrada);
void set_vdim(RDPG_o *p_rdp, char *p_entrada);
void set_catComp(RDPG_o *p_rdp, int p_comp);
void inc_vHQCV(RDPG_o *p_rdp, char *p_entrada);
void dec_vHQCV(RDPG_o *p_rdp, char *p_entrada);


int disparar(RDPG_o *p_rdp, int id_d, SHOT_MODE p_mode);
int disparar_rdpg(RDPG_o *p_rdp, int p_idT, SHOT_MODE p_mode);
int disparar_rdpg_s(RDPG_o *p_rdp, char *p_entrada, SHOT_MODE p_mode);
void cargar_Dvcomp_con_Ovcomp(RDPG_o *p_rdp, int p_vcomp_dst, int p_vcomp_src);
void update_vE(RDPG_o *p_rdp);
void update_vQ(RDPG_o *p_rdp);
void update_vW(RDPG_o *p_rdp);
void update_vB(RDPG_o *p_rdp);
void update_vL(RDPG_o *p_rdp);
void update_vG(RDPG_o *p_rdp);
void update_vA(RDPG_o *p_rdp);
void update_vUDT(RDPG_o *p_rdp);
void update_vEx(RDPG_o *p_rdp);
void update_vHD(RDPG_o *p_rdp);
void update_work_components(RDPG_o *p_rdp);


static int valid_read_mode_id(ID_READ_MODE p_component);
int valid_read_smode_id(ID_READ_SUBMODE p_smode);
int valid_mcomp_id(int p_mcomp_id);
int valid_vcomp_id(int p_vcomp_id);
char *rdpg_component_name(RDPG_o *p_rdp, int p_comp);

int read_rdpg_component(RDPG_o *p_rdp, char *p_kbuf, int p_comp, int p_read_mode);
int print_rdpg_mcomponent(matrix_o *p_mo, char *kbuf, size_t p_posP, size_t p_posT, size_t p_vdim);
int print_rdpg_mcomponent2(RDPG_o *p_rdp, int p_mcomp_id, char *p_kbuf);
int print_rdpg_vcomponent(vector_o *p_vo, char *p_kbuf, size_t posV, size_t p_vdim);
int read_rdpg_info(RDPG_o *p_rdp, char *p_kbuf);
int print_to_user(char *kbuf, char * p_buf, int p_len);

static void getString_allocMode(RDPG_o *p_rdp);
static void config_default_mode(RDPG_o *p_rdp);

/* Declaracion de conjunto de funciones con proteccion SMP-secure*/
int SMPs_disparar_rdpg(RDPG_o *p_rdp, int p_idT, SHOT_MODE p_mode);
int SMPs_disparar_rdpg_s(RDPG_o *p_rdp, char *p_entrada, SHOT_MODE p_mode);
int SMPs_disparar_rdpg(RDPG_o *p_rdp, int p_idT, SHOT_MODE p_mode);
int SMPs_add_value_vG(RDPG_o *,char *);
void SMPs_update_work_components(RDPG_o *p_rdp);
void SMPs_load_vcomp_with_vcomp(RDPG_o *p_rdp, int p_vcomp_dst, int p_vcomp_src);
int SMPs_get_TokensPlace(RDPG_o *p_rdp, char *p_entrada);
int SMPs_get_vHDelement(RDPG_o *p_rdp, char *p_entrada);
void SMPs_set_MemAllocMode(RDPG_o *p_rdp, char *c_mode);
void SMPs_set_select_comp(RDPG_o *p_rdp, int p_component);
void SMPs_set_read_mode(RDPG_o *p_rdp, ID_READ_MODE p_mode);
void SMPs_set_read_smode(RDPG_o *p_rdp, ID_READ_SUBMODE p_smode);
void SMPs_set_posVP(RDPG_o *p_rdp, char *p_entrada);
void SMPs_set_posVT(RDPG_o *p_rdp, char *p_entrada);
void SMPs_set_vdim(RDPG_o *p_rdp, char *p_entrada);
void SMPs_set_catComp(RDPG_o *p_rdp, int p_comp);
void SMPs_inc_vHQCV(RDPG_o *p_rdp, char *p_entrada);
void SMPs_dec_vHQCV(RDPG_o *p_rdp, char *p_entrada);
int SMPs_read_rdpg_component(RDPG_o *p_rdp, char *p_kbuf, int p_comp, int p_read_mode);
int SMPs_read_rdpg_info(RDPG_o *p_rdp, char *p_kbuf);


#endif /* RDPG_LIB_H */