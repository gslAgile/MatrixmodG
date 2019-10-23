#include "system_tests.h"

/* Funcionalidades para pruebas de sistema automatizadas */

void st_cargar_componentes(DriverRDPG_o *p_DriverObj)
{
	/* Carga de matrices desde archivos*/
	//new_MatrixmodGDriver(p_DriverObj, "./RDPG/identity_10x10/mII.txt", "./RDPG/identity_10x10/mIH.txt", "./RDPG/identity_10x10/mIR.txt", "./RDPG/identity_10x10/mIRe.txt", "./RDPG/identity_10x10/vMI.txt");
	new_MatrixmodGDriver(p_DriverObj, "../Caso_Aplicacion/mII.txt", "../Caso_Aplicacion/mIH.txt", "../Caso_Aplicacion/mIR.txt", "../Caso_Aplicacion/mIRe.txt", "../Caso_Aplicacion/vMI.txt");
	p_DriverObj->system_test_mode = 1; /* Se habilita modo de prueba sobre driver. */
}


void st_cargar_componentes_v2(DriverRDPG_o *p_DriverObj)
{
	/* Carga de matrices desde archivos*/
	new_MatrixmodGDriver(p_DriverObj, "./RDPG/identity_1000x1000/mII.txt", NULL, NULL, NULL, "./RDPG/identity_1000x1000/vMI.txt");
	p_DriverObj->system_test_mode = 1; /* Se habilita modo de prueba sobre driver. */
}


void st_liberar_componentes(DriverRDPG_o *p_DriverObj)
{
	
	p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);			/* Elimino RDPG cargada en kernel. */
	p_DriverObj->methods->disconnect_driverMatrixmodG(p_DriverObj); /* Cierro conexion con driver desde espacio usuario. */
}

/* Inicia vectores de muestra todos los valores a cero. */
void st_iniciar_vmuestra(void)
{
	size_t i;

	for(i=0; i < N_MUESTRAS; i++)
	{
		m_alloc_omp[i] = 0;
		m_alloc_time[i] = 0;
	}
}

/* Muestra el promedio de los vectores de muestra. Se puede elegir por uno u otro vector de acuerdo a la API de tiempo.*/
void st_promedio(void)
{
	double ptime_omp=0; 	/* promedio de tiempos de muestras en vector de omp. */
	double ptime_ltime=0; 	/* promedio de tiempos de muestras en vector de lib time. */
	size_t i;

	for(i=0; i < N_MUESTRAS; i++)
	{
		ptime_omp += m_alloc_omp[i];
		ptime_ltime += m_alloc_time[i];
	}

	printf("\n   PROMEDIOS PARA %d MUESTRAS: \n", N_MUESTRAS);
	printf("   Tiempo de operacion promedio con openMP: %lf\n", ptime_omp/N_MUESTRAS);
	printf("   Tiempo de operacion promedio con lib time: %lf\n", ptime_ltime/N_MUESTRAS);
}

/* Muestra los datos del vector con las muestras de tiempo con API openMP. */
void st_muestras(void)
{
	size_t i;

	printf("\n   Muestras de tiempo de operacion con openMP:\n");
	for(i=0; i < N_MUESTRAS; i++)
		printf("   %lf\n",m_alloc_omp[i]);

	printf("\n   Muestras de tiempo de operacion con lib time:\n");
	for(i=0; i < N_MUESTRAS; i++)
		printf("   %lf\n",m_alloc_time[i]);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------* 
 * Casos de prueba sistemicos automatizados 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* 
 * Esta prueba mide cuanto tiempo demanda la asignación de memoria en el driver para crear una RDPG cargándola desde un archivo de texto (extensión .txt).
 */
void CPS201_alloc_mem(DriverRDPG_o *p_DriverObj)
{
	size_t i;

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;

		/* Elimino RDPG del kernel. */
		p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);
	}
}

/* 
 * Esta prueba mide cuanto tiempo demanda la asignación de memoria en el driver MatrixmodG para una RDPG np x nt (plazas transiciones indicadas por 
 * parámetro). Se prueba la creación de redes con plazas y transiciones iniciando por las decenas, centenas y llegando hasta el límite de 1000 plazas
 * y transiciones. 
 */
void CPS201_alloc_mem_nxm(DriverRDPG_o *p_DriverObj, const int p_nplazas, const int p_ntransiciones)
{
	size_t i;

	/* Simulo una matriz creada. */
	p_DriverObj->lib_mII.filas = p_nplazas;
	p_DriverObj->lib_mII.columnas = p_ntransiciones;
	p_DriverObj->lib_mII.mc = OBJ_CREATED;

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;

		/* Elimino RDPG del kernel. */
		p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);
	}

	/* Restablesco datos de simulacion. */
	p_DriverObj->lib_mII.filas = 0;
	p_DriverObj->lib_mII.columnas = 0;
	p_DriverObj->lib_mII.mc = OBJ_NO_CREATED;	
}


