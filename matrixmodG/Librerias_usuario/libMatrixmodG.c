#include "libMatrixmodG.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE VARIABLES GLOBALES MANEJADAS POR LOS OBJETOS DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Declaracion global de vtable para objeto DriverRDPG_o. Todos los objetos DriverRDPG_o tendran los mismos metodos. */
struct DriverRDPG_methods DriverRDPG_vtable = 
{
	.connect_driverMatrixmodG = connect_driverMatrixmodG, 
	.disconnect_driverMatrixmodG = disconnect_driverMatrixmodG,
	.matrixmodG_createRDPG = matrixmodG_createRDPG,
	.matrixmodG_addm = matrixmodG_addm,
	.matrixmodG_addv = matrixmodG_addv,
	.matrixmodG_delRDPG = matrixmodG_delRDPG,
	.matrixmodG_confirmRDPG = matrixmodG_confirmRDPG,
	.view_RDPGinfo = view_RDPGinfo,
	.view_allCompRDPG = view_allCompRDPG,
	.view_compRDPG = view_compRDPG,
	.view_allcmdRDPG = view_allcmdRDPG,
	.shoot_RDPG = shoot_RDPG,
	.delete_MatrixmodGDriver = delete_MatrixmodGDriver,
	.set_vG = set_vG,
	.get_TokensPlace = get_TokensPlace,

	/* Llamadas al sistema sobre driver matrixmodG. */
	.write_matrixmodG = write_matrixmodG,
	.read_matrixmodG = read_matrixmodG
};


//const char *cmd_structRDPG[N_CMD_STRUCT_RDPG] = {"create RDPG ","RDPG add ","RDPG confirm\n","RDPG delete\n"};
const char *cmd_add[ID_COMP_END] = {"RDPG add mII ","RDPG add mIH ","RDPG add mIR ","RDPG add mIRe ","RDPG add vMI "};
const char *cmd_RDPGinfo[ID_VIEW_END] = {"RDPGinfo name\n","RDPGinfo places\n","RDPGinfo transitions\n","RDPGinfo shots\n","RDPGinfo memory\n","RDPGinfo comp\n",
"RDPG cat mII\n","RDPG cat mIH\n","RDPG cat mIR\n","RDPG cat mIRe\n","RDPG cat mD\n",
"RDPG cat vMI\n","RDPG cat vMA\n","RDPG cat vMN\n","RDPG cat vQ\n", "RDPG cat vW\n","RDPG cat vE\n",
"RDPG cat vB\n","RDPG cat vL\n","RDPG cat vG\n","RDPG cat vA\n","RDPG cat vUDT\n","RDPG cat vEx\n", "RDPG get tokens "};

const char *cmd_createRDPG = "create RDPG ";
const char *cmd_delRDPG = "RDPG delete\n";
const char *cmd_confirmRDPG = "RDPG confirm\n";
const char *cmd_shootRDPG = "RDPG shoot ";
const char *cmd_set_vG = "RDPG set_vG ";

