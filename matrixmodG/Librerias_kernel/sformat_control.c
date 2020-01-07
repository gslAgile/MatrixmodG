/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria sformat_control.c (archivo fuente - source file).
 * 
 * 
 * Esta libreria contiene un conjunto de funciones que trabajan sobre cadenas de caracteres, analizando los datos para la deteccion de errores y la adecuada
 * extraccion de datos.
 * 
 * En caso de encontrar errores las funciones retornan un codigo de error asociado definido en la enumeracion enum TIPO_ERROR.
 * 
 * La libreria fue testeada por un conjunto de pruebas unitarias e integrales mediante el framework Kernel Test Framework (KTF). Es por lo cual la libreria 
 * durante un proceso de ejecucion de pruebas, hace uso de las librerias KTF y habilita todas las funcionalidades del framework, en caso contrario por
 * defecto, en proceso de ejecucion estandar, se ignoran todas las definiciones de KTF.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "sformat_control.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE FUNCIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion detecta la posicion de un caracter (pasado por paramentro) sobre la cadena a la que apunte el puntero *p y dentro de n caracteres.
 * Tambien guarda (en la direccion ccf) la cantidad de avances necesarios de caracteres hasta encontrar el caracter de espacio generalmente dado por "_" .
 *
 * @param      c     Caracter a detectar.
 * @param      *p    Puntero que apunta a la cadena donde se busca el espacio ("_").
 * @param      *ccf  Puntero que apunta a la direccion de un entero donde se almacena la cantidad de avances hasta encontrar el caracter de espacio c.
 * @param      p_n   Entero que indica el numero de los n caracteres de *p sobre los que se realiza la busqueda el caracter de espacio c.
 *                   
 * @return     Retorna la direccion donde se encontro el espacio. Sino se encontro, se retorna la direccon inicial de *p.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int detectar_nesp(char c[2], char *p, int *ccf, const int p_n)
{
  int i, aux;
  aux = (int)p; /* Guardamos auxiliarmente la direccion que tiene p. */

  /* Recorro la cadena de caracter en busqueda de c[]. */
  for (i = 0; i < p_n; i++)
  {
    if(strncmp(p,c, 1) == 0)
      break;
    else {
      p = p + 1;
      *ccf = *ccf + 1;
    }
  }

  if(i == p_n) {
    p = aux; /* Volvemos a asignar direccion de inicio ya que no se encontro ningun '_'. */
    if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error: No se encontro direccion de caracter de espacio '_' sobre la entrada dada."\
      " Verifique si la cadena de entrada respeta el formato de espacios por ejemplo 10_33 siendo 10 un entero y 33 otro entero separados por el espacio '_'. \n");
  }

  return (int)p;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion detecta la posicion de un caracter (pasado por paramentro) sobre la cadena a la que apunte el puntero *p.
 *
 * @param      c     caracter a detectar.
 * @param      *p    puntero que apunta a la cadena donde se busca el espacio (dado por caracter '_' -> guion bajo)
 * la cantidad de avances hasta encontrar el espacio. Se puedde mandar NULL si no se nescesita.
 * 
 * @return     Retorna la direccion donde se encontro el espacio. Sino se encontro, se retorna la direccon inicial de *p.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int detectar_char(char c[2], char *p)
{
	int i, aux;
	aux = (int)p; // Guardamos auxiliarmente la direccion que tiene p

	/* Buscamos direccion de '_' avanzando para adelante */
  	for (i = 0; i < COMMANDSIZE; i++)
  	{
  		if(strncmp(p,c, 1) == 0)
  			break;
  		else
  			p = p + 1;
  	}

  	if(i == COMMANDSIZE) {
  		p = aux; /* Volvemos a asignar direccion de inicio ya que no se encontro ningun '_' .*/
  		if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error: No se encontro direccion de '_'\n");
  	}

  	return (int)p;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de analizar una cadena de caracteres enviada por parametro, para determinar si su tamaño, informacion y formato 
 * es el adecuado interpretado por el modulo. Caso contrario se determina avisara del inconveniente con el tamaño informacion o formato.
 * La informacion en este caso se focaliza en buscar numeros enteros de acuerdo a un formato, caso contrario se avisa de cualquier inconveniete existente.
 *
 * @param      p_entrada  Puntero a la direccion de memoria en donde se encuentra almacenada la informacion de la cadena de caracteres.
 * @param      p_cnum     Valor que indica la cantidad de numeros que se desean controlar en una entrada
 *
 * @return     1    Si la cadena respeta un adecuado tamaño, informacion y formato.
 * @return     -1   Si la cadena no respeta un adecuado tamaño, informacion y formato, lo que indica un problema/error incompatible con el modulo sobre
 * la cadena de caracteres entrante.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int controlador_errores_entrada_numeros(char *p_entrada, const int p_cnum)
{
  int c_tam; /* variable que almacenara el tamao de la cadena de caracteres. */
  int cont_cifras=0; /* Contador de cifras de numeros.*/

  /* Analisis del tamaño de la cadena de caracteres. */
  c_tam = strlen(p_entrada); /* Determino tamanio de la cadena de caracteres. */
  if(c_tam < MIN_CHAR_TAM)
  {
    if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de entrada no respeta con el minimo de longitud admitido por el modulo. "\
      "No se puede procesar informacion, verifique si la cadena enviada es correcta y respeta el formato de espacio admitido (Ejemplo 10_12_1).\n", ERROR_CADENA_0);
    return -1;
  }

  if(p_cnum < 1)
  {
  	if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Error de control de cadena. Parametro de cantidad de numeros a verificar negativo. \n", ERROR_CADENA_0);
    return -1;
  }

  /* Se determina cuantos espacios contiene la cadena.*/
  size_t c_espacios;
  c_espacios = contar_char("_", p_entrada); /* Cuento la cantida de espacios que ai en cadena de caracteres. */

  if(c_espacios >= p_cnum) /* Si la cantidad de espacios '_' es igual o mayor que la cantidad de datos a extraer (p_cnum) entonces ai datos extra por lo que habra errores en la extraccion. */
  {
    if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error: Error de control de cadena. La cantidad de datos enviados en la cadena es mayor que los datos que se desean extraer. \n");
    return -1;
  }

  /* Analisis de informacion de la cadena de caracteres y analisis de formato de la cadena de caracteres. */
  int i;
  size_t cont_espaux = 0; /* Contador de espacios a utilizar. Se usa para mantener la cuenta de espacios encontrados en la cadena por cada numero.*/
  int estado_busqueda = 1; /* - 1: Estado de busqueda de errores en 1er numero del formato interpretado. Inicialmente la busqueda comienza con este modo.
                              - 2: Estado de busqueda de errores en 2do numero del formato interpretado. Este modo es analizado cuando paso con exito el modo 1.
                              - n: Estado de busqueda de errores en n-simo numero del formato interpretado. Este modo es analizado igual que el modo 2.
                              */
  for(i=0; i < c_tam; i++)
  {
    if(estado_busqueda==1)
    {
      if((p_entrada[i] < ASCII_CERO) || (p_entrada[i] > ASCII_NUEVE))  /* Si caracter no es caracter numero (ascii del 48 al 57 caracteres del 0 al 9). */
      {
        if(strncmp((p_entrada+i),"_", 1) == 0) /* Si caracter es un '_'. Ya que este caracter es valido como espacio. Se debera anilizar 2do numero. */
        {
          cont_espaux++; /* Se suma la cantidad de espacios por numero. */
          if(cont_cifras > 0)
          {
            estado_busqueda = estado_busqueda + 1; /* Se pasa a siguiente modo de busqueda. */
            cont_cifras = 0; /* Reinicio cifras de numero. */
          }
          else
          {
            /* Cadena de caracteres  incompleta. En formato de ingreso de numeros de filas y columnas f_c, no se ingreso el numero de filas f. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Cadena de caracteres incompleta.\n", ERROR_CADENA_1);
            return -1;
          }
        }
        else
        {
          
          if((strncmp((p_entrada+i)," ", 1) == 0) && c_tam == MIN_CHAR_TAM)
          {
            /* Cadena de caracteres  vacia. En formato de ingreso de numero de transicion, no se ingreso el numero de transicion. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error: Cadena de caracteres vacia.\n");
            return -1;
          }
          else if((p_cnum == 1) && (strncmp((p_entrada+i)," ", 1) == 0)) /* Si ai espacios cuando se envia un solo dato. */
          {
            estado_busqueda = estado_busqueda + 1; /* Se pasa a siguiente modo de busqueda. */
            //break; /* Salimos del for ya que ignoramos el resto de caracteres y se extrae solamente el dato valido.*/
          }
          else
          {
            /*Error: En formato de ingreso de numeros de filas y columnas f_c, en el numero de filas se ingreso un caracter no valido como entero. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de caracteres tiene un caracter no valido como entero.\n", ERROR_CADENA_2);
            return -1;
          }
        }
      }
      else
      {
        cont_cifras++; /* Se suman las cifras de 1er numero. */
      }
    }
    else if((p_cnum > MIN_CHAR_TAM) && (estado_busqueda <= p_cnum)) /* Si la cantidad de numeros a controlar (p_cnum) es mayor que uno, entonces sigo controlando los siguientes numeros dentro de la cadena.*/
    {
      if((p_entrada[i] < ASCII_CERO) || (p_entrada[i] > ASCII_NUEVE))  /* Si caracter no es caracter numero (ascii del 48 al 57 caracteres del 0 al 9). */
      {
        if(strncmp((p_entrada+i),"_", 1) == 0) /* Si caracter es un '_'. Ya que este caracter es valido como espacio. Se debera anilizar 2dp numero. */
        {
          cont_espaux++; /* Se suma la cantidad de espacios por numero. */
          if(cont_cifras > 0)
          { 
            if((i+1)==c_tam)
            {
              /* Error por cadena imcompleta. En formato de ingreso de numeros de pos filas,columnas y valor f_c_v, no se ingreso el numero de v. Por fin de cadena. */
              if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Cadena de caracteres incompleta.\n", ERROR_CADENA_3);
              return -1;
            }
            else
            {
              estado_busqueda = estado_busqueda + 1; /* Se pasa a siguiente modo de busqueda. */
              cont_cifras = 0; /* Reinicio cifras de numero. */
            }
          }
          else
          {
            /* Cadena de caracteres incompleta. En formato de ingreso de numeros de filas y columnas f_c, no se ingreso el numero de columnas c. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Cadena de caracteres incompleta.\n", ERROR_CADENA_4);
            return -1;
          }
        }
        else if((p_entrada[i] != ASCII_ESPACIO) && (p_entrada[i] != ASCII_CR) && (p_entrada[i] != ASCII_GUION)) /* Si no se trata de un caracter espacio ' ' (ascii 32) ni tampoco un caracter retorno de carro '\n'. */
        {
          /* En formato de ingreso de numeros n1_n2_n3, el numero n2 o n3 se ingreso un caracter no valido como entero. */
          if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de caracteres tiene un caracter no valido como entero.\n", ERROR_CADENA_5);
          return -1;
        }
        else if(p_entrada[i] == 45)
        {
          if(cont_espaux < 2)
          {
            /* En formato de ingreso de numeros n1_n2_n3, n2 tiene un numero negativo. Solo se permiten negativos de n3 en adelante. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de caracteres tiene un caracter no valido como entero.\n", ERROR_CADENA_6);
            return -1;
          }
          else if( (i+1) == c_tam)
          {
            /* Error por cadena imcompleta. En formato de ingreso de numeros de posicones de fila, columna y valor f_c_v, el simbolo '-' es el ultimo caracter. */
              if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Cadena de caracteres incompleta.\n", ERROR_CADENA_7);
              return -1;
          }
          else if(strncmp((p_entrada+i), " ", 1) == 0)
          {
              /* Error por cadena imcompleta. idem anterior, sola que se determina por darse cuenta de que existe un espacio ' '. */
              if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Cadena de caracteres incompleta.\n", ERROR_CADENA_7);
              return -1;
          }
        }
        else /* Si se trata de un caracter retorno de carro '\n' (ascii 13) o espacio ' ' (ascii 32). */
        {
          if(cont_espaux < c_espacios)
          {
            /* En formato de ingreso de numeros n1_n2_n3, entre n2 y n1, o entre n2 y n3 existe un espacio. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de caracteres tiene un caracter no valido como entero.\n", ERROR_CADENA_8);
            return -1;
          }
          else if(cont_cifras < 1)
          {
            /* Error por cadena incompleta. */
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: La cadena de caracteres es incompleta.\n", ERROR_CADENA_7);
            return -1;
          }
        }
      }
      else
      {
        cont_cifras++; /* Se suman las cifras de 2do numero. */
      }
    }
  }

  return 1;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion detecta la posicion de un caracter (pasado por paramentro) sobre la cadena a la que apunte el puntero *p y dentro de n caracteres.
 * Tambien guarda (en la direccion ccf) la cantidad de avances necesarios de caracteres hasta encontrar el caracter de espacio generalmente dado por '_' .
 *
 * @param      p_c     Caracter que se desea contar sobre cadena de caracteres.
 * @param      *p_entrada   Puntero que apunta a la cadena de caracteres donde se busca el caracter p_c.
 *                   
 * @return     Retorna la cantidad de veces que se encontro p_c en la cadena de caracteres. Tipo de dato size_t ya que no ai posibilidad que la cuenta de
 * caracteres devuelva un numero negativo.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t contar_char(char p_c[2], char *p_entrada)
{
  size_t i, c_tam, cuenta_char;
  char *punt_cuenta; /* Puntero utilizado para realizar la cuenta del caracter sobre la cadena.*/

  /* Inicializo variables. */
  cuenta_char = 0;
  punt_cuenta = p_entrada; /* Se inicia puntero en misma posicion que p_entrada sobre la cadena.*/

  /* Se determina tamaño de cadena. */
  c_tam = strlen(p_entrada);

  /* Recorro la cadena de caracter en busqueda de caracter p_c. */
  for (i = 0; i < c_tam; i++)
  {
    if(strncmp(punt_cuenta,p_c, 1) == 0)
    {
      cuenta_char++; /* Sumo la cuenta en uno, cada vez que se encuentra caracter sobre cadena.*/
    }
    
    punt_cuenta = punt_cuenta + 1; /* Avanzo proximo caracter de cadena. */
  }

  return cuenta_char;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion extrae el numero de la filas y el numero de las columnas desde una entrada de cadena de caracteres. Los datos se extraen desde
 * p_entrada que es la cadena de caracteres que contiene la informacion con un formato definido. Cada dato extraido se almacena en variables enteras para
 * luego ser utilizadas en la creacion de un nuevo componente de una RDPG.
 *
 * @param      *p_f        Puntero a la direccion de la variable entera donde se almacena el "numero de filas".
 * @param      *p_c        Puntero a la direccion de la variable entera donde se almacena el "numero de columnas".
 * @param      *p_entrada  Puntero a la direccion de la entrada que contiene la informacion de la cadena de caracteres.
 * 
 * @return     El retorno es 1 si la extraccion del numero de filas y columnas fue exitosa.
 * @return     El retorno es -1 si la extraccion del numero de las filas y columnas no fue exitosa.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int extraer_enterox2(int *p_f, int *p_c, char *p_entrada)
{
  char *p0;                   /* Puntero destinado para que apunte a la direccion del caracterer que indica "numero de columnas." */
  int cc_filas=0;             /* cc_filas: contador de cifras del numero de filas. */ 
  char s1[10];                /* Cadena de char donde se almacenara los caracteres del numero de filas. */
  const int cnum_control = 2; /* Cantidad de numeros que se ingresan en la cadena para ser controlados por el algoritmo de busqueda y control
                               * de errores por numero. En este caso como se controla el ingreso de las plazas (filas) y transiciones (columnas) de una RDP es 2. */

  memset(s1, '\0', 10);       /* Limpio cadena s1. */

  /* Control de tamaño, informacion y formato de entrada de caracteres con numeros. */
  if(controlador_errores_entrada_numeros(p_entrada, cnum_control)==-1)
  {
    return ERROR_EXTRAER_FC; /* La funcion se encarga de informar el tipo de error en la cadena de entrada. */
  }
  else /* Si no existe error en la cadena de entrada, se extraen datos desde la misma.*/
  {
    /* Inicializo variables y punteros a utilzar. */
    p0 = p_entrada;
    cc_filas = 0;
    p0 = detectar_nesp("_", p0, &cc_filas, (const int)strlen(p_entrada)); /* Se determina direccion donde empiza el primer espacio dado por '_', de esta forma se saltea "numero de filas". */

    /* Extraccion de entero que indica el "numero de filas". */    
    strncpy(s1, p_entrada, cc_filas); /* Copiamos n=cc_filas caracteres, donde apunta p1+1, en s1. (p1+1): ya que queremos copiar solo el valor luego del espacio '_'. */

    /* Extraccion de entero que indica el "numero de filas". */
    if(sscanf(s1,"%d", p_f) == 1)
    {
      if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: Se extrae de cadena numero de filas asociado a componente de la RDP: %d\n", *p_f);
    }
    else
    {
      if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: No se pudo extraer el numero de filas desde cadena char. Verificar si el comando ingresado es correcto.", ERROR_EXTRAER_1);
      return ERROR_EXTRAER_FC;
    }

    /* Si existe un caso en donde la cadena no se utilizo el caracter '_' como espacio por confusion o error, debemos avisar del error.*/
    /* En este caso si p0 es igual que p_entrada, nos indica que luego de buscar '_', no se encontro y por lo cual las cadenas son iguales.*/
    if(p0 == p_entrada) {
      if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: No se pudo tomar numero de filas/columnas desde comando ingresado. Verificar si el comando ingresado es correcto.", ERROR_EXTRAER_2);
      return ERROR_EXTRAER_FC;
    } else {
      
        /* Extraccion de entero que indica el "numero de columnas". */    
        if(sscanf(p0,"_%d", p_c) == 1) { /* Guardamos valor entero en p_c. */
          if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: Se extrae de cadena numero de columnas asociado a componente de la RDP: %d\n", *p_c);
        } else
          {
            if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Fallo la extraccion de numero de columnas asociado a componente de la RDP. \n", ERROR_EXTRAER_3);
            return ERROR_EXTRAER_FC;
          }

        /* Verificamos si los valores de filas y columnas son negativos o cero. En dicho caso falla la extraccion ya que no se pueden tomar valores negativos o cero.*/
        // if((p_f <= 0) || (p_c <= 0))
        // {
        //   if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Se detectaron valores negativos o ceros de filas y/o columnas. Valores no validos para crear un componente RDP. \n",ERROR_EXTRAER_4);
        //   return ERROR_EXTRAER_FC;
        // }
      }
    return EXTRACCION_OK;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion extrae el numero de la posicion de fila, la posicion de columna y el numero del valor a agregar en esas posiciones. Los datos
 * se extraen desde p_entrada que es la cadena de caracteres que contiene la informacion. Cada extraccion se almacena en variables enteras para luego
 * ser utilizada con los respectivos componentes de una RDPG.
 *
 * @param      *p_f        Puntero a la direccion de la variable entera donde se almacena el numero que indica la "posicion de fila".
 * @param      *p_c        Puntero a la direccion de la variable entera donde se almacena el numero que indica la "posicion de columna".
 * @param      *p_v        Puntero a la direccion de la variable entera donde se almacena el numero que indica el "valor".
 * @param      *p_entrada  Puntero a la direccion de la entrada que contiene la informacion en caracteres.
 * 
 * @return     El retorno es 1 si la extraccion del numero de la posicion de fila, columna y del valor fue exitosa.
 * @return     El retorno es -1 si la extraccion del numero de la posicion de fila, columna y del valor no fue exitosa.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int extraer_fcv(int *p_f, int *p_c, int *p_v, char *p_entrada)
{
  char *p0; /* Puntero destinado para que apunte a la direccion 0 de entrada de caracteres, direccion del caracter que indican "posicion de fila" */
  char *p1; /* Puntero destinado para que apunte a la direccion del caracterer que indica "posicion de columna" */
  char *p2; /* Puntero destinado para que apunte a la direccion del caracter que indica "valor" */
  char s1[10]; /* Cadena de char donde se almacenara la porcion de la cadena de entrada que tiene los caracteres de la posicion de columna.*/
  int cc_col; /* cc_col: contador de cifras de valor que indica la posicion de columna.*/
  const int cnum_control = 3; /* Cantidad de numeros que se ingresan en la cadena para ser controlados por el algoritmo de busqueda y control
                         * de errores por numero. En este caso como se controla las posiciones de filas y columnas de un componente de matrix_o 
                         * y su valor a ingresar sobre dichas posiciones , entonces son 3 numeros que se ingresan a la cadena.  */

  /* Control de tamaño, informacion y formato de entrada de caracteres con numeros. */
  if(controlador_errores_entrada_numeros(p_entrada, cnum_control)==-1)
  {
    return ERROR_EXTRAER_FCV; /* La funcion se encarga de informar el tipo de error en la cadena de entrada. */
  }
  else /* Si no existe error en la cadena de entrada, se extraen datos desde la misma.*/
  {

    /* Extraccion de entero que indica la "posicion de fila". */
    if(sscanf(p_entrada,"%d_%*d_%*d", p_f) != 1)
    {
      if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: No se pudo extraer la posicion de fila. Verificar si el comando ingresado es correcto.", ERROR_EXTRAER_1);
      return ERROR_EXTRAER_FCV;
    }

    /* Inicializo variables y punteros a utilzar. */
    cc_col = 0; /* Inicializamos contador. */
    p0 = p_entrada; /* Asignamos al puntero la direccion 0 de entrada */
    p1 = detectar_char("_", p0); /* Se determina direccion donde empiza el primer espacio dado por '_', de esta forma se saltea "posicion de fila". */
    p2 = detectar_nesp("_", p1+1, &cc_col, (const int)strlen(p1+1)); /* Avanzamos al proximo espacio para conocer las cifras que indican la posicioon de columna. (p1+1) para ignorar
                                            el primer espacio '_' sobre el que esta parado p1. */

    /* Si existe un caso en donde la cadena no se utilizo el caracter '_' como espacio por confusion o error, debemos avisar del error.*/
    /* En este caso si p2=p1=p0 es igual que p_entrada, nos indica que luego de buscar '_', no se encontro y por lo cual las cadenas son iguales.*/
    if(p2 == p_entrada) {
      if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: No se pudo tomar posicion de filas/columna desde comando ingresado. Verificar si el comando ingresado es correcto.", ERROR_EXTRAER_2);
      return ERROR_EXTRAER_FCV;
    } else {
      /* Extraccion de entero que indica la "posicion de columna". */    
      strncpy(s1, p1+1, cc_col); /* Copiamos n=cc_col caracteres, donde apunta p1+1, en s1. (p1+1): ya que queremos copiar solo el valor luego del espacio '_'. */

      if(sscanf(s1,"%d", p_c) == 1) { /* Guardamos valor copiado en s1 como entero en p_c. */
        if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: Extraccion de posicion de columna asociado a componente de la RDP: %d\n", *p_c);
      } else
        {
          if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Fallo la extraccion de posicion de columna asociado a componente de la RDP. \n", ERROR_EXTRAER_3);
          return ERROR_EXTRAER_FCV;
        }

      /* Verificamos si los valores de posicion de fila y columna son negativos. En dicho caso falla la extraccion ya que no se pueden tomar valores negativos.*/
      if((p_f<0) || (p_c < 0))
      {
        if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Se detectaron valores negativos en posicion de fila y/o columna asociado a componente de la RDP."\
          " No existen referencia negativas. \n", ERROR_EXTRAER_4);
        return ERROR_EXTRAER_FCV;
      }
      
      /* Extraccion de entero que indica el "valor". */
      if(sscanf(p2,"_%d", p_v) == 1) {
        if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: Extraccion de valor: %d\n", *p_v);
      } else{
        if(SFC_DB_MSG) printk(KERN_ERR "matrixmod_error_%d: Fallo la extraccion de valor asociado a componente de la RDP. \n", ERROR_EXTRAER_5);
        return ERROR_EXTRAER_FCV;
      }
        
      /*if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: Valor de avance de cifras de 'posicion columna' cc_col : %d \n", cc_col);*/
      memset(s1, '\0', 10); /* Limpio cadena s1. */
    }

    return EXTRACCION_OK;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de extraer un numero entero desde la cadena de caracteres enviada por parametro.
 *
 * @param      entrada     Puntero de la cadena de caracteres que continene un numero en caracteres.
 * @param      p_entero    Puntero de una variable entera en donde sera almacenado el valor entero extraido de la cadena
 * de caracteres.
 *
 * @return     El retorno es EXTRACCION_OK (1) si la extraccion del numero de transicion fue exitosa.
 * @return     El retorno es ERROR_EXTRAER_ENTERO (-1) si la extraccion del numero de entero no fue exitosa.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int extraer_entero(char *p_entrada, int *p_entero)
{
  int t; // t: test funcion sscanf
  
  const int cnum_control = 1; /* Cantidad de numeros que se ingresan en la cadena para ser controlados por el algoritmo de busqueda y control
                         * de errores por numero. En este caso como se controla el numero de disparo a realizar, entonces es 1. */

  /* Control de tamaño, informacion y formato de entrada de caracteres con numeros. */
  if(controlador_errores_entrada_numeros(p_entrada, cnum_control)==-1)
  {
    return ERROR_EXTRAER_ENTERO; /* Termina la funcion ya que la funcion controlador_errores... encontro un tipo de error en la cadena de entrada. */
  }
  else
  {
	  if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: entrada capturada para extraer nro entero: %s\n", p_entrada);

	  t = sscanf(p_entrada," %d", p_entero);

	  if(t != EXTRACCION_OK) {
	    if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_error: no se pudo extraer de comando ingresado nro entero!!! \n");
	    return ERROR_EXTRAER_ENTERO;
	  }
   }

   if(SFC_DB_MSG) printk(KERN_INFO "matrixmod_info: nro entero extraido: %d\n", *p_entero);
   return EXTRACCION_OK;
}
