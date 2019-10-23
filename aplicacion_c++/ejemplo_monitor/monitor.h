/* compile with g++, flags -std=c++0x -pthread */

#ifndef MONITOR_ULIB_H
#define MONITOR_ULIB_H /* ULIB: por biblioteca de espacio usuario. */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <iostream>
using namespace std;


class monitor {

private:

    mutex mtx_monitor;				/* Mutex que gestiona cola de procesos que ingresan al monitor. */
    condition_variable b_lleno;		/* CV para el control de los procesos que disparan transiciones de entrada a la RDPG. */
    condition_variable b_vacio;	/* CV para el control de los procesos que disparan transiciones de salida a la RDPG. */
    int buffer;
    //char element[32];
    //std::unique_lock::unique_lock l;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE METODOS DE Objeto monitor
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

public:

	/* Constructores */
    monitor(): buffer(0) {}

	/* Destructor */


	/* Metodos de objeto RDPG */
    void producir();
    void consumir();

};


#endif /* MONITOR_ULIB_H */