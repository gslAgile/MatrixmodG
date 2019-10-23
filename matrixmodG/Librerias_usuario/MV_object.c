#include "MV_object.h" 

int MV_OBJECT_MESSAGES = MESSAGES_OFF;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE VARIABLES GLOBALES MANEJADAS POR LOS OBJETOS DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Declaracion global de vtable para objeto matrix_o. Todos los objetos matrix_o tendran los mismos metodos. */
struct matrix_methods matrix_vtable = 
{
	.alloc_matrix_mem = alloc_matrix_mem,
	.free_matrix_mem = free_matrix_mem,
	.set_mem_alloc_mode = set_Mmem_alloc_mode,
	.create_zero_matrix = crear_matriz_cero,
	.create_one_matrix = crear_matriz_uno,
	.create_identity_matrix = crear_matriz_identidad,
	.clean_matrix = limpiar_matriz,
	.negate_matrix = negar_matriz,
	.import_matrix = importar_matriz
};

/* Declaracion global de vtable para objeto vector_o. Todos los objetos vector_o tendran los mismos metodos. */
struct vector_methods vector_vtable = 
{
	.alloc_vector_mem = alloc_vector_mem,
	.free_vector_mem = free_vector_mem,
	.set_mem_alloc_mode = set_Vmem_alloc_mode,
	.create_zero_vector = crear_vector_cero,
	.create_one_vector = crear_vector_uno,
	.clean_vector = limpiar_vector,
	.negate_vector = negar_vector,
	.LoadLiteral_vector = CargarLiteral_vector
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE FUNCIONES DE OBJETO matrix_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion es el constructor del objeto p_mo enviado por parametro. Se encarga de asignar todos los metodos que debe
 * conocer el objeto iniciando la vtable y se inician todos los campos del objeto.
 *
 * @param      p_mo  Es la direccion del objeto matriz que sera inicializado.
 * @note       La funcion es extern ya que podra ser utilizada por funciones de otros archivos que incluyan la cabecera .h.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void new_matrix(matrix_o *p_mo, char *p_name)
{
	if(p_mo->mc != OBJ_CREATED) /* Si objeto matrix_o p_mo no fue creado previamente. Se inicializan sus campos.*/
	{
		/* Inicializo por defecto los campos del objeto. */
		p_mo->filas= 0;
		p_mo->columnas= 0;
		p_mo->tam_bytes= 0;
		p_mo->mc= OBJ_NO_CREATED;
		p_mo->alloc_mode= M_MALLOC;
		/* p_mo->matriz: El doble puntero se inicializa en NULL por defecto segun estandar de C. Se asignara una direccion cuando se
		 * crea la matriz del objeto con la funcion crear_matriz_zero/uno en conjunto con crear_matriz. */
		if(p_name != NULL)
		{
			strcpy(p_mo->name, p_name);
		}
		else
		{
			strcpy(p_mo->name, "-Sin definir-");
		}

		/* Asigno metodos a la vtable del objeto. */
		p_mo->mo_methods= &matrix_vtable;
	}
	else
	{
		printf("LIB_MatrixmodG_error: Error al inicializar nuevo componente, el componente %s ya fue inicializado !!!\n", p_mo->name);
	}
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion aloja memoria en la matriz perteneciente al objeto matrix_o p_mo enviado por parametro.
 * 
 * @param      p_mo         Puntero a la direccion de memoria del objeto matrix_o en donde se reserva la memoria para la matriz.
 *
 * @return     El retorno sera:
 * 			ALLOC_OK: cuando la reserva de memoria para la matriz del objeto matrix_o p_mo se realizo exitosamente.
 * 			ALLOC_ERROR: cuando fallo la reserva de memoria para la matriz del objeto matrix_o p_mo.
 * 			
 * @note       La funcion es static ya que sera utilizada solo por funciones de este archivo. Desde otro archivo no se puede llamar a esta funcion.
 * Se trata de una funcion de gestion interna, para evitar errores.
 **---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int alloc_matrix_mem(matrix_o *p_mo)
{	
	int **m;
	size_t i = 0;
	size_t aux_i = 0;
	size_t aux_tam = 0;
	short alloc_fail = 0;


	if(p_mo->matriz==NULL){

		if(p_mo->alloc_mode == M_MALLOC) /* Si modo de resrva de memoria es M_VMALLOC: Realizo la reserva de memoria dinamica mediante vmalloc().*/
		{
			/* Reserva de memoria para filas de m (creación de arreglo de punteros) */
			m = (int **)malloc(p_mo->filas*sizeof(int*));
			if(m == NULL) /* Si falla la reserva.*/
			{
				return ALLOC_ERROR;
			}

			aux_tam = p_mo->filas*sizeof(int*);
			
			/* Reserva de memoria para columnas de m (creacion de arreglos de enteros por cada m fila) */
			for (i=0; i<p_mo->filas; i++) {
				m[i] = (int*)malloc(p_mo->columnas*sizeof(int));
				if(m[i] == NULL) /* Si falla la reserva en arreglos de alguna fila.*/
				{
					alloc_fail = 1;
					aux_i = i; /* rescato el valor de i, para liberar lo previamente asignado.*/
					aux_tam = 0;
					break; /* Termina bucle for*/
				}
				aux_tam = aux_tam + p_mo->columnas*sizeof(int);
			}

			if(alloc_fail)
			{
				rollback_alloc(m, aux_i, p_mo->alloc_mode);
				return ALLOC_ERROR;
			}

			/* Rellena cada elemento de la matriz con cero. */
			rellenar_matriz(m, p_mo->filas, p_mo->columnas, 0);
		}
		else if (p_mo->alloc_mode == M_CALLOC) /* Si modo de resrva de memoria es M_KCALLOC: Realizo la reserva de memoria dinamica mediante kmalloc_array() y kcalloc().*/
		{
			/* Reserva de memoria para filas de m (creación de arreglo de punteros) */
			m = (int **)malloc(p_mo->filas*sizeof(int*));
			if(m == NULL) /* Si falla la reserva.*/
			{
				return ALLOC_ERROR;
			}

			aux_tam = p_mo->filas*sizeof(int*);
			
			/* Reserva de memoria para columnas de m (creacion de arreglos de enteros por cada m fila) */
			for (i=0; i<p_mo->filas; i++) {
				m[i] = (int*)calloc(p_mo->columnas, sizeof(int));
				if(m[i] == NULL) /* Si falla la reserva en arreglos de alguna fila.*/
				{
					alloc_fail = 1;
					aux_i = i; /* rescato el valor de i, para liberar lo previamente asignado.*/
					aux_tam = 0;
					break; /* Termina bucle for*/
				}
				aux_tam = aux_tam + p_mo->columnas*sizeof(int);
			}

			if(alloc_fail)
			{
				rollback_alloc(m, aux_i, p_mo->alloc_mode);
				return ALLOC_ERROR;
			}
		}
		else
		{
			printf("LIB_MatrixmodG_error: No se pudo reservar memoria. El componente %s no tiene asignado un modo valido de reserva de memoria!!!\n", p_mo->name);
			return ALLOC_ERROR;
		}

		p_mo->matriz=m; /* Guardo la direccion de memoria en la que se aloja mem, en puntero doble matriz de objeto matrix_o. */
		p_mo->tam_bytes = aux_tam; /* Guardo registro de cantidad de memoria reservada en la matriz del objeto matrix_o. */
		
		return ALLOC_OK;
	}
	
	return ALLOC_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de liberar la memoria reservada para la matriz del objeto p_mo enviado por parametro.
 *
 * @param      p_mo  Es un puntero a la estructura matrix_m sobre la que se desea realizar la liberacion de memoria.
 *
 * @return     El retorno es:
 * 				FREE_OK: Si la liberacion de memoria se realizo con exito.
 * 				FREE_ERROR: Si no se puede liberar memoria porque el puntero ya esta liberado o es null por no tener reservada memoria.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int free_matrix_mem(matrix_o *p_mo)
{	
	int i = 0;
	if(p_mo->matriz != NULL){
		
		/* Libero reserva de memoria para columnas de matriz de p_mo */
		for (i = 0; i<(p_mo->filas); i++) 
			free(p_mo->matriz[i]);

		/* Liberar memoria para filas de matriz de p_mo */
		free(p_mo->matriz);

		p_mo->filas = 0;
		p_mo->columnas = 0;
		p_mo->tam_bytes = 0;
		p_mo->mc = OBJ_NO_CREATED;
		p_mo->matriz = NULL; /* Se asigno null a la direccion de la matriz de objeto ya que no tiene mas memoria reservada.*/

		return FREE_OK;
	}
	
	return FREE_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de cambiar el modo de reserva de memoria dinamica para la creacion del componente matrix_o p_mo
 * enviado por parametro.
 *
 * @param[in]  p_mo    Es el objeto matrix_o p_mo enviado por parametro sobre el cual se desea realizar el cambio.
 * @param[in]  p_mode  Es el modo de reserva de memoria dinamica que se desea utilizar para la creacion del objeto matrix_o p_mo enviado
 * por parametro. Los modos posibles son los definidos en la enumeracion MEM_ALLOC_MODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_Mmem_alloc_mode(matrix_o *p_mo, int p_mode)
{
	/* Mientras objeto matrix p_mo no haya sido creado, se puede modificar el modo de reserva de memoria. Caso contrario no sera posible.*/
	if(p_mo->mc == OBJ_NO_CREATED) 
	{
		if(valid_alloc_mode(p_mode) == VALID_MODE)
		{
			p_mo->alloc_mode = p_mode;
		}
	}
	else
	{
		printf("LIB_MatrixmodG_error: No es posible cambiar el modo de reserva de memoria para el componente %s, el componente ya esta creado !!!\n", p_mo->name);
	}
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de verificar si el modo de reserva de memoria enviado por parametro es correcto o no.
 *
 * @param[in]  p_mode  Es el modo de reserva de memoria dinamica a verificar por la funcion.
 *
 * @return     El retorno es:
 * 				VALID_MODE: Si el modo de reserva de memoria dinamica enviado por paremetro corresponde a uno de los modos definidos
 * 				en la enumeracion MEM_ALLOC_MODE.
 * 				INVALID_MODE: Si el modo de reserva de memoria dinamica enviado por paremetro no corresponde a uno de los modos definidos
 * 				en la enumeracion MEM_ALLOC_MODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int valid_alloc_mode(int p_mode)
{
	size_t i;
	for(i=M_INIT; i < M_END; i++)
	{
		if(p_mode==i)
		{
			return VALID_MODE;
		}
	}
	
	printf("LIB_MatrixmodG_error: Se detecto un modo de reserva de memoria no valido por la libreria de objetos matrix_o.\n");
	return INVALID_MODE;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de liberar la memoria previamente asignada en un objeto matrix_o y durante el
 * fallo de una asignacion que conformaba al objeto. Es por lo cual retrocede asignaciones previas debido a un fallo
 * en alguna de las asignaciones de los arreglos de enteros del objeto matrix_o.
 *
 * @param[in]  p_mo      Es el objeto matrix_o p_mo enviado por parametro sobre el cual se realiza el rollback_alloc.
 * @param[in]  pos_fail  Es la posicion o numero de la fila en la que se produjo la falla de asignacion de memoria.
 * A partir de esta posicion, toda la memoria anterior a pos_fail debe ser memoria para liberar antes de notificar un
 * error de asignacion en este punto del objeto matrix_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rollback_alloc(int **p_m, size_t pos_fail, int p_mode)
{
	size_t j;
	
	/*
	 * Libero reserva de memoria en parte de columnas de matriz de p_mo, antes de pos_fail posicion donde
	 * se produce la falla de asignacion de memoria.
	 */
	for(j=0; j<pos_fail; j++)
	{
		free(p_m[j]);
	}

	/* Liberar memoria para arreglo de punteros con cada una de las filas de matriz de p_mo */
	free(p_m);
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de cambiar el modo de reserva de memoria dinamica para la creacion de un componente vector_o p_vo
 * enviado por parametro.
 *
 * @param[in]  p_vo    Es el objeto vector_o enviado por parametro sobre el cual se desea realizar el cambio.
 * @param[in]  p_mode  Es el modo de reserva de memoria dinamica que se desea utilizar para la creacion del objeto vector_o p_mo enviado
 * por parametro. Los modos posibles son los definidos en la enumeracion MEM_ALLOC_MODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_Vmem_alloc_mode(vector_o *p_vo, int p_mode)
{
	/* Mientras objeto matrix p_mo no haya sido creado, se puede modificar el modo de reserva de memoria. Caso contrario no sera posible.*/
	if(p_vo->mc == OBJ_NO_CREATED) 
	{
		if(valid_alloc_mode(p_mode) == VALID_MODE)
		{
			p_vo->alloc_mode = p_mode;
		}
	}
	else
	{
		printf("LIB_MatrixmodG_error: No es posible cambiar el modo de reserva de memoria para el componente %s, el componente ya esta creado !!!\n", p_vo->name);
	}
}


/*
* Descripcion de funcion: 
* Parametros:
* @param **x: Doble puntero de matriz de incidencia.
*/
void importar_matriz(matrix_o *p_mo, char *pfname)
{	
	// Se determinan filas columnas de matriz desde archivo	pfname
	leer_fc_matriz(p_mo, pfname);
	
	// Con las filas y columnas leidas se aloja memoria dinamicamente a matriz de objeto
	//x->matriz = crear_matriz(p_mo);
	crear_matriz_cero(p_mo, p_mo->filas, p_mo->columnas);
	
	FILE *fp;
    char buffer[FILE_SIZE_MAX]; // Arreglo que almacenara cada linea de archivo leido (fn)
	char *fname = pfname;
	int filas = 0; //
	int columnas = 0; //
	char *aux;
  	
    fp= fopen(fname,"r");// se abre archivo de nombre fname
    if( !fp ) {
	   	printf("Error al intetar abrir archivo %s \n", fname);
	   	return;
	   	//exit(1);
	}
	
	/*Leer linea por linea del contenido del archivo(fn) en el buffer */
   	while (fgets(buffer, FILE_SIZE_MAX, fp) != NULL) {
		aux = strtok(buffer, " "); // guardamos primera linea
		p_mo->matriz[filas][columnas] = atoi(aux);
		columnas = 1;
		
		while((aux = strtok( NULL, " ")) != NULL){   // Posteriores llamadas, se avanza datos entre espacios " " dentro de una misma fila.
			p_mo->matriz[filas][columnas] = atoi(aux);
			columnas++;
		}
   		columnas = 0;
		filas++;
   	}
   	fclose(fp); // cierra archivo
	
   	printf("\n   --> Se cargo exitosamente %s desde (%s).\n", p_mo->name, fname);
}


/*
* Descripcion de funcion: 
* Parametros:
* @param *x:
*/
void leer_fc_matriz(matrix_o *p_mo, char *pfname)
{
	FILE *fp;
    char buffer[FILE_SIZE_MAX]; // Arreglo que almacenara los datos del archivo leido (fn)
	char *fname = pfname;
	int filas = 0; //
	int columnas = 0; //
	char *aux;
    	
    fp= fopen(fname,"r");// se abre archivo de nombre fname
   	if( !fp ){
      	printf("\n   --> Error al intetar abrir %s \n\n   --> Finalice el programa.\n\n", fname);
      	return;
      	//exit(1);
	}
	
   	printf( "\n   --> Leyendo filas y columnas de matriz de incidencia (%s)...\n", fname);
	
	/* Leer linea por linea del contenido del archivo(fn) en el buffer */
   	while (fgets(buffer, FILE_SIZE_MAX, fp) != NULL)
		filas++;

   	fclose(fp); /* cierra archivo */	
	
	aux = strtok(buffer, " "); /* analizao ultima linea */
	columnas = 1;

	while( (aux = strtok( NULL, " ")) != NULL ) /* Posteriores llamadas, se le caracter por caracter luego de espacio " ". */
	 columnas++;
	
	p_mo->filas = filas;
	p_mo->columnas = columnas;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion crea la matriz del objeto p_mo enviado por parametro, asignandole la memoria necesaria para la matriz indicada
 * por parametro e inicializando todos sus valores a cero.
 *
 * @param      p_mo        Puntero a la direccion de memoria del objeto matrix_o en donde se reserva la memoria para la matriz y se 
 * asignan los correspondientes valores.
 * @param[in]  p_filas     Indica el numero de filas a reservar en la matriz del objeto p_mo.
 * @param[in]  p_columnas  Indica el numero de columnas a reservar en la matriz del objeto p_mo.
 *
 * @return     El retorno puede ser:
 * 				CREATE_OK: Si la creacion de la matriz fue exitosa. La creacion equivale a la reserva de memoria y la asignacion de
 * 				todos los valores a cero en la matriz.
 * 				CREATE_ERROR: Si la creacion de la matriz no fue exitosa. Por un fallo en el envio de parametros o en la reserva de
 * 				memoria para la matriz.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int crear_matriz_cero(matrix_o *p_mo, int p_filas, int p_columnas)
{
	/* Si matriz no existe, se reserva memoria para filas y columnas de matriz */
    if( (p_filas > 0) && (p_columnas > 0) && (p_mo->mc != OBJ_CREATED)){
		// Se asigna numero de filas y columnas de matriz segun parametros
		p_mo->filas = p_filas;
		p_mo->columnas = p_columnas;

        /* Con las filas y columnas asignadas en matriz, alojo memoria dinamicamente */
        if(p_mo->mo_methods->alloc_matrix_mem(p_mo) == ALLOC_OK)
        {
	        if( MV_OBJECT_MESSAGES == MESSAGES_ON)
	        {
	        	printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d filas de %s!!!\n", p_filas, p_mo->name);
        		printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d columnas de %s!!!\n", p_columnas, p_mo->name);
        	}

			p_mo->mc= OBJ_CREATED; /* marca de objeto matriz creada. */

			return CREATE_OK;
		}
	}
	return CREATE_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion crea la matriz del objeto p_mo enviado por parametro, asignandole la memoria necesaria para la matriz indicada
 * por parametro e inicializando todos sus valores a uno.
 *
 * @param      p_mo         Puntero a la direccion de memoria del objeto matrix_o en donde se reserva la memoria para la matriz y se
 * asignan los correspondientes valores.
 * @param[in]  p_filas     Indica el numero de filas a reservar en la matriz del objeto p_mo.
 * @param[in]  p_columnas  Indica el numero de columnas a reservar en la matriz del objeto p_mo.
 *
 * @return     El retorno puede ser:
 * 				CREATE_OK: Si la creacion de la matriz fue exitosa. La creacion equivale a la reserva de memoria y la asignacion de
 * 				todos los valores a cero en la matriz.
 * 				CREATE_ERROR: Si la creacion de la matriz no fue exitosa. Por un fallo en el envio de parametros o en la reserva de
 * 				memoria para la matriz.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int crear_matriz_uno(matrix_o *p_mo, int p_filas, int p_columnas)
{
	int i, j;
	/* Si matriz no existe, asignamos filas y columnas a matriz */
    if( (p_filas > 0) && (p_columnas > 0) && (p_mo->mc != OBJ_CREATED)){
		// Se asigna numero de filas y columnas de matriz segun parametros
		p_mo->filas = p_filas;
		p_mo->columnas = p_columnas;

        /* Con las filas y columnas asignadas en matriz, alojo memoria dinamicamente */
        if(p_mo->mo_methods->alloc_matrix_mem(p_mo) == ALLOC_OK)
        {
        	/* Asigno todos los valores de la matrixz a uno. */
			for(i = 0; i<p_mo->filas; i++) {
				for(j = 0; j<p_mo->columnas; j++)
					p_mo->matriz[i][j] = 1;
	        }
	        
	        if( MV_OBJECT_MESSAGES == MESSAGES_ON)
	        {
	        	printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d filas de %s!!!\n", p_filas, p_mo->name);
        		printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d columnas de %s!!!\n", p_columnas, p_mo->name);
        	}

			p_mo->mc= OBJ_CREATED; /* marca de matriz de objeto creada. */

			return CREATE_OK;
		}
	}
	return CREATE_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de crear una matriz identidad, asignando memoria al doble puntero del objeto matrix_o p_mo 
 * enviado por parametro y asignando los valores de acuerdo a la matriz identidad.
 *
 * @param      p_mo         Puntero a la direccion de memoria del objeto matrix_o en donde se reserva la memoria para la matriz y se
 * asignan los correspondientes valores.
 * @param[in]  p_dimension  Dimension que debe tener la matriz identidad a crear.
 *
 * @return     El retorno puede ser:
 * 				CREATE_OK: Si la creacion de la matriz fue exitosa. La creacion equivale a la reserva de memoria y la asignacion de
 * 				todos los valores a cero en la matriz.
 * 				CREATE_ERROR: Si la creacion de la matriz no fue exitosa. Por un fallo en el envio de parametros o en la reserva de
 * 				memoria para la matriz.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int crear_matriz_identidad(matrix_o *p_mo, int p_dimension)
{
	int i, j;

	/* Si matriz no existe, asignamos filas y columnas a matriz */
    if( (p_dimension > 0) && (p_mo->mc != OBJ_CREATED)){
		// Se asigna numero de filas y columnas de matriz segun parametros
		p_mo->filas = p_dimension;
		p_mo->columnas = p_dimension;

        /* Con las filas y columnas asignadas en matriz, alojo memoria dinamicamente */
        if(p_mo->mo_methods->alloc_matrix_mem(p_mo) == ALLOC_OK)
        {
        	/* Asigno todos los valores de la matriz a cero a exepcion de i=j en uno. */
			for(i = 0; i < p_mo->filas; i++) {
				for(j = 0; j < p_mo->columnas; j++) {
					if(i==j){
						p_mo->matriz[i][j] = 1;
						break;
					}
					
					/*else
						p_mo->matriz[i][j] = 0;*/
				}
			}
	        
	        if( MV_OBJECT_MESSAGES == MESSAGES_ON)
	        {
	        	printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d filas de %s!!!\n", p_dimension, p_mo->name);
        		printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d columnas de %s!!!\n", p_dimension, p_mo->name);
        	}

			p_mo->mc= OBJ_CREATED; /* marca de matriz de objeto creada. */

			return CREATE_OK;
		}
	}
	return CREATE_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de cargar una matriz con un valor indicado en el parametro p_valor de acuerdo a las filas y
 * columnas indicadas por parametro, la matriz se envia por parametro como un doble puntero (**p_m).
 *
 * @param      p_m         Es el doble puntero que referencia a la matriz a rellenar.
 * @param[in]  p_filas     Es el numero de filas de la matriz.
 * @param[in]  p_columnas  Es el numero de columnas de la matriz.
 * @param[in]  p_valor     Es el valor que se utiliza para rellenar a todos los elementos de la matriz.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rellenar_matriz(int **p_m, int p_filas, int p_columnas, int p_valor)
{
	int i, j;
	if((p_filas > 0) && (p_columnas > 0) ) {
		for(i = 0; i<p_filas; i++) {
			for(j = 0; j<p_columnas; j++)
				p_m[i][j] = p_valor;
		}
	}	
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de asignar a cero todos los elementos de la matriz del objeto matrix_o enviado por parametro.
 *
 * @param      p_mo     Puntero a la direccion de memoria donde se encuentra el objeto matrix_o que se desea modificar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void limpiar_matriz(matrix_o *p_mo)
{
	int i, j;
	if(p_mo->filas > 0 && p_mo->columnas > 0) {
		for(i = 0; i<p_mo->filas; i++) {
			for(j = 0; j<p_mo->columnas; j++)
				p_mo->matriz[i][j] = 0;
		}
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de convertir todos los elementos de la matriz enviada por parametro de cero a uno y el resto de
 * elementos diferentes a cero se convierten a cero.
 *
 * @param      p_mo     Puntero a la direccion de memoria del objeto matrix_o al que se desea negar los elementos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void negar_matriz(matrix_o *p_mo)
{
	int i, j;

	if((p_mo->filas > 0) && (p_mo->columnas > 0)) {
		for(i = 0; i < p_mo->filas; i++) {
			for(j = 0; j < p_mo->columnas; j++) {
				if(p_mo->matriz[i][j]==0)
					p_mo->matriz[i][j] = 1;
				else
					p_mo->matriz[i][j] = 0;
			}
		}
	}
	else
		printf( "LIB_MatrixmodG_error: No se puede negar matriz %s.\n", p_mo->name);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE FUNCIONES DE OBJETO vector_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion es el contructor de un objeto vector_o. Se encarga de inicializar todos sus elementos a valores por defecto.
 *
 * @param      p_vo  Puntero a la direccion de memoria del objeto vector_o que se desea inicializar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void new_vector(vector_o *p_vo, char * p_name)
{
	if(p_vo->mc != OBJ_CREATED) /* Si objeto vector_o p_vo no fue creado previamente. Se inicializan sus campos.*/
	{
		/* Inicializo por defecto los campos del objeto. */
		p_vo->elements= 0;
		p_vo->tam_bytes= 0;
		p_vo->mc= OBJ_NO_CREATED;
		p_vo->alloc_mode= M_MALLOC;
		 /*p_vo->vector: El puntero se inicializa en NULL por defecto segun estandar de C. Se asignara una direccion cuando se
		 * crea el vector del objeto con la funcion crear_vector_zero/uno en conjunto con crear_vector. */
		if(p_name != NULL)
		{
			strcpy(p_vo->name, p_name);
		}
		else
		{
			strcpy(p_vo->name, "-Sin definir-");
		}

		/* Asigno metodos a la vtable del objeto. */
		p_vo->vo_methods= &vector_vtable;
	}
	else
	{
		printf("LIB_MatrixmodG_error: Error al inicializar nuevo componente, el componente %s ya fue inicializado !!!\n", p_vo->name);
	}
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion aloja memoria en el vector perteneciente al objeto vector_o p_vo enviado por parametro.
 * 
 * @param      p_vo         Puntero a la direccion de memoria del objeto vector_o en donde se reserva la memoria para el vector.
 *
 * @return     El retorno sera:
 * 			ALLOC_OK: cuando la reserva de memoria para el vector del objeto vector_o p_vo se realizo exitosamente.
 * 			ALLOC_ERROR: cuando fallo la reserva de memoria para el vector del objeto vector_o p_vo.
 * 			
 * @note       La funcion es static ya que sera utilizada solo por funciones de este archivo. Desde otro archivo no se puede llamar a esta funcion.
 * Se trata de una funcion de gestion interna, para evitar errores.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int alloc_vector_mem(vector_o *p_vo)
{	
	int *p_vector; 			/* Puntero vector de asignacion dinamica. */
	size_t i = 0;
	size_t aux_i = 0;
	size_t aux_tam = 0;


	if(p_vo->vector==NULL){

		if(p_vo->alloc_mode == M_MALLOC) /* Si modo de resrva de memoria es M_VMALLOC: Realizo la reserva de memoria dinamica mediante vmalloc().*/
		{
			/* Reserva de memoria para elementos de p_vector (creacion de arreglo de enteros) */
			p_vector = (int *)malloc(p_vo->elements*sizeof(int));
			
			if(p_vector == NULL) /* Si falla la reserva.*/
			{
				return ALLOC_ERROR;
			}

			/* Rellena cada elemento del vector con cero. */
			rellenar_vector(p_vector, p_vo->elements, 0);

		}
		else if (p_vo->alloc_mode == M_CALLOC) /* Si modo de resrva de memoria es M_KZALLOC o M_kCALLOC: Realizo la reserva de memoria dinamica mediante kzalloc().*/
		{
			/* Reserva de memoria para elementos de p_vector (creacion de arreglo de enteros) */
			p_vector = (int *)calloc(p_vo->elements, sizeof(int));
			
			if(p_vector == NULL) /* Si falla la reserva.*/
			{
				return ALLOC_ERROR;
			}
		}
		else
		{
			printf("LIB_MatrixmodG_error: No se pudo reservar memoria. El componente %s no tiene asignado un modo valido de reserva de memoria!!!\n", p_vo->name);
			return ALLOC_ERROR;
		}

		aux_tam = p_vo->elements*sizeof(int);
		p_vo->vector=p_vector; 			   /* Guardo la direccion de memoria en la que se aloja mem, en puntero del vector de objeto vector_o. */
		p_vo->tam_bytes = aux_tam; 		   /* Guardo registro de cantidad de memoria reservada en el vector del objeto vector_o. */
		
		return ALLOC_OK;
	}
	
	return ALLOC_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de liberar la memoria reservada para el vector del objeto p_vo enviado por parametro.
 *
 * @param      p_vo  Puntero a la direccion de memoria del objeto vector_o sobre el que se desea realizar la liberacion de memoria.
 *
 * @return     El retorno es:
 * 				FREE_OK: Si la liberacion de memoria se realizo con exito.
 * 				FREE_ERROR: Si no se puede liberar memoria porque el puntero ya esta liberado o es null por no tener reservada memoria.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int free_vector_mem(vector_o *p_vo)
{
	int i = 0;
	if(p_vo->vector != NULL){
		
		/* Liberar memoria para elementos de vector de p_vo */
		free(p_vo->vector);

		p_vo->elements = 0;
		p_vo->tam_bytes = 0;
		p_vo->mc = OBJ_NO_CREATED;
		p_vo->vector = NULL; /* Se asigno null a la direccion del vector de objeto ya que no tiene mas memoria reservada.*/

		return FREE_OK;
	}
	
	return FREE_ERROR;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion crea el vector del objeto p_vo enviado por parametro. Se asigna la memoria necesaria al vector indicado
 * por parametro e inicializa todos sus valores a cero.
 *
 * @param      p_vo        Puntero a la direccion de memoria del objeto vector_o en donde se reserva la memoria para el vector y se 
 * asignan los correspondientes valores.
 * @param[in]  p_elements  Indica el numero de elementos a reservar en el vector del objeto p_vo.
 *
 * @return     El retorno puede ser:
 * 				CREATE_OK: Si la creacion del vector fue exitosa. La creacion equivale a la reserva de memoria y la asignacion de
 * 				todos los valores a cero en el vector.
 * 				CREATE_ERROR: Si la creacion del vector no fue exitosa. Por un fallo en el envio de parametros o en la reserva de
 * 				memoria para el vector.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int crear_vector_cero(vector_o *p_vo, int p_elements)
{
	/* Si vector no existe, asignamos elementos a objeto vector */
    if( (p_elements > 0) && (p_vo->mc != OBJ_CREATED)){
		/* Se asigna numero de elementos de vector segun parametros */
		p_vo->elements = p_elements;

        /* Con los elementos asignados en vecotr, alojo memoria dinamicamente */
        if(p_vo->vo_methods->alloc_vector_mem(p_vo) == ALLOC_OK)
        {
	        if( MV_OBJECT_MESSAGES == MESSAGES_ON)
	        {
	        	printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d elementos de %s!!!\n", p_elements, p_vo->name);
	        }

			p_vo->mc= OBJ_CREATED; /* marca de objeto vector creado. */

			return CREATE_OK;
		}
	}
	return CREATE_ERROR;
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion crea el vector del objeto p_vo enviado por parametro. Se asigna la memoria necesaria al vector indicado
 * por parametro e inicializa todos sus valores a cero.
 *
 * @param      p_vo        Puntero a la direccion de memoria del objeto vector_o en donde se reserva la memoria para el vector y se 
 * asignan los correspondientes valores.
 * @param[in]  p_elements  Indica el numero de elementos a reservar en el vector del objeto p_vo.
 *
 * @return     El retorno puede ser:
 * 				CREATE_OK: Si la creacion del vector fue exitosa. La creacion equivale a la reserva de memoria y la asignacion de
 * 				todos los valores a cero en el vector.
 * 				CREATE_ERROR: Si la creacion del vector no fue exitosa. Por un fallo en el envio de parametros o en la reserva de
 * 				memoria para el vector.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int crear_vector_uno(vector_o *p_vo, int p_elements)
{
	int i;
	/* Si vector no existe, asignamos elementos a objeto vector */
    if( (p_elements > 0) && (p_vo->mc != OBJ_CREATED)){
		// Se asigna numero de elementos de vector segun parametros
		p_vo->elements = p_elements;

        /* Con los elementos asignados en vecotr, alojo memoria dinamicamente */
        if(p_vo->vo_methods->alloc_vector_mem(p_vo) == ALLOC_OK)
        {
			for(i = 0; i<p_vo->elements; i++){
					p_vo->vector[i] = 1;
	        }

	        if(MV_OBJECT_MESSAGES == MESSAGES_ON)
	        {
	            printf( "LIB_MatrixmodG_info: Se reservo exitosamente memoria para %d elementos de %s!!!\n", p_elements, p_vo->name);
	        }
			p_vo->mc= OBJ_CREATED; /* marca de objeto vector creado. */

			return CREATE_OK;
		}
	}
	return CREATE_ERROR;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de asignar a cero todos los elementos del vector del objeto vector_o enviado por parametro.
 *
 * @param      p_mo     Puntero a la direccion de memoria donde se encuentra el objeto vector_o que se desea modificar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void limpiar_vector(vector_o *p_vo)
{
	int i;
	if((p_vo->vector != NULL)) {
		for(i = 0; i<p_vo->elements; i++) {
			p_vo->vector[i] = 0;
		}
	}
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de convertir todos los elementos del vector enviado por parametro de cero a uno y el resto de
 * elementos diferentes a cero se convierten a cero.
 *
 * @param      p_mo     Puntero a la direccion de memoria del objeto vector_o al que se desea negar los elementos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void negar_vector(vector_o *p_vo)
{
	int i;

	if(p_vo->vector != NULL) {
		for(i = 0; i < p_vo->elements; i++) {
			if(p_vo->vector[i]==0)
				p_vo->vector[i] = 1;
			else
				p_vo->vector[i] = 0;
		}
	}
	else
		printf( "LIB_MatrixmodG_error: No se puede negar vector %s.\n", p_vo->name);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion llena con un valor entero cada uno de las posiciones del vector del objeto p_vo enviado por parametro.
 *
 * @param      p_v         Puntero a la direccion de memoria de un vector de enteros.
 * @param[in]  p_elements  Numero de elementos del vector a llenar con el numero entero p_valor enviado por parametro.
 * @param[in]  p_valor     Es el numero entero con el que se rellenan todas las posiciones del vector del objeto p_vo.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void rellenar_vector(int *p_v, int p_elements, int p_valor)
{
	int i;
	if((p_v != NULL) && (p_elements>0)) {
		for(i = 0; i<p_elements; i++) {
				p_v[i] = p_valor;
		}
	}
}


/**
 * @brief      	Esta funcion carga un literal en todos los elementos de un vector_o enviado por parametro.
 *
 * @param      p_vo       Puntero a la direccion de memoria del objeto vector_o sobre el que se desea cargar el literal en sus elementos.
 * @param[in]  p_literal  Es un numero entero literal que se carga en cada elemento del vector_o.
 */
void CargarLiteral_vector(vector_o *p_vo, int p_literal)
{
	int i;
	if((p_vo->vector != NULL) && (p_vo->elements>0)) {
		for(i = 0; i<p_vo->elements; i++) {
				p_vo->vector[i] = p_literal;
		}
	}
}

// int main(int argc, char const *argv[])
// {
// 	/* code */
// 	return 0;
// }