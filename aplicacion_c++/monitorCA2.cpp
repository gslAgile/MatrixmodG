#include "monitorCA2.hpp"


monitorCA2::monitorCA2(string p_name, size_t p_places, size_t p_transitions)
: red(p_name, p_places, p_transitions), vCV(p_transitions), mtimeop_omp(0.0), mtimeop_ltime(0.0) {}


monitorCA2::monitorCA2(string p_name, string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
: red(p_name, p_mII, p_mIH, p_mIR, p_mIRe, p_vMI), vCV(red.get_lineElements(p_mII)), mtimeop_omp(0.0), mtimeop_ltime(0.0) 
{ 
	red.set_vdim(MAX_VDIM);
	//vector_o& comp = red.ref_vcomp(_vG);
	//comp[7]=0; /* Se inicia guarda G(T1) y G(T7). */
} 



void monitorCA2::producir()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout << "Entra productor." << endl;

	ashoot_RDPG(0, SHOT_MODE_E, l);

	cout << "Produciendo..." << endl;

	ashoot_RDPG(1, SHOT_MODE_E, l);

	cout << "Productor pone en buffer." << endl;

	/* Abandono monitor.*/

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitorCA2::consumir()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	cout << "Entra consumidor." << endl;

	ashoot_RDPG(2, SHOT_MODE_E, l);

	cout << "Consumidor extrae de buffer." << endl;

	ashoot_RDPG(3, SHOT_MODE_E, l);

	cout << "Consumiendo..." << endl;
	
										/* Abandono monitor, */

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */




int monitorCA2::get_tareasProcesadas()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	if(red.get_TokensPlace(2) == -EC_datoIncorrecto)
	{
		return -EC_datoIncorrecto;
	}

	return red.get_TokensPlace(14);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


int monitorCA2::get_TokensPlace(size_t p_place)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	return red.get_TokensPlace(p_place);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


bool monitorCA2::empty()
{
	return red.empty();
}


void monitorCA2::print_vcomp(ID_VCOMPONENT p_comp)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.print_vcomp(p_comp);

} /* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitorCA2::print_allComp()
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



void monitorCA2::import_RDPG(string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.import_RDPG(p_mII, p_mIH, p_mIR, p_mIRe, p_vMI);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


int monitorCA2::shoot_RDPG(int p_transicion, SHOT_MODE p_mode)
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	return red.shoot_rdpg(p_transicion, p_mode);

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */


void monitorCA2::ashoot_RDPG(int p_transicion, SHOT_MODE p_mode, unique_lock<mutex>& lck)
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
int monitorCA2::next_shoot(int p_transicion)
{
	switch(p_transicion)
	{
		case 1: return 2;

		case 2: return 1;

		default: return -1;
	}
}


void monitorCA2::print_RDPGinfo()
{
	unique_lock<mutex> l{mtx_monitor};	/* Adquiere exclusion mutua del monitor */

	red.print_RDPGinfo();

}/* Libera la exclusion mutua al finalizar el bloque de codigo de la funcion. */
