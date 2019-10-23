#ifndef MONITOR_ULIB_H
#define MONITOR_ULIB_H /* ULIB: por biblioteca de espacio usuario. */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include "RDPG_object.hpp"

#define		N_TASKS		100000



class monitor {

private:

    mutex mtx_monitor;					/* Mutex que gestiona cola de procesos que ingresan al monitor. */
    vector <condition_variable> vCV;	/* Vector de todas las variables de condicion. */    
    RDPG red;							/* Recurso compartido protegido por monitor al multiprocesamiento (se garantiza exclusion mutua). */
    double mtimeop_omp; 				/* Tiempo de operacion desde API omp. */
	double mtimeop_ltime; 				/* Tiempo de operacion desde libreria time. */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE METODOS DE Objeto monitor
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

public:
	/* Constructores */
	monitor(string, size_t, size_t);
	monitor(string, string, string, string, string, string);

	/* Destructor */


	/* Metodos de monitor con proteccion de recurso RDPG. */
	int get_tareasProcesadas();
	int get_TokensPlace(size_t);
	size_t get_transiciones() const { return red.get_transiciones(); }
	double get_mtimeop_omp() const { return mtimeop_omp; }
	double get_mtimeop_ltime() const { return mtimeop_ltime; }
	void set_mtimeop_omp(double p_time) { mtimeop_omp = p_time; }
	void set_mtimeop_ltime(double p_time) { mtimeop_ltime = p_time; }
	bool empty();
	void print_vcomp(ID_VCOMPONENT);
	void import_RDPG(string, string, string, string, string);
	int shoot_RDPG(int, SHOT_MODE);
	void ashoot_RDPG(int, SHOT_MODE, unique_lock<mutex>&);
	void print_allComp();
	void print_RDPGinfo();
	int next_shoot(int);

	/* Funciones CA 1 para multiples hilos. */
	void generar_tarea();
	void ON_OFF_CPU1();
	void ON_OFF_CPU2();
	void procesar_tarea_CPU1();
	void procesar_tarea_CPU2();
	void finalizar_tarea_CPU1();
	void finalizar_tarea_CPU2();

	/* Fnciones CA 1 para un solo hilo. */
	void generar_tarea_m();
	void encender_CPU1_m();
	void encender_CPU2_m();
	void procesar_tarea_CPU1_m();
	void procesar_tarea_CPU2_m();
	void finalizar_tarea_CPU1_m();
	void finalizar_tarea_CPU2_m();
	void apagar_CPU1_m();
	void apagar_CPU2_m();
};


#endif /* MONITOR_ULIB_H */