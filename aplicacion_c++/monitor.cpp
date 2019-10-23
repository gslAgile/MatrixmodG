#include "monitor.hpp"


monitor::monitor(string p_name, size_t p_places, size_t p_transitions)
: red(p_name, p_places, p_transitions), vCV(p_transitions), mtimeop_omp(0.0), mtimeop_ltime(0.0) {}


monitor::monitor(string p_name, string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
: red(p_name, p_mII, p_mIH, p_mIR, p_mIRe, p_vMI), vCV(red.get_lineElements(p_mII)), mtimeop_omp(0.0), mtimeop_ltime(0.0) 
{ 
	red.set_vdim(MAX_VDIM);
	//vector_o& comp = red.ref_vcomp(_vG);
	//comp[7]=0; /* Se inicia guarda G(T1) y G(T7). */
} 




void monitor::generar_tarea()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

    /* Analisis de Guardas. */
    if((red.get_TokensPlace(2) == -EC_datoIncorrecto) && (red.get_TokensPlace(8) == -EC_datoIncorrecto))
    {
    	/* Si no existen marcados de plazas indicadas, gestionar error.*/

    }
    else if(red.get_TokensPlace(2) <= red.get_TokensPlace(8))
    {
    	/* Manejo de politica de RDPG mediante configuracion de guardas. */
    	red.add_value_vG(1,1); 		/* equivale a vG[1]=1; y update_vEx(); */
    	red.add_value_vG(7,0);		/* equivale a vG[7]=0; y update_vEx(); */

    	ashoot_RDPG(0, SHOT_MODE_E, l);

    	/* Envio tarea a buffer de CPU1. */
    	ashoot_RDPG(1, SHOT_MODE_E, l);

    	cout <<"Hilo 0: Se genero tarea para CPU1." << endl;

    	/* Abandono monitor.*/
    }
    else /* red.get_TokensPlace(2) > red.get_TokensPlace(8) */
    {
    	/* Manejo de politica de RDPG mediante configuracion de guardas. */
    	red.add_value_vG(7,1);		/* equivale a vG[7]=1; y update_vEx(); */
    	red.add_value_vG(1,0); 		/* equivale a vG[1]=0; y update_vEx(); */

    	ashoot_RDPG(0, SHOT_MODE_E, l);

    	/* Envio tarea a buffer de CPU2. */
    	ashoot_RDPG(7, SHOT_MODE_E, l);

    	cout <<"Hilo 0: Se genero tarea para CPU2." << endl;

    	/* Abandono monitor.*/
    }

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */



void monitor::ON_OFF_CPU1()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 1: CPU1 en Stand by." << endl;

	ashoot_RDPG(4, SHOT_MODE_E, l);		/* Se solicita a la RDPG una peticion de encendido de la CPU1 (T4). */

	ashoot_RDPG(5, SHOT_MODE_E, l);		/* Se solicita a la RDPG encender CPU1 (T5). */

	cout <<"Hilo 1: CPU1 ON." << endl;

	ashoot_RDPG(6, SHOT_MODE_E, l);		/* Apago CPU1, inicia su modo de ahorro de energetico (T6).*/
	
	cout <<"Hilo 1: CPU1 OFF." << endl;
	
										/* Abandono monitor, */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::ON_OFF_CPU2()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 3: CPU2 en Stand by." << endl;

	ashoot_RDPG(10, SHOT_MODE_E, l);	/* Se solicita a la RDPG una peticion de encendido de la CPU2 (T10). */

	ashoot_RDPG(11, SHOT_MODE_E, l);	/* Se solicita a la RDPG encender CPU2 (T11). */

	cout <<"Hilo 3: CPU2 ON." << endl;

	ashoot_RDPG(12, SHOT_MODE_E, l);	/* Apago CPU2, inicia su modo de ahorro de energetico (T12).*/
	
	cout <<"Hilo 3: CPU2 OFF." << endl;
	
										/* Abandono monitor, */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::procesar_tarea_CPU1()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 2: CPU1 idle." << endl;

	ashoot_RDPG(2, SHOT_MODE_E, l);		/* Se solicita a RDPG iniciar el procesamiento de la tarea por la CPU1 (T2). */

	cout <<"Hilo 2: CPU1 procesando tarea..." << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::procesar_tarea_CPU2()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */
	
	cout <<"Hilo 4: CPU2 idle." << endl;

	ashoot_RDPG(8, SHOT_MODE_E, l);		/* Se solicita a RDPG iniciar el procesamiento de la tarea por la CPU2 (T8). */

	cout <<"Hilo 4: CPU2 procesando tarea..." << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::finalizar_tarea_CPU1()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */
	
		
	ashoot_RDPG(3, SHOT_MODE_E, l);		/* Finalizo ejecucion de tarea en CPU1 (T3).*/
	
	cout <<"Hilo 2: CPU1 termino tarea!" << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::finalizar_tarea_CPU2()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */

	ashoot_RDPG(9, SHOT_MODE_E, l);		/* Finalizo ejecucion de tarea en CPU2 (T9).*/

	cout <<"Hilo 4: CPU2 termino tarea!" << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


int monitor::get_tareasProcesadas()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	if(red.get_TokensPlace(2) == -EC_datoIncorrecto)
	{
		return -EC_datoIncorrecto;
	}

	return red.get_TokensPlace(14);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


int monitor::get_TokensPlace(size_t p_place)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	return red.get_TokensPlace(p_place);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


bool monitor::empty()
{
	return red.empty();
}


void monitor::print_vcomp(ID_VCOMPONENT p_comp)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.print_vcomp(p_comp);

} /* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::print_allComp()
{
	size_t comp;

	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	/* Componentes matrices: */
	for( comp=ID_MC_INIT; comp< ID_MC_END; comp++)
	{
		red.print_mcomp((ID_MCOMPONENT)comp);
	}

	/* Componentes vectores: */
	for( comp=ID_VC_INIT; comp< ID_VC_END; comp++)
	{
		red.print_vcomp((ID_VCOMPONENT)comp);
	}
}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */



