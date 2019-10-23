#include "system_tests.hpp"

/* Archivos de importacion de RDPG: gn: global name*/
string gn_mII = "./RDPG/identity_1000x1000/mII.txt";
string gn_mIH = "./RDPG/identity_1000x1000/mIH.txt";
string gn_mIR = "./RDPG/identity_1000x1000/mIR.txt";
string gn_mIRe = "./RDPG/identity_1000x1000/mIRe.txt";// = "mIRe.txt";
string gn_vMI = "./RDPG/identity_1000x1000/vMI.txt";


string CA_mII = "./Caso_Aplicacion/mII.txt";
string CA_mIH = "./Caso_Aplicacion/mIH.txt";
string CA_mIR = "./Caso_Aplicacion/mIR.txt";
string CA_mIRe = "./Caso_Aplicacion/mIRe.txt";// = "mIRe.txt";
string CA_vMI = "./Caso_Aplicacion/vMI.txt";

string CA2_mII = "./Caso_Aplicacion2/mII.txt";
string CA2_mIH;
string CA2_mIR;
string CA2_mIRe;
string CA2_vMI = "./Caso_Aplicacion2/vMI.txt";

const size_t N_P = 1000; /* Numero de plazas para pruebas que lo utilizan. */
const size_t M_T = 1000; /* Numero de transiciones para pruebas que lo utilizan. */


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
 * Esta prueba mide cuanto tiempo demanda la asignación de memoria en el driver MatrixmodG para una RDPG np x nt (plazas transiciones indicadas por 
 * parámetro). Se prueba la creación de redes con plazas y transiciones iniciando por las decenas, centenas y llegando hasta el límite de 1000 plazas
 * y transiciones. 
 */