char libCadena[USR_BUF_SIZE];

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE FUNCIONES DE OBJETO DriverRDPG_o 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion es el constructor de cualquier objeto del tipo DriverRDPG_o que se envie por parametro. La funcion se encarga de inicializar
 * todos los atributos a los valores por defecto definidos y hacerle conocer al objeto todas sus funcionalidades disponibles.
 * Tambien se solicita la importacion de los archivos de la RDPG cuyos nombres se envian por parametros almacenandolos en objetos temporales gestionados
 * por el objeto p_DriverObj.
 * Luego de la importacion, se procede a establecer la conexion con el driver MatrixmodG previamente instalado en el kernel.
 * Si la conexion es exitosa se solicita a driver MatrixmodG crear la RDPG de los archivos importados, enviando la cantidad de plazas y transiciones de
 * la RDPG y luego cada uno de los elementos de cada componente base de la RDPG, por ultimo se envia una confirmacion de la RDPG para que esta establezca
 * por defecto el resto de sus componentes en el kernel de Linux.
 * 
 * @note: Para los casos que solo requieran conexion directa con el driver sin importar la RDPG desde archivos se debera enviar NULL en todos los 
 * parametros de nombres de archivos. Por ejemplo: new_MatrixmodGDriver(&driver_obj, NULL, NULL, NULL, NULL, NULL);
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que se desea inicializar.
 * @param      p_mII        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia I.
 * @param      p_mIH        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia H.
 * @param      p_mIR        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia R.
 * @param      p_mIRe       Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia Re.
 * @param      p_vMI        Puntero a la cadena de caracter que contiene el nombre del archivo del vector de marcado inicial vMI.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void new_MatrixmodGDriver(DriverRDPG_o *p_DriverObj, char *p_mII, char *p_mIH, char *p_mIR, char *p_mIRe, char *p_vMI)
{
	if(p_DriverObj->mc != OBJ_CREATED) /* Si objeto matrix_o p_mo no fue creado previamente. Se inicializan sus campos.*/
	{
		/* Inicializo por defecto los campos del objeto. */
		p_DriverObj->methods = &DriverRDPG_vtable;					/* Primero que todo hago conocer al objeto todas sus funcionalidades disponibles. */
		p_DriverObj->LIBMATRIXMODG_MESSAGES = MESSAGES_OFF; 		/* Deshabilito por defecto mensajes de libreria. */
		p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP = MESSAGES_OFF; 		/* Deshabilito por defecto mensajes de tiempos de operaciones. */

		/* Se inicializan campos de objetos matrix_o para luego almacenar los datos.*/
	    new_matrix(&(p_DriverObj->lib_mII), "< Matriz de incidencia I >");
	    new_matrix(&(p_DriverObj->lib_mIH), "< Matriz de incidencia H >");
	    new_matrix(&(p_DriverObj->lib_mIR), "< Matriz de incidencia R >");
	    new_matrix(&(p_DriverObj->lib_mIRe), "< Matriz de incidencia Re >");
	    new_matrix(&(p_DriverObj->lib_vMI), "< Vector de marcado inicial >");

	    /* Definicion de ID de objetos matrix_o. */
	    p_DriverObj->lib_mII.obj_id= mII;
	    p_DriverObj->lib_mIH.obj_id= mIH;
	    p_DriverObj->lib_mIR.obj_id= mIR;
	    p_DriverObj->lib_mIRe.obj_id= mIRe;
	    p_DriverObj->lib_vMI.obj_id= vMI;

		/* Se carga matrix_o con valores de acuerdo a archivos .txt de RDPG.*/
		importFiles_RDPG(p_DriverObj, p_mII, p_mIH, p_mIR, p_mIRe, p_vMI);

		/* Se establece conexion con file device de driver MatrixmodG. */
		p_DriverObj->connect_driver = DISABLED_CONNECTION; 		/* Inicia flag de conexion deshabilitada.*/
		connect_driverMatrixmodG(p_DriverObj);					/* Se establece conexion con driver.*/
		index_components(p_DriverObj);
		p_DriverObj->system_test_mode = 0; 						/* Inicia por defecto modo de pruebas de sistema desactivado.*/
		
		
		/* Medicion de tiempo de operaciones de driver. */
		p_DriverObj->timeop_omp = 0;
		p_DriverObj->timeop_ltime = 0;

		/* Creo RDPG con plazas y transiciones siempre que se haya importado matriz de incidencia I minimamente. */
		matrixmodG_createRDPG(p_DriverObj);
		
		/* Cargo valores de mII a RDPG de p_DriverObj matrixmodG */
		matrixmodG_addm(p_DriverObj,mII);
		
		/* Cargo valores de matriz de inicidencia H */
		matrixmodG_addm(p_DriverObj,mIH);

		/* Cargo valores de matriz de inicidencia R */
		matrixmodG_addm(p_DriverObj,mIR);

		/* Cargo valores de matriz de transiciones reset Re */
		matrixmodG_addm(p_DriverObj,mIRe);

		/* Cargo valores de vectorde marcado inicil vMI a RDPG de p_DriverObj matrixmodG */
		matrixmodG_addv(p_DriverObj,vMI);

		/* Confirmacion de componentes enviados a RDPG del kernel. al enviar este comando, la RDPG del kernel termina de calcular el resto de componentes.*/
		matrixmodG_confirmRDPG(p_DriverObj);

		if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
		{
			printf("\n	--> Tiempo de asignacion de memoria en kernel (con API openMP): %lf seg. \n", p_DriverObj->timeop_omp);
			printf("	--> Tiempo de asignacion de memoria en kernel (con API timer): %lf seg. \n\n", p_DriverObj->timeop_ltime);
		}

		p_DriverObj->mc= OBJ_CREATED;
	}
	else
	{
		printf("LIB_MatrixmodG_error: Error al inicializar objeto DriverRDPG_o. Verifique si el componente ya fue inicializado !!!\n");
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de importar los archivos de una RDPG desde los nombres enviados por parametros. Los datos importados desde los 
 * archivos se almacenan en los objetos respectivos de p_DriverObj. Para los casos en los que no sea necesario cargar un archivo, enviando NULL es 
 * suficiente para ignorarlo y no importarlo. Los datos se mantienen en el objeto p_DriverObj hasta que este sea eliminado.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o sobre el cual importar los datos de una RDPG.
 * @param      p_mII        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia I.
 * @param      p_mIH        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia H.
 * @param      p_mIR        Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia R.
 * @param      p_mIRe       Puntero a la cadena de caracter que contiene el nombre del archivo de la matriz de incidencia Re.
 * @param      p_vMI        Puntero a la cadena de caracter que contiene el nombre del archivo del vector de marcado inicial vMI.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void importFiles_RDPG(DriverRDPG_o *p_DriverObj, char *p_mII, char *p_mIH, char *p_mIR, char *p_mIRe, char *p_vMI)
{
	/* Carga de matrices desde archivos*/
	if(p_mII != NULL)
		p_DriverObj->lib_mII.mo_methods->import_matrix(&p_DriverObj->lib_mII,p_mII);
	
	if(p_mIH != NULL)
		p_DriverObj->lib_mIH.mo_methods->import_matrix(&p_DriverObj->lib_mIH,p_mIH);
	
	if(p_mIR != NULL)
		p_DriverObj->lib_mIR.mo_methods->import_matrix(&p_DriverObj->lib_mIR,p_mIR);
	
	if(p_mIRe != NULL)
		p_DriverObj->lib_mIRe.mo_methods->import_matrix(&p_DriverObj->lib_mIRe,p_mIRe);
	
	if(p_vMI != NULL)
		p_DriverObj->lib_vMI.mo_methods->import_matrix(&p_DriverObj->lib_vMI,p_vMI);

}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de realizar la conexion con el driver MatrixmodG por medio de su archivo de dispositivo en /proc.
 * Si la conexion es exitosa se guarda el file descriptor para su acceso hasta finalizar la conexion.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea realizar la conexion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void connect_driverMatrixmodG(DriverRDPG_o *p_DriverObj)
{
	/* Abrimos modulo matrixmodG*/
	p_DriverObj->my_fd = open("/proc/matrixmodG_fd", O_RDWR); /* abro file descriptor en modo escritura/lectura */

	if(p_DriverObj->my_fd<0 ) /* Verificacion de creacion. */
	{
		//error
		perror("	--> Error al abrir file descriptor /proc/matrixmodG_fd no existe.\n 	--> Finalizando programa.\n\n");
		disconnect_driverMatrixmodG(p_DriverObj);
		exit(1);
	}

	p_DriverObj->connect_driver = ENABLED_CONNECTION;
	printf("\n	--> Se establecio conexion con driver exitosamente. \n");
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de armar una tabla de indices ordenada para la gestion de los objetos de una RDPG importados desde los archivos.
 * De esta forma se puede gestionar de manera ordenada todos los datos de los archivos importados desde un arreglo de punteros denominado component[ID_COMP_END]. 
 * Siendo ID_COMP_END el numero de componentes que gestiona el arreglo component[] del objeto p_DriverObj.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o sobre el que se desea crear los indices de los componentes de la 
 * RDPG importada.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void index_components(DriverRDPG_o *p_DriverObj)
{
  
  p_DriverObj->component[mII] = &(p_DriverObj->lib_mII);
  if (p_DriverObj->lib_mII.mc == OBJ_NO_CREATED)
  	printf("	--> Cuidado: El indice de %s apunta a una matriz de usuario que no se importo desde ningun archivo. \n", p_DriverObj->lib_mII.name);
  
  p_DriverObj->component[mIH] = &(p_DriverObj->lib_mIH);
  if (p_DriverObj->lib_mIH.mc == OBJ_NO_CREATED)
  	printf("	--> Cuidado: El indice de %s apunta a una matriz de usuario que no se importo desde ningun archivo. \n", p_DriverObj->lib_mIH.name);

  p_DriverObj->component[mIR] = &(p_DriverObj->lib_mIR);
  if (p_DriverObj->lib_mIR.mc == OBJ_NO_CREATED)
  	printf("	--> Cuidado: El indice de %s apunta a una matriz de usuario que no se importo desde ningun archivo. \n", p_DriverObj->lib_mIR.name);

  p_DriverObj->component[mIRe] = &(p_DriverObj->lib_mIRe);
  if (p_DriverObj->lib_mIRe.mc == OBJ_NO_CREATED)
  	printf("	--> Cuidado: El indice de %s apunta a una matriz de usuario que no se importo desde ningun archivo. \n", p_DriverObj->lib_mIRe.name);

  p_DriverObj->component[vMI] = &(p_DriverObj->lib_vMI);
  if (p_DriverObj->lib_vMI.mc == OBJ_NO_CREATED)
  	printf("	--> Cuidado: El indice de %s apunta a una matriz de usuario que no se importo desde ningun archivo. \n", p_DriverObj->lib_vMI.name);

}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de desconectar el objeto DriverRDPG_o del driver MatrixmodG cargado en el kernel. Luego de la desconexion con el 
 * driver se elimina el objeto p_DriverRDPG enviado por parametro.
 * 
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea realizar la desconexion con el driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void disconnect_driverMatrixmodG(DriverRDPG_o *p_DriverObj)
{	

	if(p_DriverObj->connect_driver == ENABLED_CONNECTION) /* Si conexion esta establecida con driver. Se finaliza. */
	{
		close(p_DriverObj->my_fd); 							/* Cierro connexion con file descriptor a file device de matrixmodG. */
		p_DriverObj->connect_driver = DISABLED_CONNECTION; 	/* Flag de conexion en estado deshabilitada. */
	}

	delete_MatrixmodGDriver(p_DriverObj); /* Eliminacion de objeto. */
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de eliminar un objeto DriverRDPG_o enviado por parametro. La eliminacion libera los componentes de la RDPG importados
 * desde los archivos asociados en caso de existir. Para el caso de que no existan importaciones solo se cambia el flag de que el objeto ya no esta creado
 * con la macro OBJ_NO_CREATED.
 * 
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que se desea eliminar. 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void delete_MatrixmodGDriver(DriverRDPG_o *p_DriverObj)
{
	if(p_DriverObj->mc == OBJ_CREATED)
	{
		/* Elimino todos los componentes del objeto DriverRDPG_o. */
		if(p_DriverObj->lib_mII.mc == OBJ_CREATED)
		{
			delete_driverComponent(&p_DriverObj->lib_mII);
		}
		
		if(p_DriverObj->lib_mIH.mc == OBJ_CREATED)
		{
			delete_driverComponent(&p_DriverObj->lib_mIH);
		}
		
		if(p_DriverObj->lib_mIR.mc == OBJ_CREATED)
		{
			delete_driverComponent(&p_DriverObj->lib_mIR);
		}

		if(p_DriverObj->lib_mIRe.mc == OBJ_CREATED)
		{
			delete_driverComponent(&p_DriverObj->lib_mIRe);
		}

		if(p_DriverObj->lib_vMI.mc == OBJ_CREATED)
		{
			delete_driverComponent(&p_DriverObj->lib_vMI);
		}
		
		p_DriverObj->mc = OBJ_NO_CREATED;
	}
	else
	{
		printf("LIB_MatrixmodG_error: Error al eliminar objeto DriverRDPG_o. El objeto indicado no existe!!!\n");
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de realizar la liberacion de memoria de los componentes que gestiona un objeto DriverRDPG_o.
 * Como los componentes que gestiona un objeto DriverRDPG_o son matrix_o se envia por parametro el objeto matrix_o que se desea liberar.
 *
 * @param      p_mo  Puntero a la direccion de memoria del objeto matrix_o que se desea liberar la memoria.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void delete_driverComponent(matrix_o *p_mo)
{
  int aux_tam_bytes = p_mo->tam_bytes; /* Guardo memoria utilizada por p_mo antes de liberarla. */

  /* Liberamos memoria de p_mo*/
  if(p_mo->mo_methods->free_matrix_mem(p_mo) == FREE_OK)
      printf("LIB_MatrixmodG_info: Se libero [%d] bytes de componente %s.\n", aux_tam_bytes, p_mo->name);
  else if (p_mo->matriz != NULL)
      printf("LIB_MatrixmodG_error: No se pudo liberar memoria de componente %s. Fallo funcion de liberar memoria.\n", p_mo->name);
  else
      printf("LIB_MatrixmodG_info: El componente %s no reservo memoria o ya fue liberado.\n", p_mo->name);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar una solicitud de creacion de una nueva RDPG al driver matrixmodG para que inicie la creacion de la RDPG
 * en el kernel de Linux. El envio de la solicitud se realiza escribiendo en el file device asociado al modulo MatrixmodG.
 *              
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que contiene la RDPG en sus componentes para enviar al driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void matrixmodG_createRDPG(DriverRDPG_o *p_DriverObj)
{
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	if(p_DriverObj->lib_mII.mc == OBJ_CREATED) /* Si existe como minimo la matriz de incidencia I, se crea RDPG en kernel.*/
	{
		char comando[N_CMD];
		memset(comando, '\0', N_CMD);// se limpia cadena
	
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();
	
		// Se copia parametro a cadena comando
		//strcpy(comando, (char*)cmd_createRDPG);
		strcpy(comando, cmd_createRDPG);
	
		char numeros[N_CMD];
		memset(numeros, '\0', N_CMD);// se limpia cadena
		
		// Se carga en cadena numeros los numeros enteros correspondientes a las plazas y transiciones de la RDPG
		sprintf(numeros, "%d_%d", p_DriverObj->lib_mII.filas, p_DriverObj->lib_mII.columnas);
	
		// Se almacena todo en cadena comando para dejarlo completo
		strcat(comando, numeros);
	
		// Se crea matriz en modulo con comando write
		(void)write_matrixmodG(p_DriverObj, comando);
	
		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();
	
		if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
		{
			printf("	--> Envio a driver comando: %s\n", comando);
			printf("\n	--> Tiempo de asignacion de memoria en kernel (con API openMP): %lf seg. \n", (t_fin-t_ini));
			printf("	--> Tiempo de asignacion de memoria en kernel (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
		}
	
		printf("	--> Envio a driver comando: %s\n", comando);
	}
	
	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar al driver MatrixmodG, los elementos de una matriz o vector de la RDPG importada desde los archivos.
 * Los elementos a enviar se realizan desde uno de todos los componentes base de la RDPG, esto se indica mediante el parametro p_mcomp.
 * El envio de cada elemento del componente elegido se realiza escribiendo el comando de escritura al file device de driver MatrixmodG con los datos
 * requeridos de cada elemento en particular. Por ejemplo: 0_0_1, siendo fila_columna_valor donde fila es la posicion de fila de un componente matriz 
 * columna es la posicion de columna y valor es el valor que se desea asignar en las posicion indicada.
 * 
 * @note: Para el caso de vectores el driver MatrixmodG entiende dos modos:
 * 1. si se realiza el envio del comando mediante la posicion directa Pe: 0_1, posicion_valor.
 * 2. Tambien entiende si al vector se lo trata como una matriz por ejemplo 0_0_1. 0_posicion_valor. el primer 0 siempre se ignora ya que en un vector
 * representdo como matriz este valor siempre sera 0 (cero).
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o del cual se desea enviar sus componentes de RDPG al driver MatrixmodG
 * para que se actualicen en la RDPG del kernel de Linux.
 * @param[in]  p_mcomp       Numero entero que representa el componete de la RDPG que se desea enviar al driver MatrixmodG. Este entero no puede ser menor
 * que cero ni mayor que ID_COMP_END (ver enumeracion ID_COMPONENT).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void matrixmodG_addm(DriverRDPG_o *p_DriverObj, int p_mcomp)
{
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	if(p_DriverObj->component[p_mcomp]->mc == OBJ_CREATED) /* Si objeto fue importado previamente, se envia elementos asociados a driver. */
	{
		int i,j;
		char comando[N_CMD];
		memset(comando, '\0', N_CMD);// se limpia cadena
	
		if((p_mcomp >= ID_COMP_END) || (p_mcomp < 0))
		{
			printf("No existe el componente indicado. Ver macro ID_COMPOENET de libMatrixmodG.h.\n\n");
			return;
		}
		else if(p_DriverObj->component[p_mcomp]->mc == OBJ_NO_CREATED)
		{
			printf("La matriz %s no se importo desde archivo de texto.\n\n", p_DriverObj->component[p_mcomp]->name);
			return;	
		}
	
		/* Se copia parametro a cadena comando */
		strcpy(comando, cmd_add[p_mcomp]);
		char valor_aux[256];
		memset(valor_aux, '\0', N_CMD);// se limpia cadena
	
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();
		
		for(i = 0; i < p_DriverObj->component[p_mcomp]->filas; i++)
		{
			for (j = 0; j < p_DriverObj->component[p_mcomp]->columnas; j++)
			{
				if(p_DriverObj->component[p_mcomp]->matriz[i][j] != 0)
				{
					// Se carga en cadena_numeros los numeros enteros correspondientes a las filas y columnas
					sprintf(valor_aux, "%d_%d_%d", i, j, p_DriverObj->component[p_mcomp]->matriz[i][j]);
	
					// Se almacena todo en el comando para dejarlo completo
					strcat(comando, valor_aux);
	
					//if(p_DriverObj->system_test_mode == 1)
					printf("	--> Envio a driver comando: %s\n", comando);
					//debug_check_point(&I,&MI,&H,&R,fd, comando);
					// Se crea matriz en modulo con comando write
					(void)write_matrixmodG(p_DriverObj, comando);
	
					memset(comando, '\0', N_CMD);// se limpia cadena
	
					// Se copia parametro a cadena comando
					strcpy(comando, cmd_add[p_mcomp]);
					memset(valor_aux, '\0', N_CMD);// se limpia cadena
				}
			}
		}
		printf("	--> Envio de elementos de %s con exito.\n", p_DriverObj->component[p_mcomp]->name);
	}

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}


void matrixmodG_addv(DriverRDPG_o *p_DriverObj, int p_vcomp)
{
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	if(p_DriverObj->component[p_vcomp]->mc == OBJ_CREATED) /* Si objeto fue importado previamente, se envia elementos asociados a driver. */
	{
		int i;
		char comando[N_CMD];
		memset(comando, '\0', N_CMD);// se limpia cadena
	
		if((p_vcomp >= ID_COMP_END) || (p_vcomp < 0))
		{
			printf("No existe el componente indicado. Ver macro ID_COMPOENET de libMatrixmodG.h.\n\n");
			return;
		}
		else if(p_DriverObj->component[p_vcomp]->mc == OBJ_NO_CREATED)
		{
			printf("La matriz %s no se importo desde archivo de texto.\n\n", p_DriverObj->component[p_vcomp]->name);
			return;	
		}
	
		/* Se copia parametro a cadena comando */
		strcpy(comando, cmd_add[p_vcomp]);
		char valor_aux[256];
		memset(valor_aux, '\0', N_CMD);// se limpia cadena
	
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();
		
		for(i = 0; i < p_DriverObj->component[p_vcomp]->columnas; i++)
		{
			if(p_DriverObj->component[p_vcomp]->matriz[0][i] != 0)
			{
				// Se carga en cadena_numeros los numeros enteros correspondientes a las filas y columnas
				sprintf(valor_aux, "%d_%d", i, p_DriverObj->component[p_vcomp]->matriz[0][i]);

				// Se almacena todo en el comando para dejarlo completo
				strcat(comando, valor_aux);

				//if(p_DriverObj->system_test_mode == 1)
				printf("	--> Envio a driver comando: %s\n", comando);
				//debug_check_point(&I,&MI,&H,&R,fd, comando);
				// Se crea matriz en modulo con comando write
				(void)write_matrixmodG(p_DriverObj, comando);

				memset(comando, '\0', N_CMD);// se limpia cadena

				// Se copia parametro a cadena comando
				strcpy(comando, cmd_add[p_vcomp]);
				memset(valor_aux, '\0', N_CMD);// se limpia cadena
			}
		}
		printf("	--> Envio de elementos de %s con exito.\n", p_DriverObj->component[p_vcomp]->name);
	}

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar el comando de confirmacion al driver MatrixmodG. Con este comando la RDPG del kernel se encarga de actualizar
 * todos sus componentes restantes a sus valores iniciales.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o desde el cual se desea enviar el comando.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void matrixmodG_confirmRDPG(DriverRDPG_o *p_DriverObj)
{
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	if(p_DriverObj->lib_mII.mc == OBJ_CREATED)
	{
		(void)write_matrixmodG(p_DriverObj, (char *)cmd_confirmRDPG); /* Confirmacion de componentes enviado a RDPG del kernel. La RDPG del kernel termina de calcular el resto de componentes.*/
	}

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);	
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar el comando para eliminar la RDPG del kernel de Linux. El envio del comando se realiza escribiendo en el
 * file device del driver MatrixmodG, el driver al detectarlo realiza la eliminacion de la RDPG. 
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o desde el cual se desea realizar la eliminacion de la RDPG gestionada 
 * por el driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void matrixmodG_delRDPG(DriverRDPG_o *p_DriverObj)
{
	char cadena[N_CMD];
	memset(cadena, '\0', N_CMD);// se limpia cadena

	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	if(p_DriverObj->system_test_mode==0)
	{
		printf("\n   ¿Desea eliminar la RDPG cargada en el kernel? (yes/no) ");
		scanf( "%s", cadena );

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		if(strcmp(cadena, "yes") == 0)
		{
			printf("	--> Envio a driver comando: %s\n", cmd_delRDPG);

			// Se elimina RDPG en driver con comando write
			(void)write_matrixmodG(p_DriverObj, (char *)cmd_delRDPG);
		}

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();
	}
	else if(p_DriverObj->system_test_mode==1)
	{
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		// Se elimina RDPG en driver con comando write
		(void)write_matrixmodG(p_DriverObj, (char *)cmd_delRDPG);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();
	}

	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("\n	--> Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));	
	}

	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar el comando de solicitud de informacion de la RDPG cargada en el kernel de Linux. El envio del comando
 * se realiza mediante la escritura al file device del driver MatrixmodG. Luego de escribir dicho comando se realiza una lectura al file device el cual
 * devuelve la informacion solicitada en una cadena de espacio usuario.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea obtener la informacion de la RDPG cargada en el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void view_RDPGinfo(DriverRDPG_o *p_DriverObj)
{
	memset(libCadena, '\0', USR_BUF_SIZE);// se limpia cadena

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	view_compRDPG(p_DriverObj, (int)view_infoName, libCadena);
    view_compRDPG(p_DriverObj, view_infoPlaces, libCadena);
    view_compRDPG(p_DriverObj, view_infoTransitions, libCadena);
    view_compRDPG(p_DriverObj, view_infoShots, libCadena);
    view_compRDPG(p_DriverObj, view_infoMemory, libCadena);

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion TOTAL (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion TOTAL (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}

	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de obtener el estado de todos los componentes gestionados por la RDPG cargada en el kernel de Linux. Para esto 
 * se escribe en el file device del driver MatricmodG el comando correspondiente a cada componente y se lo obtiene realizando la lectura al file device.
 * Intercalando una escritura y lectura al file device por cada componente de la RDPG se logra obtener todos los estados de componentes de la RDPG 
 * cargada en el kernel.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea obtener la informacion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void view_allCompRDPG(DriverRDPG_o *p_DriverObj)
{
	int TIME_OFF =0;
	memset(libCadena, '\0', USR_BUF_SIZE);// se limpia cadena

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	printf("\n  Matriz de incidencia de la RDPG:\n" );
	
	view_compRDPG(p_DriverObj, view_mII, libCadena);

	printf("\n  Matriz de incidencia de brazos inhibidores de la RDPG:\n" );
	
    view_compRDPG(p_DriverObj, view_mIH, libCadena);

    printf("\n  Matriz de incidencia de brazos lectores de la RDPG:\n" );
    
    view_compRDPG(p_DriverObj, view_mIR, libCadena);

    printf("\n  Matriz de transiciones reset de la RDPG:\n" );
    
    view_compRDPG(p_DriverObj, view_mIRe, libCadena);

    printf("\n  Vectores de disparos posibles: \n" );
    
	view_compRDPG(p_DriverObj, view_mD, libCadena);

    printf("\n  Marcado inicial de la RDPG:\n" );
    
    view_compRDPG(p_DriverObj, view_vMI, libCadena);

	printf("\n  Marcado actual de la RDPG:\n" );
	
    view_compRDPG(p_DriverObj, view_vMA, libCadena);

    printf("\n  Marcado nuevo de la RDPG:\n" );
    
    view_compRDPG(p_DriverObj, view_vMN, libCadena);

    printf("\n  Vector Q de funcion cero: \n" );
    
	view_compRDPG(p_DriverObj, view_vQ, libCadena);

	printf("\n  Vector W de funcion uno: \n" );
	
	view_compRDPG(p_DriverObj, view_vW, libCadena);

	printf("\n  Vector E de transiciones sensibilizadas:\n" );
	
    view_compRDPG(p_DriverObj, view_vE, libCadena);

    printf("\n  Vector B de transiciones des-sensibilizadas por arco inhibidor:\n" );
    
    view_compRDPG(p_DriverObj, view_vB, libCadena);

    printf("\n  Vector L de transiciones des-sensibilizadas por arco lector:\n" );
    
    view_compRDPG(p_DriverObj, view_vL, libCadena);

    printf("\n  Vector G de guardas de transiciones:\n" );
    
    view_compRDPG(p_DriverObj, view_vG, libCadena);

    printf("\n  Vector A de transiciones de reseteo:\n" );
    
    view_compRDPG(p_DriverObj, view_vA, libCadena);

    printf("\n  Vector UDT de resultado de ultimo disparo de transiciones:\n" );
    
    view_compRDPG(p_DriverObj, view_vUDT, libCadena);

	printf("\n  Vector Ex de transiciones sensibilizadas extendido: \n" );
	
	view_compRDPG(p_DriverObj, view_vEx, libCadena);


	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion TOTAL (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion TOTAL (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}

	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de obtener el estado de un componente de la RDPG cargada en el kernel. El componete sobre el cual se desea conocer
 * su estado se indica por parametro. Para enviar el comando se realiza una escritura al file device del driver MatrixmodG, para obtener la informacion 
 * del estado del componete se realiza una lectura al file device.
 * 
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea obtener la informacion.
 * @param[in]  cmd_INFO_ID  Macro representativa al numero en donde se encuentra el comando asociado del componente que se desea obtener su estado.
 * @param      cadena       Puntero a una cadena de caracter del espacio usuario donde se almacena la informacion proporcionada por el driver MatrixmodG
 * desde el file device asociado.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void view_compRDPG(DriverRDPG_o *p_DriverObj, int cmd_INFO_ID, char *cadena)
{
	char comando_auxwr[N_CMD];
	memset(comando_auxwr, '\0', N_CMD);// se limpia cadena

	if( (cmd_INFO_ID < ID_VIEW_END) && (cmd_INFO_ID >= ID_VIEW_INIT))
		strncpy(comando_auxwr, cmd_RDPGinfo[cmd_INFO_ID], strlen(cmd_RDPGinfo[cmd_INFO_ID]));
	else
	{
		printf("LIB_MatrixmodG_error: Numero de comando indicado no existe en libreria. \n");
		return;
	}

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	/* Write sobre driver */
	if(write_matrixmodG(p_DriverObj, comando_auxwr) == 0)
	{
		/* Read fichero */
		(void)read_matrixmodG(p_DriverObj,cadena);
	}

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	printf("%s\n", cadena);
	
	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}

	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

	memset(cadena, '\0', USR_BUF_SIZE);// se limpia cadena
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de mostrar todos los comandos que se pueden enviar al driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void view_allcmdRDPG(void)
{
	size_t i;

	/* Comandos de creacion y eliminacion de la RDPG.*/
	printf("   - %s\n", cmd_createRDPG);
	printf("   - %s\n", cmd_delRDPG);
	printf("   - %s\n", cmd_confirmRDPG);

	/* Comandos de agregacion de elemnetos de la RDPG, */
	for(i=0; i<ID_COMP_END; i++)
	{
		printf("   - %s\n", cmd_add[i]);
	}

	/* Comando de disparo una transicion de la RDPG. */
	printf("   - %s\n", cmd_shootRDPG);
	
	/* Comandos de informacion de la RDPG y sus componentes. */
	for(i=0; i<ID_VIEW_END; i++)
	{
		printf("   - %s\n", cmd_RDPGinfo[i]);
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de enviar al driver MatrixmodG el comando de disparo de una transicion de la RDPG. El envio del comando se realiza
 * escribiendo en el archivo de dispositivo del driver MatrixmodG. La escritura sobre el archivo de dispositivo retorna 0 si el disparo fallo o 1 si el
 * disparo fue exitoso.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o desde el que se desea realizar el disparo de una transicion de la RDPG.
 * @param      cadena       Cadena de caracteres que contiene el comando con el numero de la transicion a disparar sobre la RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void shoot_RDPG(DriverRDPG_o *p_DriverObj, int p_transicion)
{
	char cadena_read[N_CMD]; 			/* cadena para almacenar el resultado del disparo. */
	char comando[N_CMD]; 				/* comando para realizar disparo*/
	char s_transition[N_BYTES];			/* cadena de caracter que almacena el numero entero de una transicion en ascii. */
	memset(cadena_read, '\0', N_CMD);	/* se limpia cadena */
	memset(comando, '\0', N_CMD);		/* se limpia cadena */

	itoa(p_transicion, s_transition, N_BYTES);		/* Conversion de numero entero a ascii. */

	strcat(comando, cmd_shootRDPG); 	/* se agrega comando de disparo. */
	strcat(comando, s_transition);		/* se agrega cadena(nro transicion) al final de comando */

	printf("   Cadena ingresada: %s\n\n", comando);

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini = omp_get_wtime();
	t_ini2 = clock();
	
	// Write sobre driver -> se realiza disparo
	(void)write_matrixmodG(p_DriverObj, comando);

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	// indicamos a driver que muestre resultado de ultimo disparo
	if(write_matrixmodG(p_DriverObj, "RDPGinfo shot_result\n") == 0)
	{
		// almacenamos cadena_read
		(void)read_matrixmodG(p_DriverObj,cadena_read);
	}

	printf("\n	-->	%s", cadena_read);

	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}

	p_DriverObj->timeop_omp = (t_fin-t_ini);
	p_DriverObj->timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de setear cualquiera de los elementos de vector vG de la RDPG del kernel. El valor que se desea setear se envia
 * 			   en una cadena de caracter enviada por parametro.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o desde el que se desea realizar el seteo.
 * @param      p_entrada    Puntero a la cadena de caracter con la posicion del elemento a setear y con el valor de seteo.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_vG(DriverRDPG_o *p_DriverObj, char *p_entrada)
{
	char comando_auxwr[N_CMD];
	memset(comando_auxwr, '\0', N_CMD);// se limpia cadena

	/* Se construye comando para enviar a driver. */
	strncpy(comando_auxwr, cmd_set_vG, strlen(cmd_set_vG));
	
	/* Se almacena todo en cadena comando_auxwr para dejarlo completo */
	strcat(comando_auxwr, p_entrada);

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	// Write sobre driver
	(void)write_matrixmodG(p_DriverObj, comando_auxwr);

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion obtiene el numero de tokens de una plaza de la RDPG del kernel. La plaza es indicada por la cadena de caracteres enviada por
 * 			   parametro.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o desde el que se desea realizar la consulta.
 * @param      p_entrada    Puntero a la cadena de caracter con el numero de plaza que se desea consultar.
 *
 * @return     El retorno es cero o mayor a cero si la funcion realiza la consulta con exito al driver.
 * 			   El retorno es < que cero si falla la consulta con el driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int get_TokensPlace(DriverRDPG_o *p_DriverObj, char *p_entrada)
{
	int n_tokens; /* variable que almacena el numero de tokens de la plaza indicada.*/
	char comando_auxwr[N_CMD];
	memset(comando_auxwr, '\0', N_CMD);// se limpia cadena

	/* Se construye comando para enviar a driver. */
	strncpy(comando_auxwr, cmd_RDPGinfo[get_Tokens], strlen(cmd_RDPGinfo[get_Tokens]));
	
	/* Se almacena todo en cadena comando_auxwr para dejarlo completo */
	strcat(comando_auxwr, p_entrada);

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();

	// Write sobre driver
	n_tokens = write(p_DriverObj->my_fd, comando_auxwr, strlen(comando_auxwr));
	if ( n_tokens < 0)
	{
		printf("LIB_MatrixmodG_error: Fallo la consulta de token en la plaza indicada. Analice comando dmesg para conocer mas detalles del error.\n");
	}

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	if(p_DriverObj->LIBMATRIXMODG_MSG_TIMEOP == MESSAGES_ON)
	{
		printf("	--> Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
		printf("	--> Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));
	}

	return n_tokens;
}


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
		printf("LIBMatrixmodG_error: Falla en conversion de entero a ascci (funcion itoa()). \n");
	}
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de escribir sobre el archivo de dispositivo (o file device) del driver MatrixmodG. La escritura se realiza utilizando
 * el file descriptor que contiene el objeto p_DriverObj enviado por parametro para establecer la conexion con el driver.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea escribir en el file device del driver MatrixmodG.
 * @param      cadena       Puntero a la cadena de caracteres que se desean escribir en el file device del driver MatrixmodG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int write_matrixmodG(DriverRDPG_o *p_DriverObj, char *cadena)
{	
	int id_thread;	
	omp_set_num_threads(2);

	#pragma omp parallel private(id_thread)
    {
 
    id_thread = omp_get_thread_num();
	
    if(id_thread == 1) /* Hilo 1 realiza la operacion. Se deja al hilo master (id_thread = 0) libre. */
	{ 
	    /* Write a file device de driver matrixmodG. */
		if (write(p_DriverObj->my_fd, cadena, strlen(cadena)) != strlen(cadena))
		{
			printf("Error de escritura sobre driver\n");
			// Close de files descriptors
			//close(p_DriverObj->my_fd);
			disconnect_driverMatrixmodG(p_DriverObj); /* Libera memoria utilizada y cierra la conexion con el driver (close(p_DriverObj->my_fd)). */
			exit(1);
		}
	}
 
    }//Fin Parallel

    return 0;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de leer el archivo de dispositivo (o file device) del driver MatrixmodG. La lectura se realiza utilizando el codigo 
 * del file descriptor que contiene el objeto p_DriverObj enviado por parametro con el que se establece la conexion con el driver.
 *
 * @param      p_DriverObj  Puntero a la direccion de memoria del objeto DriverRDPG_o que desea leer el file device del driver MatrixmodG.
 * @param      cadena       Puntero a la cadena de caracteres donde se almacena la lectura brindada por el driver MatrixmodG mediante el file device.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int read_matrixmodG(DriverRDPG_o *p_DriverObj, char *cadena)
{
	int id_thread;	
	omp_set_num_threads(2);

	#pragma omp parallel private(id_thread)
    {
 
    id_thread = omp_get_thread_num();
	
    if(id_thread == 1) /* Hilo 1 realiza la operacion. Se deja al hilo master (id_thread = 0) libre. */
	{ 
	    /* Read file device de driver MatrixmodG */
		if(read(p_DriverObj->my_fd, cadena, USR_BUF_SIZE) < 0)
		{
			printf("Error de lectura sobre driver\n");
			// Close de files descriptors
			//close(p_DriverObj->my_fd);
			disconnect_driverMatrixmodG(p_DriverObj); /* Libera memoria utilizada y cierra la conexion con el driver (close(p_DriverObj->my_fd)). */
			exit(1);
		}
	}
 
    }//Fin Parallel

    return 0;
}


/*int main()
{
	return 0;
}*/