void monitor::import_RDPG(string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.import_RDPG(p_mII, p_mIH, p_mIR, p_mIRe, p_vMI);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


int monitor::shoot_RDPG(int p_transicion, SHOT_MODE p_mode)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	return red.shoot_rdpg(p_transicion, p_mode);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::ashoot_RDPG(int p_transicion, SHOT_MODE p_mode, unique_lock<mutex>& lck)
{
	//unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	double timeop_omp=0; 				/* tiempo de operacion desde API omp. */
	double timeop_ltime=0; 				/* tiempo de operacion desde libreria time. */

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini2 = clock();
	t_ini = omp_get_wtime();


	while(red.shoot_rdpg(p_transicion, p_mode) == SHOT_FAILED) /* Si disparo de transicion fallo. */
	{
		/* Finaliza cuenta de tiempo. */
		t_fin2 = clock();
		t_fin = omp_get_wtime();

		timeop_omp = (t_fin-t_ini);
		timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

		/* Registro de tiempo Total acumulado: */
		mtimeop_omp += timeop_omp;
		mtimeop_ltime += timeop_ltime;

		red.ref_vcomp(_vHQCV)[p_transicion]++;	/* Entra un hilo a cola de CV de transicion a disparar. */
		vCV[p_transicion].wait(lck); 			/* Duermo hilo. */
		red.ref_vcomp(_vHQCV)[p_transicion]--;	/* Sale un hilo de cola de CV de transicion a disparar. */	

		/* Comienza cuenta de tiempo. */
		t_ini2 = clock();
		t_ini = omp_get_wtime();
	}

	int proximo_disparo = next_shoot(p_transicion);
	
	if(proximo_disparo>=0)	/* Si ai proximo disparo, despierto hilo de mayor prioridad a disparar. */
	{
		if(red.ref_vcomp(_vHD)[proximo_disparo] > 0)
			vCV[proximo_disparo].notify_one();
	}
												/* Si no ai proximo disparo, abandono monitor. */

	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	timeop_omp = (t_fin-t_ini);
	timeop_ltime = ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC);

	/* Registro de tiempo Total acumulado: */
	mtimeop_omp += timeop_omp;
	mtimeop_ltime += timeop_ltime;
}


/* Politica de proximo disparo a realizar depende de cada RDPG en particular. */
int monitor::next_shoot(int p_transicion)
{
	switch(p_transicion)
	{
		case 1: return 4;

		case 3: return 6;

		case 5: return 2;

		case 7: return 10;

		case 9: return 12;

		case 11: return 8;

		default: return -1;
	}
}


void monitor::print_RDPGinfo()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.print_RDPGinfo();

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */



