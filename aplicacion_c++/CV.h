#ifndef CV_ULIB_H
#define CV_ULIB_H /* ULIB: por biblioteca de espacio usuario. */

#include <mutex>

class CV {

private:
    mutex *mxt_monitor;                 /* Mutex para el control de la cola de procesos del monitor. */
    mutex miCV;                         /* Mutex para el control de la cola de porcesos de miCV. */
    size_t bloqueados;                  /* Numero de procesos bloqueados en miCV (variable de condicion). */

public:

    /* Constructores */
    CV(mutex *p_mtx): mxt_monitor(p_mtx), bloqueados(0) {}

    /* Destructor */


    /* Metodos de objeto CV */
    void delay() {
        bloqueados++;               /* Se suma un proceso a la cola de miCV. */
        (*mxt_monitor).unlock();    /* Libero mtx de monitor (permito el acceso de un proceso de la cola del monitor). */
        miCV.lock();         /* Pongo a dormir proceso en cola de miCV. */
    }

    void resume() {
        if(bloqueados>0){           /* Si ai procesos en la cola de miCV. Los despierto. */
            bloqueados--;           /* Se descuenta un proceso a la cola de miCV. */
            miCV.unlock();          /* Despierto uno de los procesos de la cola de miCV. */
        }
        else{                       /* Si no ai procesos en la cola de miCV. Libero mtx de monitor. */
            (*mxt_monitor).unlock();/* Libero mtx de monitor (permito el acceso de un proceso de la cola del monitor). */
        }
    }
    bool empty(){
        if(bloqueados > 0)  return false;
        else    return true;
    }
};


#endif /* CV_ULIB_H */