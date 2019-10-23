#ifndef SFORMAT_CONTROL_LIB_H
#define SFORMAT_CONTROL_LIB_H

/* sformat_control.h: Este archivo proporciona un conjunto de funciones para controlar el Formato de Strings (cadenas de caracteres). */

#include <linux/module.h>
#include <linux/kernel.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE MACROS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SFC_DB_MSG   0            /* Macro de mensajes de debug para Sformat control lib. 0: Deshailitada, 1: Habilitada. */
#define MIN_CHAR_TAM 1

#define EXTRACCION_OK 1           /* Retorno de funcion cuando la extraccion de un dato fue exitosa. */
#define ERROR_EXTRAER_FC  -1      /* Retorno de error cuando no se pudo extraer numero de filas o colunas segun tipo de errores de enum TIPO_ERROR. */
#define ERROR_EXTRAER_FCV -1      /* Retorno de error cuando no se pudo extraer numero de posicion de filas/colunas o valor segun tipo de errores de enum TIPO_ERROR. */
#define ERROR_EXTRAER_ENTERO -1   /* Retorno de error cuando no se pudo extraer el numero de transicion que se desea disparar, o el numero extraido es negativo. */

#define COMMANDSIZE     256
#define BUFFER_LENGTH   2048


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES ASOCIADAS A OBJETO matrix_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int controlador_errores_entrada_numeros(char *p_entrada, const int p_cnum);
int detectar_nesp(char c[2], char *p, int *ccf, const int p_n);
int detectar_char(char c[2] ,char *p);
size_t contar_char(char p_c[2], char *p_entrada);
int extraer_enterox2(int *p_f, int *p_c, char *p_entrada);
int extraer_fcv(int *p_f, int *p_c, int *p_v, char *p_entrada);
int extraer_entero(char *entrada, int *transicion);


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE ENUMERACIONES DISPONIBLES POR LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum TIPO_ERROR{
  /* ERRORES EN CADENAS DE ENTRADA DE NUMEROS*/
  ERROR_CADENA_0 = 0,
  ERROR_CADENA_1,
  ERROR_CADENA_2,
  ERROR_CADENA_3,
  ERROR_CADENA_4,
  ERROR_CADENA_5,
  ERROR_CADENA_6,
  ERROR_CADENA_7,
  ERROR_CADENA_8,

  /* ERRORES DE EXTRACCION DE DATOS*/
  ERROR_EXTRAER_1 = 101,
  ERROR_EXTRAER_2,
  ERROR_EXTRAER_3,
  ERROR_EXTRAER_4,
  ERROR_EXTRAER_5

};


enum ASCII_CHARS{
	ASCII_CERO = 48,
	ASCII_NUEVE = 57,
	ASCII_CR = 13,
	ASCII_ESPACIO = 32,
	ASCII_GUION = 45

};


#endif /* SFORMAT_CONTROL_LIB_H */