/* Funciones ejecutados por un mismo hilo (m: monocore). */
void monitor::generar_tarea_m()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */
	//mtx_monitor.lock();
	static size_t tasks=0; 				/* Tareas generadas. */
	tasks++;

	//red.print_vcomp(_vEx);

    /* Analisis de Guardas. */
    if((red.get_TokensPlace(2) == -EC_datoIncorrecto) && (red.get_TokensPlace(8) == -EC_datoIncorrecto))
    {
    	/* Si no existen marcados de plazas indicadas, gestionar error.*/

    }
    else if(red.get_TokensPlace(2) <= red.get_TokensPlace(8))
    {
    	/* Manejo de politica de RDPG mediante configuracion de guardas. */
    	red.add_value_vG(1,1); 		/* equivale a vG[1]=1; y update_vEx(); */
    	red.add_value_vG(7,0);		/* equivale a vG[7]=0; y update_vEx(); */

    	red.shoot_rdpg(0, SHOT_MODE_E);

    	/* Envio tarea a buffer de CPU1. */
    	red.shoot_rdpg(1, SHOT_MODE_E);

    	cout <<"Hilo 0: Se genero tarea para CPU1." << endl;

    	/* Abandono monitor.*/
    }
    else /* red.get_TokensPlace(2) > red.get_TokensPlace(8) */
    {
    	/* Manejo de politica de RDPG mediante configuracion de guardas. */
    	red.add_value_vG(7,1);		/* equivale a vG[7]=1; y update_vEx(); */
    	red.add_value_vG(1,0); 		/* equivale a vG[1]=0; y update_vEx(); */

    	red.shoot_rdpg(0, SHOT_MODE_E);

    	/* Envio tarea a buffer de CPU2. */
    	red.shoot_rdpg(7, SHOT_MODE_E);

    	cout <<"Hilo 0: Se genero tarea para CPU2." << endl;

    	/* Abandono monitor.*/
    }

    /*if(tasks== N_TASKS){
    	cout << "Vector de marcado actual: "<<endl;
    	red.print_vcomp(_vMA);
    	cout << endl;
    }*/
    //mtx_monitor.unlock();

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::encender_CPU1_m()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 1: CPU1 en Stand by." << endl;

	red.shoot_rdpg(4, SHOT_MODE_E);		/* Se solicita a la RDPG una peticion de encendido de la CPU1 (T4). */

	red.shoot_rdpg(5, SHOT_MODE_E);		/* Se solicita a la RDPG encender CPU1 (T5). */

	cout <<"Hilo 1: CPU1 ON." << endl;
	
										/* Abandono monitor, */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::encender_CPU2_m()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 3: CPU2 en Stand by." << endl;

	red.shoot_rdpg(10, SHOT_MODE_E);	/* Se solicita a la RDPG una peticion de encendido de la CPU2 (T10). */

	red.shoot_rdpg(11, SHOT_MODE_E);	/* Se solicita a la RDPG encender CPU2 (T11). */

	cout <<"Hilo 3: CPU2 ON." << endl;
	
										/* Abandono monitor, */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::procesar_tarea_CPU1_m()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 2: CPU1 idle." << endl;

	red.shoot_rdpg(2, SHOT_MODE_E);		/* Se solicita a RDPG iniciar el procesamiento de la tarea por la CPU1 (T2). */

	cout <<"Hilo 2: CPU1 procesando tarea..." << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::procesar_tarea_CPU2_m()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout <<"Hilo 4: CPU2 idle." << endl;

	cout <<"Hilo 4: CPU2 procesando tarea..." << endl;

	red.shoot_rdpg(8, SHOT_MODE_E);		/* Se solicita a RDPG iniciar el procesamiento de la tarea por la CPU2 (T8). */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::finalizar_tarea_CPU1_m()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */
	
	red.shoot_rdpg(3, SHOT_MODE_E);		/* Finalizo ejecucion de tarea en CPU1 (T3).*/
		
	cout <<"Hilo 2: CPU1 termino tarea!" << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::finalizar_tarea_CPU2_m()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */

	red.shoot_rdpg(9, SHOT_MODE_E);		/* Finalizo ejecucion de tarea en CPU2 (T9).*/

	cout <<"Hilo 4: CPU2 termino tarea!" << endl;

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::apagar_CPU1_m()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */

	cout << "Hilo 1: Esperando apagar CPU1.";

	cout <<"Hilo 1: CPU1 OFF." << endl;

	red.shoot_rdpg(6, SHOT_MODE_E);			/* Apago CPU1, inicia su modo de ahorro de energetico (T6).*/

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitor::apagar_CPU2_m()
{
	unique_lock<mutex> l{mtx_monitor};		/* Adquiere exclusion mutua del monitor */

	cout << "Hilo 3: Esperando apagar CPU1." << endl;

	//red.print_vcomp(_vE);

	cout <<"Hilo 3: CPU2 OFF." << endl;

	red.shoot_rdpg(12, SHOT_MODE_E);/* Apago CPU2, inicia su modo de ahorro de energetico (T12).*/

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */
