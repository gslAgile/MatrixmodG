#include "monitor.h"


void monitor::producir() {
    
    unique_lock<mutex> l{mtx_monitor};	/* Adquiere el cerrojo */

    sleep(2);
    
    while(buffer > 1){
       cout<< "Productor esperando que se vacie buffer..."<<endl;
       b_lleno.wait(l);					/* Se Libera el cerrojo antes de dormir proceso en CV. */

    }
    
    buffer++;
    cout << "Produccion de un elemento." << endl;
    b_vacio.notify_one();
}										/* Libera el cerrojo al finalizar el bloque de codigo de la funcion. */


void monitor::consumir() {
    unique_lock<mutex> l{mtx_monitor};	/* Adquiere el cerrojo */

    sleep(1);
	
	while(buffer ==0){
	    cout<< "Consumidor esperando que se llene buffer..."<<endl;
	    b_vacio.wait(l);				/* Se Libera el cerrojo antes de dormir proceso en CV. */
	}
    
    buffer--;
    cout << "Consumo de un elemento." << endl;
    b_lleno.notify_one();
}										/* Libera el cerrojo al finalizar el bloque de codigo de la funcion. */



int main()
{
	monitor buff;


	thread t1([&]() {
        buff.consumir();
    });

	sleep(1);

	thread t2([&]() {
        buff.producir();
    });
	

	t1. join () ;
	t2. join () ;

	return 0;
}