#ifndef SYSTEM_TESTS_H
#define SYSTEM_TESTS_H

#include "../../Librerias_usuario/libMatrixmodG.h"

#define N_MUESTRAS	30

/* Variables globales de pruebas de sistema automatizadas*/ 
/* Muestras */
double m_alloc_omp[N_MUESTRAS]; /* muestras de opoen MP */
double m_alloc_time[N_MUESTRAS]; /* muestras de time */

/*matrices dinamicas*/
DriverRDPG_o DTests; /* Driver de pruebas (tests).*/


/* Declaracion de funciones*/
void st_cargar_componentes(DriverRDPG_o *p_DriverObj);
void st_liberar_componentes(DriverRDPG_o *p_DriverObj);
void st_iniciar_vmuestra(void);
void st_promedio(void);
void st_muestras(void);

void CPS201_alloc_mem(DriverRDPG_o *p_DriverObj);
void CPS201_alloc_mem_nxm(DriverRDPG_o *p_DriverObj, const int p_nplazas, const int p_ntransiciones);
void CPS202_add_comps(DriverRDPG_o *p_DriverObj);
void CPS203_create_rdp(DriverRDPG_o *p_DriverObj);
void CPS204_delete_rdp(DriverRDPG_o *p_DriverObj);
void CPS204_delete_rdp_nxm(DriverRDPG_o *p_DriverObj, const int p_nplazas, const int p_ntransiciones);
void CPS205_shoot(DriverRDPG_o *p_DriverObj);
void CPS206_readAllComp(DriverRDPG_o *p_DriverObj);
void CPS207_readRDPGinfo(DriverRDPG_o *p_DriverObj);

#endif