void CPS201_alloc_mem_nxm(monitor *p_rdp, const size_t p_nplazas, const size_t p_ntransiciones)
{
	size_t i;
	double timeop_omp=0, timeop_ltime=0;
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;


	/* Simulo una RDPG de nxm. */
	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		p_rdp = new monitor("Red de Prueba", p_nplazas, p_ntransiciones);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


/* 
 * Esta prueba mide cuanto tiempo demanda la asignación de valores para los componentes base de una RDPG cargada en el kernel. Se prueba cuanto demora 
 * el envió de datos al kernel para la asignación de todos los componentes bases de una RDPG.
 */
void CPS202_add_comps_10x10(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Archivos de importacion de RDPG: */
	string n_mII = "./RDPG/identity_10x10/mII.txt";
	string n_mIH = "./RDPG/identity_10x10/mIH.txt";
	string n_mIR = "./RDPG/identity_10x10/mIR.txt";
	string n_mIRe = "./RDPG/identity_10x10/mIRe.txt";// = "mIRe.txt";
	string n_vMI = "./RDPG/identity_10x10/vMI.txt";


	for(i=0; i<N_MUESTRAS; i++)
	{
		p_rdp = new monitor("Red de Prueba", (size_t)10, (size_t)10);

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		/* Al medir solo el tiempo de asignacion de valores se resetea a cero nuevamente el tiempo. */
		p_rdp->import_RDPG(n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


void CPS202_add_comps_100x100(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Archivos de importacion de RDPG: */
	string n_mII = "./RDPG/identity_100x100/mII.txt";
	string n_mIH = "./RDPG/identity_100x100/mIH.txt";
	string n_mIR = "./RDPG/identity_100x100/mIR.txt";
	string n_mIRe = "./RDPG/identity_100x100/mIRe.txt";// = "mIRe.txt";
	string n_vMI = "./RDPG/identity_100x100/vMI.txt";


	for(i=0; i<N_MUESTRAS; i++)
	{
		p_rdp = new monitor("Red de Prueba", (size_t)100, (size_t)100);

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		/* Al medir solo el tiempo de asignacion de valores se resetea a cero nuevamente el tiempo. */
		p_rdp->import_RDPG(n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


void CPS202_add_comps_500x500(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Archivos de importacion de RDPG: */
	string n_mII = "./RDPG/identity_500x500/mII.txt";
	string n_mIH = "./RDPG/identity_500x500/mIH.txt";
	string n_mIR = "./RDPG/identity_500x500/mIR.txt";
	string n_mIRe = "./RDPG/identity_500x500/mIRe.txt";// = "mIRe.txt";
	string n_vMI = "./RDPG/identity_500x500/vMI.txt";


	for(i=0; i<N_MUESTRAS; i++)
	{
		p_rdp = new monitor("Red de Prueba", (size_t)500, (size_t)500);

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		/* Al medir solo el tiempo de asignacion de valores se resetea a cero nuevamente el tiempo. */
		p_rdp->import_RDPG(n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


void CPS202_add_comps_1000x1000(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Archivos de importacion de RDPG: */
	string n_mII = "./RDPG/identity_1000x1000/mII.txt";
	string n_mIH = "./RDPG/identity_1000x1000/mIH.txt";
	string n_mIR = "./RDPG/identity_1000x1000/mIR.txt";
	string n_mIRe = "./RDPG/identity_1000x1000/mIRe.txt";// = "mIRe.txt";
	string n_vMI = "./RDPG/identity_1000x1000/vMI.txt";


	for(i=0; i<N_MUESTRAS; i++)
	{
		p_rdp = new monitor("Red de Prueba", (size_t)1000, (size_t)1000);

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		/* Al medir solo el tiempo de asignacion de valores se resetea a cero nuevamente el tiempo. */
		p_rdp->import_RDPG(n_mII, n_mIH, n_mIR, n_mIRe, n_vMI);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


void CPS203_create_RDPG(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	cout << gn_mII << endl;

	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		p_rdp = new monitor("Red de Prueba", gn_mII, gn_mIH, gn_mIR, gn_mIRe, gn_vMI);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i]= timeop_ltime;

		/* Elimino RDPG. */
		delete p_rdp;
	}
}


/* 
 * Esta prueba mide cuanto tiempo demanda al driver MatrixmodG la operación delete de una RDPG nxm creada previamente. 
 * Tanto n (plazas) como m (transiciones) se parametrizan iniciando desde las decenas hasta llegar a las 1000 plazas y transiciones que es el 
 * límite máximo que soporta el driver MatrixmodG.
 */
void CPS204_delete_rdp_nxm(monitor *p_rdp, const size_t p_nplazas, const size_t p_ntransiciones)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	for(i=0; i<N_MUESTRAS; i++)
	{
		p_rdp = new monitor("Red de Prueba", p_nplazas, p_ntransiciones);

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();
		
		/* Elimino RDPG. */
		delete p_rdp;

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i]= timeop_ltime;
	}
}



void CPS205_shoot(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Se importan los componentes de RDPG. */
	string vacio;
	p_rdp = new monitor("Red de Prueba", gn_mII, vacio, vacio, vacio, gn_vMI);

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	
	for(i=0; i<N_MUESTRAS; i++)
	{
		int n_rand = rand()%(p_rdp->get_transiciones());

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		p_rdp->shoot_RDPG(n_rand, SHOT_MODE_E);

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;
	}

	/* Elimino RDPG. */
	delete p_rdp;
}


void CPS206_readAllComp(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Se importan los componentes de RDPG. */
	p_rdp = new monitor("Red de Prueba", gn_mII, gn_mIH, gn_mIR, gn_mIRe, gn_vMI);

	//st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Comienza cuenta de tiempo. */
		t_ini = omp_get_wtime();
		t_ini2 = clock();

		p_rdp->print_allComp();

		/* Finaliza cuenta de tiempo. */
		t_fin = omp_get_wtime();
		t_fin2 = clock();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;
	}

	/* Elimino RDPG. */
	delete p_rdp;
}


void CPS207_readRDPGinfo(monitor *p_rdp)
{
	size_t i;
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	
	/* Se importan los componentes de RDPG. */
	p_rdp = new monitor("Red de Prueba", gn_mII, gn_mIH, gn_mIR, gn_mIRe, gn_vMI);

	
	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */

	
	for(i=0; i<N_MUESTRAS; i++)
	{
		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		p_rdp->print_RDPGinfo();

		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[i] = timeop_omp;
		m_alloc_time[i] = timeop_ltime;
	}

	/* Elimino RDPG. */
	delete p_rdp;
}



void CPS208_CA_threads3()
{
	double timeop_omp=0; 	/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 	/* tiempo de operacion desde libreria time. */
	int id_thread;

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Se importan los componentes de RDPG. */
	monitor red("Red de Prueba", CA_mII, CA_mIH, CA_mIR, CA_mIRe, CA_vMI);

	st_iniciar_vmuestra();		/* Se limpian registros de tiempos promedios. */


	for(size_t j=0; j<N_MUESTRAS; j++){	

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();

		/* Generador de tareas. */
		for(size_t i=0; i< N_TASKS; i++)
		{
			red.generar_tarea_m();
		}

		red.print_vcomp(_vEx);


		#pragma omp parallel private(id_thread)
	    {
	 
	    id_thread = omp_get_thread_num();
	 
	    #pragma omp sections
	    {
	    	#pragma omp section
	      	{
	          	cout << "   Hilo "<< id_thread << " ejecutando ... \n";
	          	/* Encender CPU1*/
				red.encender_CPU1_m();

				//red.print_allComp();

				/*Procesar tareas CPU1*/
				while(red.get_TokensPlace((size_t)2) > 0)
				{
					red.procesar_tarea_CPU1_m();
					red.finalizar_tarea_CPU1_m();
				}

				/* Apagar CPU1 */
				red.apagar_CPU1_m();
				cout << "CPU1: Termino de procesar tareas. "<< endl;
	          	
	      	}
	 
		    #pragma omp section
			{
		        cout << "   Hilo "<< id_thread<< " ejecutando ... \n";
		        /* Encender CPU2*/
				red.encender_CPU2_m();

				//red.print_allComp();

				/*Procesar tareas CPU2*/
				while(red.get_TokensPlace((size_t)8) > 0)
				{
					red.procesar_tarea_CPU2_m();
					red.finalizar_tarea_CPU2_m();
				}

				/* Apagar CPU1 */
				red.apagar_CPU2_m();
				cout << "CPU1: Termino de procesar tareas. "<< endl;
		    }
	 
	    }//Fin Sections
	 
	    }//Fin Parallel


		cout << "Vector de marcado actual: "<<endl;
		red.print_vcomp(_vMA);
		cout << endl;


		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de muestras de tiempos. */
		m_alloc_omp[j] = timeop_omp;
		m_alloc_time[j] = timeop_ltime;
	}


	cout << "   Las tareas procesadas son: " << red.get_tareasProcesadas() << endl;
	printf("   Tiempo de operacion promedio con openMP: %lf\n", timeop_omp);
	printf("   Tiempo de operacion promedio con lib time: %lf\n", timeop_ltime);
}



void CPS208_CA_threads5()
{	
	/* Se importan los componentes de RDPG. */
	monitor *red = new monitor("Red de Prueba", CA_mII, CA_mIH, CA_mIR, CA_mIRe, CA_vMI);

	thread t2([&]() {
        while(true)
        {
        	red->ON_OFF_CPU1();
        }
    });


    thread t4([&]() {
        while(true)
        {
        	red->ON_OFF_CPU2();
        }
    });


    thread t3([&]() {
        while(true)
        {
        	red->procesar_tarea_CPU1();
        	usleep(100);
        	red->finalizar_tarea_CPU1();
        }
    });


    thread t5([&]() {
        while(true)
        {
        	red->procesar_tarea_CPU2();
        	usleep(5);
        	red->finalizar_tarea_CPU2();
        }
    });

    usleep(100);

    thread t1([&]() {
        for(size_t i=0; i<N_TASKS; i++)
        {
        	red->generar_tarea();
        }
        red->print_vcomp(_vMA);
    });


	int opcion = 0, op_thread = 0;
    while( opcion < 6)
	{
	    cout << "\n--------------------------------------------------------------------------------\n";
    	cout <<  "\t\tAPP C++ - MENU DE OPERACIONES RDPG \n";
    	cout << "--------------------------------------------------------------------------------\n";
        cout <<  "\n  1. Ver estado de RDPG (marcado actual).";
        cout <<  "\n  2. Ver estado completo de RDPG (todos los componentes).";
        cout <<  "\n  3. Ver vector Ex de transicicones sensibilizadas extendido en la RDPG.";
        cout <<  "\n  4. Ver tiempo total del procesamiento de tareas.";
        cout <<  "\n  5. Repetir prueba.";
        cout <<  "\n  6. Salir y finalizar hilos.";
        cout <<  "\n\n   Ingrese numero de opcion: \n";
        cin >> opcion;


        switch(opcion)
        {
        	case 1: 
        			red->print_vcomp(_vMA);
        			break;

        	case 2:	
        			red->print_allComp();
        			break;

        	case 3:	
        			red->print_vcomp(_vEx);
        			break;

        	case 4:	
        			cout << "   --> TIEMPO TOTAL DE PROCESAMIENTO DE TAREAS: " << red->get_mtimeop_omp() << " (API OpenMP)" << endl;
        			cout << "   --> TIEMPO TOTAL DE PROCESAMIENTO DE TAREAS: " << red->get_mtimeop_ltime() << " (API time.h)" << endl;
        			break;

        	case 5: 
        			/* Reinicio variables de medicion de tiempo: */
        			red->set_mtimeop_omp(0.0);
        			red->set_mtimeop_ltime(0.0);

        			/* Repito prueba: */
        			for(size_t i=0; i<N_TASKS; i++)
			        {
			        	red->generar_tarea();
			        }
        			break;
        	
        	case 6:
        			op_thread = 1;
        			break;

        	default:
        			cout<< "Opcion incorrecta!" << endl;
        }
    }

    if(op_thread == 2)
	{
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();	
	}

	delete red;
}


void CPS209_CA2_threads2()
{	
	/* Se importan los componentes de RDPG. */
	monitorCA2 *red = new monitorCA2("Red de Prueba", CA2_mII, CA2_mIH, CA2_mIR, CA2_mIRe, CA2_vMI);

	thread t2([&]() {
        while(true)
        {
        	red->consumir();
        }
    });

    /* Espera a que se activen el resto de hilos de la RDPG (t2,t3,t4 y t5). */
    usleep(100);

    thread t1([&]() {
        for(size_t i=0; i<N_OPERACIONES; i++)
        {
        	red->producir();
        }
        red->print_vcomp(_vMA);
    });


	int opcion = 0, op_thread = 0;
    while( opcion < 6)
	{
	    cout << "\n--------------------------------------------------------------------------------\n";
    	cout <<  "\t\tAPP C++ - MENU DE OPERACIONES RDPG \n";
    	cout << "--------------------------------------------------------------------------------\n";
        cout <<  "\n  1. Ver estado de RDP (marcado actual).";
        cout <<  "\n  2. Ver estado completo de RDP (todos los componentes).";
        cout <<  "\n  3. Ver vector Ex de transicicones sensibilizadas extendido en la RDPG.";
        cout <<  "\n  4. Ver tiempo total del procesamiento de tareas.";
        cout <<  "\n  5. Repetir prueba.";
        cout <<  "\n  6. Salir y finalizar hilos.";
        cout <<  "\n\n   Ingrese numero de opcion: \n";
        cin >> opcion;


        switch(opcion)
        {
        	case 1: 
        			red->print_vcomp(_vMA);
        			break;

        	case 2:	
        			red->print_allComp();
        			break;

        	case 3:	
        			red->print_vcomp(_vEx);
        			break;

        	case 4:	
        			cout << "   --> TIEMPO TOTAL DE PROCESAMIENTO DE TAREAS: " << red->get_mtimeop_omp() << " (API OpenMP)" << endl;
        			cout << "   --> TIEMPO TOTAL DE PROCESAMIENTO DE TAREAS: " << red->get_mtimeop_ltime() << " (API time.h)" << endl;
        			break;

        	case 5:
        			/* Reinicio variables de medicion de tiempo: */
        			red->set_mtimeop_omp(0.0);
        			red->set_mtimeop_ltime(0.0);

        			/* Repito prueba: */
        			for(size_t i=0; i<N_OPERACIONES; i++)
			        {
			        	red->producir();
			        }
        			break;

        	case 6:
        			op_thread = 1;
        			break;

        	default:
        			cout<< "Opcion incorrecta!" << endl;
        }
    }

    if(op_thread == 2)
	{
		t1.join();
		t2.join();
	}

	delete red;
}




int main()
{

	int opcion_MTEST = 0;

    /* --- MENU DE OPCIONES ---	*/
    while ( (opcion_MTEST != 14) )
    {
		cout <<"\n--------------------------------------------------------------------------------\n";
		cout << "\t\t  PRUEBAS DE SISTEMA AUTOMATIZADAS \n";
		cout <<"--------------------------------------------------------------------------------\n";
        cout << "\n   1. Ejecutar CPS201_alloc_mem_nxm.";
        cout << "\n   2. Ejecutar CPS202_add_comps_10x10.";
        cout << "\n   3. Ejecutar CPS202_add_comps_100x100.";
        cout << "\n   4. Ejecutar CPS202_add_comps_500x500 .";
        cout << "\n   5. Ejecutar CPS202_add_comps_1000x1000.";
        cout << "\n   6. Ejecutar CPS204_delete_rdp_nxm.";
        cout << "\n   7. Ejecutar CPS203_create_RDPG.";
        cout << "\n   8. Ejecutar CPS205_shoot.";
        cout << "\n   9. Ejecutar CPS206_readAllComp.";
        cout << "\n   10. Ejecutar CPS207_readRDPGinfo.";
        cout << "\n   11. Ejecutar CPS208_CA_threads3.";
        cout << "\n   12. Ejecutar CPS208_CA_threads5.";
        cout << "\n   13. Ejecutar CPS209_CA2_threads2.";
		cout << "\n   14. Salir." ;
        cout << "\n\n   Ingrese numero de opcion: ";

    	cin >> opcion_MTEST;

    	switch ( opcion_MTEST )
        {
            case 1: 
            		cout << "\n   Ejecutando CPS201_alloc_mem_nxm. Para RDPG con " << N_P << " plazas y " << M_T << " transiciones.   \n";
            		CPS201_alloc_mem_nxm(TestRed, N_P, M_T);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 2: cout << "\n   Ejecutando CPS202_add_comps_10x10...   \n" ;
            		CPS202_add_comps_10x10(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 3: cout << "\n   Ejecutando CPS202_add_comps_100x100...   \n" ;
            		CPS202_add_comps_100x100(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 4: cout << "\n   Ejecutando CPS202_add_comps_500x500... \n";
            		CPS202_add_comps_500x500(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 5: cout << "\n   Ejecutando CPS202_add_comps_1000x1000... \n";
            		CPS202_add_comps_1000x1000(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 6: cout << "\n   Ejecutando CPS204_delete_rdp_nxm. Para RDPG con " << N_P << " plazas y " << M_T << " transiciones.   \n";
            		CPS204_delete_rdp_nxm(TestRed, N_P, M_T);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 7: cout << "\n   Ejecutando CPS203_create_RDPG. " << endl;
            		CPS203_create_RDPG(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 8: cout << "\n   Ejecutando CPS205_shoot. " << endl;
            		CPS205_shoot(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;
            
            case 9: cout << "\n   Ejecutando CPS206_readAllComp. " << endl;
            		CPS206_readAllComp(TestRed);
            		st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

            case 10: cout << "\n   Ejecutando CPS207_readRDPGinfo. " << endl;
            		CPS207_readRDPGinfo(TestRed);
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;
            
            case 11: cout << "\n   Ejecutando CPS208_CA_threads3. " << endl;
            		CPS208_CA_threads3();
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		st_promedio();
                    break;

			case 12: cout << "\n   Ejecutando CPS208_CA_threads5. " << endl;
            		CPS208_CA_threads5();
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		//st_promedio();
                    break;

            case 13: cout << "\n   Ejecutando CPS209_CA2_threads2. " << endl;
            		CPS209_CA2_threads2();
            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
            		//st_promedio();
                    break;

            case 14: cout << "\n   Saliendo de aplicacion.\n\n";
					opcion_MTEST = 14; /* Sale del bucle while y sale de la aplicacion. */
                    break;

            default: cout << "\n   Comando no valido. Intente nuevamente segun opciones de menu.\n ";

        }
    }

	return 0;
}