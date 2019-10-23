#include "mascii.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE FUNCIONES DECLARADAS EN mascii.h
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion realiza la conversion de un entero a un ascii en una cadena de caracteres de tamaño p_len enviado por parametro.
 *
 * @param[in]  p_entero  Numero entero que se desea convertir a ascii.
 * @param      p_str     Puntero a la cadena de caracter donde se almacena la conversion del entero a ascii.
 * @param[in]  p_len     Longitud de la cadena en la que se almacena la conversion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void itoa(int p_entero, char *p_str, size_t p_len)
{
	memset(p_str, '\0', p_len);// se limpia cadena

	if(sprintf(p_str, "%d", p_entero) < 0) /* Si falla conversion. */
	{
		strncpy(p_str, "?", p_len); /* Se indica con un ? (signo de pregunta) que no se dectecto el entero. */
		if(MASCII_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Falla en conversion de entero a ascci (funcion itoa()). \n");
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de concatenar una cadena de caracteres en una cadena destino.
 *
 * @param      p_strdst  Puntero a la cadena de carcteres donde se almacena la concatenacion.
 * @param      p_str1    Puntero a la 1er cadena de caracteres a concatenar.
 * 
 * @return     El retorno es:
 * 				strlen(p_strdst): Es el tamaño final de la cadena concatenada.
 * 				FAIL_CONCAT: es un numero negativo que indica un fallo en la ejecucion de la funcion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int concat_x1(char *p_strdst, char *p_str1)
{
	if( (p_strdst != NULL) && (p_str1 != NULL) )
	{
		strcat(p_strdst, p_str1);
	}
	else
	{
		if(MASCII_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Falla en la concatenacion de cadenas de caracter (funcion concat_x1()). \n");
		return FAIL_CONCAT;
	}

	return strlen(p_strdst);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de concatenar dos cadenas de caracteres en una cadena destino.
 *
 * @param      p_strdst  Puntero a la cadena de carcteres donde se almacena la concatenacion.
 * @param      p_str1    Puntero a la 1er cadena de caracteres a concatenar.
 * @param      p_str2    Puntero a la 2da cadena de caracteres a concatenar.
 * 
 * @return     El retorno es:
 * 				strlen(p_strdst): Es el tamaño final de la cadena concatenada.
 * 				FAIL_CONCAT: es un numero negativo que indica un fallo en la ejecucion de la funcion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int concat_x2(char *p_strdst, char *p_str1, char *p_str2)
{
	if( (p_strdst != NULL) && (p_str1 != NULL) && (p_str2 != NULL) )
	{
		strcat(p_strdst, p_str1);
		strcat(p_strdst, p_str2);
	}
	else
	{
		if(p_strdst != NULL)

		if(MASCII_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Falla en la concatenacion de cadenas de caracter (funcion concat_x2()). \n");
		return FAIL_CONCAT;
	}

	return strlen(p_strdst);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de concatenar tres cadenas de caracteres en una cadena destino.
 *
 * @param      p_strdst  Puntero a la cadena de carcteres donde se almacena la concatenacion.
 * @param      p_str1    Puntero a la 1er cadena de caracteres a concatenar.
 * @param      p_str2    Puntero a la 2da cadena de caracteres a concatenar.
 * @param      p_str3    Puntero a la 3er cadena de caracteres a concatenar.
 * 
 * @return     El retorno es:
 * 				strlen(p_strdst): Es el tamaño final de la cadena concatenada.
 * 				FAIL_CONCAT: es un numero negativo que indica un fallo en la ejecucion de la funcion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int concat_x3(char *p_strdst, char *p_str1, char *p_str2, char *p_str3)
{
	if( (p_strdst != NULL) && (p_str1 != NULL) && (p_str2 != NULL) && (p_str3 != NULL) )
	{
		strcat(p_strdst, p_str1);
		strcat(p_strdst, p_str2);
		strcat(p_strdst, p_str3);
	}
	else
	{
		if(MASCII_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Falla en la concatenacion de cadenas de caracter (funcion concat_x3()). \n");
		return FAIL_CONCAT;
	}

	return strlen(p_strdst);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de concatenar dos cadenas de caracteres en una cadena destino, teniendo en cuenta el tamaño
 * real de cada cadena a concatenar. Ocupa menos memoria.
 *
 * @param      p_strdst  Puntero a la cadena de carcteres donde se almacena la concatenacion.
 * @param      p_str1    Puntero a la 1er cadena de caracteres a concatenar.
 * @param      p_str2    Puntero a la 2da cadena de caracteres a concatenar.
 * 
 * @return     El retorno es:
 * 				strlen(p_strdst): Es el tamaño final de la cadena concatenada.
 * 				FAIL_CONCAT: es un numero negativo que indica un fallo en la ejecucion de la funcion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int concatn_x2(char *p_strdst, char *p_str1, char *p_str2)
{
	if( (p_strdst != NULL) && (p_str1 != NULL) && (p_str2 != NULL) )
	{
		strncat(p_strdst, p_str1, strlen(p_str1));
		strncat(p_strdst, p_str2, strlen(p_str2));
	}
	else
	{
		if(MASCII_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Falla en la concatenacion de cadenas de caracter (funcion concat_x2()). \n");
		return FAIL_CONCAT;
	}

	return strlen(p_strdst);
}