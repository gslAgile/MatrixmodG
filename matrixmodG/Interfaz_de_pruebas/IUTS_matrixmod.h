#ifndef IUTC_H
#define IUTC_H

#include "../Librerias_usuario/libMatrixmodG.h"
#include "Pruebas_Sistema/system_tests.h"

#define NUMSEM 			5
#define TAM 			(1024*10 -1)
#define N_COMANDOS		19

/* Enumeraciones */
/**
 * NUM_OPCIONES: Enumerador del numero de opciones disponibles en el menu de opciones de la interfaz IUTS_matrixmod.c.
 */
enum NUM_OPCIONES
{
	OPCION_0 = 0, 
	OPCION_1,
	OPCION_2, 
	OPCION_3, 
	OPCION_4, 
	OPCION_5, 
	OPCION_6, 
	OPCION_7, 
	OPCION_8, 
	OPCION_9, 
	OPCION_10, 
	OPCION_11, 
	OPCION_12, 
	OPCION_13, 
	OPCION_14, 
	OPCION_15,
	OPCION_16,
	OPCION_17,
	OPCION_18,
	OPCION_19,
	OPCION_20,
	OPCION_21,
	OPCION_22,
	OPCION_MAX
};


/* Variables globales*/
int db_exit=0;
const char *comandos_write[N_COMANDOS] = {"RDPGinfo name\n","RDPGinfo places\n","RDPGinfo transitions\n","RDPGinfo shots\n","RDPGinfo memory\n",
"RDPG cat mII\n","RDPG cat mIH\n","RDPG cat mIR\n","RDPG cat mD\n",
"RDPG cat vMI\n","RDPG cat vMA\n","RDPG cat vMN\n","RDPG cat vQ\n",
"RDPG cat vW\n","RDPG cat vE\n","RDPG cat vB\n","RDPG cat vL\n","RDPG cat vEx\n","RDPGinfo comp\n"};

const char program_sem_path[] = "./semaforos_IPC/S_gestion_de_semaforos.out ";
const char *sem_cmd[] = {"i \n", "u \n", "l \n", "b \n", "e \n" };

/* Funciones de debug*/
void debug_check_point(char * p_comando);

/* Funciones */
void disparar_trasnsicion_nHilos(DriverRDPG_o *p_DriverObj, char *cadena, unsigned int p_nHilos);
void disparo_dos_hilos(DriverRDPG_o *p_DriverObj,char * p_comando);
void disparo_tres_hilos(DriverRDPG_o *p_DriverObj, char * p_comando);
void disparo_cuatro_hilos(DriverRDPG_o *p_DriverObj, char * p_comando);

#endif