/* 
 * Esta prueba mide cuanto tiempo demanda la asignación de valores para los componentes base de una RDPG cargada en el kernel. Se prueba cuanto demora 
 * el envió de datos al kernel para la asignación de todos los componentes bases de una RDPG.
 */
void CPS202_add_comps(DriverRDPG_o *p_DriverObj)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */
	
	p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);

	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Al medir solo el tiempo de asignacion de valores se resetea a cero nuevamente el tiempo. */
		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mII);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addv(p_DriverObj, vMI);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIH);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIR);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIRe);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		timeop_omp = 0;
		timeop_ltime = 0;
	}

	/* Elimino RDPG del kernel. */
	p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);
}


/* 
 * Esta prueba mide cuanto tiempo demanda al driver MatrixmodG la creación una RDPG de manera completa en el kernel.
 * Esta operacion implica:
 * - Asignacion de memoria de todos los componentes base e internos. 
 * - Asignacion de valores de todos los componentes base e internos de la RDPG.
 */
void CPS203_create_rdp(DriverRDPG_o *p_DriverObj)
{
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; /* tiempo de operacion desde libreria time. */
	size_t i;

	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;


	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();


		/* Asignacion de memoria de todos los componetes base e interno de la RDPG. */
		p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		/* Asignacion de todos los valores de los componetes base e internos de la RDPG. */
		/*p_DriverObj->methods->matrixmodG_add(p_DriverObj, mII);

		p_DriverObj->methods->matrixmodG_add(p_DriverObj, vMI);

		p_DriverObj->methods->matrixmodG_add(p_DriverObj, mIH);

		p_DriverObj->methods->matrixmodG_add(p_DriverObj, mIR);

		p_DriverObj->methods->matrixmodG_add(p_DriverObj, mIRe);

		p_DriverObj->methods->matrixmodG_confirmRDPG(p_DriverObj);*/ /* Confirmacion de componentes enviado a RDPG del kernel. La RDPG del kernel termina de calcular el resto de componentes.*/

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mII);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addv(p_DriverObj, vMI);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIH);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIR);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_addm(p_DriverObj, mIRe);
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		p_DriverObj->methods->matrixmodG_confirmRDPG(p_DriverObj); /* Confirmacion de componentes enviado a RDPG del kernel. La RDPG del kernel termina de calcular el resto de componentes.*/
		timeop_omp += p_DriverObj->timeop_omp;
		timeop_ltime += p_DriverObj->timeop_ltime;

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		//timeop_omp = (t_fin-t_ini);
		//timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i]=timeop_omp;
		m_alloc_time[i]=timeop_ltime;

		/* Elimino RDPG del kernel. */
		p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);

		/* Reseteo a cero el tiempo para proxima muestra. */
		timeop_omp=0;
		timeop_ltime=0;
	}
}


/* 
 * Esta prueba mide cuanto tiempo demanda al driver MatrixmodG la operación delete de una RDPG creada previamente.
 */
void CPS204_delete_rdp(DriverRDPG_o *p_DriverObj)
{
	size_t i;

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);

		p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;
	}
}


/* 
 * Esta prueba mide cuanto tiempo demanda al driver MatrixmodG la operación delete de una RDPG nxm creada previamente. 
 * Tanto n (plazas) como m (transiciones) se parametrizan iniciando desde las decenas hasta llegar a las 1000 plazas y transiciones que es el 
 * límite máximo que soporta el driver MatrixmodG.
 */
void CPS204_delete_rdp_nxm(DriverRDPG_o *p_DriverObj, const int p_nplazas, const int p_ntransiciones)
{
	size_t i;
	
	/* Simulo una matriz creada. */
	p_DriverObj->lib_mII.filas = p_nplazas;
	p_DriverObj->lib_mII.columnas = p_ntransiciones;
	p_DriverObj->lib_mII.mc = OBJ_CREATED;

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);

		p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i]= p_DriverObj->timeop_ltime;
	}

	/* Restablesco datos de simulacion. */
	p_DriverObj->lib_mII.filas = 0;
	p_DriverObj->lib_mII.columnas = 0;
	p_DriverObj->lib_mII.mc = OBJ_NO_CREATED;	
}


