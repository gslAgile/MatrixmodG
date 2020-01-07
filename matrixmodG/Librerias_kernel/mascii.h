/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria mascii.h (archivo cabecera - header file).
 * 
 * 
 * Esta libreria contiene un conjunto de funciones propias que trabajan sobre cadenas de caracteres en codigo ASCII.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef MASCII_LIB_H
#define MASCII_LIB_H

#include <linux/string.h>
#include <linux/kernel.h>


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE MACROS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MASCII_DB_MSG   0       /* Macro de mensajes de debug para mascii lib. 0: Deshailitada, 1: Habilitada. */
#define FAIL_CONCAT		-1			/* Erorr en la funcion de concatenacion. */

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES DE LIBRERIA
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/
void itoa(int p_entero, char *p_str, size_t p_len);
int concat_x1(char *p_strdst, char *p_str1);
int concat_x2(char *p_strdst, char *p_str1, char *p_str2);
int concat_x3(char *p_strdst, char *p_str1, char *p_str2, char *p_str3);
int concatn_x2(char *p_strdst, char *p_str1, char *p_str2);


#endif /* Fin de MASCII_LIB_H */