void CPS205_shoot(DriverRDPG_o *p_DriverObj)
{
	size_t i;

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */
	//p_DriverObj->methods->matrixmodG_createRDPG(p_DriverObj);
	//p_DriverObj->methods->matrixmodG_add(p_DriverObj, mII);
	//p_DriverObj->methods->matrixmodG_add(p_DriverObj, vMI);
	//p_DriverObj->methods->matrixmodG_confirmRDPG(p_DriverObj); /* Confirmacion de componentes enviado a RDPG del kernel. La RDPG del kernel termina de calcular el resto de componentes.*/

	for(i=0; i<N_MUESTRAS; i++)
	{
		int n_rand = rand()%(p_DriverObj->lib_mII.columnas);

		p_DriverObj->methods->shoot_RDPG(p_DriverObj, n_rand);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;
	}

	/* Elimino RDPG del kernel. */
	//p_DriverObj->methods->matrixmodG_delRDPG(p_DriverObj);
}


void CPS206_readAllComp(DriverRDPG_o *p_DriverObj)
{
	size_t i;

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->view_allCompRDPG(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;
	}
}


void CPS207_readRDPGinfo(DriverRDPG_o *p_DriverObj)
{
	size_t i;

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_DriverObj->methods->view_RDPGinfo(p_DriverObj);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = p_DriverObj->timeop_omp;
		m_alloc_time[i] = p_DriverObj->timeop_ltime;
	}
}



// void CPS210_CA(monitor *p_rdp)
// {
// 	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
// 	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */
// 	int id_thread;

// 	/* Variables para medicion de tiempos. */
// 	double t_ini = 0;
// 	double t_fin = 0;
// 	clock_t t_ini2 = 0;
// 	clock_t t_fin2 = 0;

// 	/* Se importan los componentes de RDPG. */
// 	monitor red("Red de Prueba", CA_mII, CA_mIH, CA_mIR, CA_mIRe, CA_vMI);

// 	/* Comienza cuenta de tiempo. */
// 	t_ini2 = clock();
// 	t_ini = omp_get_wtime();

// 	/* Generador de tareas. */
// 	for(size_t i=0; i< N_TASKS; i++)
// 	{
// 		red.generar_tarea_m();
// 	}

// 	red.print_vcomp(_vEx);


// 	#pragma omp parallel private(id_thread)
//     {
 
//     id_thread = omp_get_thread_num();
 
//     #pragma omp sections
//     {
//     	#pragma omp section
//       	{
//           	cout << "   Hilo "<< id_thread << " ejecutando ... \n";
//           	/* Encender CPU1*/
// 			red.encender_CPU1_m();

// 			//red.print_allComp();

// 			/*Procesar tareas CPU1*/
// 			while(red.get_TokensPlace((size_t)2) > 0)
// 			{
// 				red.procesar_tarea_CPU1_m();
// 				red.finalizar_tarea_CPU1_m();
// 			}

// 			/* Apagar CPU1 */
// 			red.apagar_CPU1_m();
// 			cout << "CPU1: Termino de procesar tareas. "<< endl;
          	
//       	}
 
// 	    #pragma omp section
// 		{
// 	        cout << "   Hilo "<< id_thread<< " ejecutando ... \n";
// 	        /* Encender CPU2*/
// 			red.encender_CPU2_m();

// 			//red.print_allComp();

// 			/*Procesar tareas CPU2*/
// 			while(red.get_TokensPlace((size_t)8) > 0)
// 			{
// 				red.procesar_tarea_CPU2_m();
// 				red.finalizar_tarea_CPU2_m();
// 			}

// 			/* Apagar CPU1 */
// 			red.apagar_CPU2_m();
// 			cout << "CPU1: Termino de procesar tareas. "<< endl;
// 	    }
 
//     }//Fin Sections
 
//     }//Fin Parallel


// 	cout << "Vector de marcado actual: "<<endl;
// 	red.print_vcomp(_vMA);
// 	cout << endl;


// 	/* Finaliza cuenta de tiempo. */
// 	t_fin2 = clock();
// 	t_fin = omp_get_wtime();

// 	timeop_omp = (t_fin-t_ini);
// 	timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

// 	/* Registro de muestras de tiempos. */
// 	//m_alloc_omp[i] = timeop_omp;
// 	//m_alloc_time[i] = timeop_ltime;


// 	cout << "   Las tareas procesadas son: " << red.get_tareasProcesadas() << endl;
// 	printf("   Tiempo de operacion promedio con openMP: %lf\n", timeop_omp);
// 	printf("   Tiempo de operacion promedio con lib time: %lf\n", timeop_ltime);
// }

