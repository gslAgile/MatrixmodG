// Gestor de un conjunto de semáforos
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <omp.h>
#include <time.h>
#define NUMSEM 10

const char sem_path[] = "/home/gabriel/Documentos/PI/Repo_Git/pi_driver_linux_rdpg/1_Software/matrixmod/Interfaz_de_pruebas/semaforos_IPC";

int main( int argc, char *argv[] ) {
	key_t clave;
	int semid, i, carga, recurso;
	//int seminit[NUMSEM] = { 3, 3, 2, 2, 1 };
	int seminit[NUMSEM] = { 3, 3, 2, 2, 1, 10, 1, 5, 5, 2 };
	//int seminit[NUMSEM];
	struct sembuf misem;

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;


	if( argc < 2 ) {
		printf( "Uso: %s operación [parámetros]\nOperaciones: i(nicializar) u(tilizar) l(iberar) b(orrar conjunto) e(stado del conjunto)\n",  argv[0] );
		exit( 1 );
	}

	//clave = ftok( "/home", 'A' );
	clave = ftok( sem_path, 'A' );
	if( clave == -1 ) {  
		perror( "ftok" );
		return 1;
	}
	else
		printf( "Identificador de semáforo: %0x\n", clave );
  
	// Generación o asociación a un conjunto de semáforos
	semid = semget( clave, NUMSEM, IPC_CREAT | 0666 );
	if( semid == -1 )
		perror( "semid" );

	switch( argv[1][0] ) {
		case 'i': // Carga de valor inicial de los semáforos
			/* Comienza cuenta de tiempo. */
			t_ini2 = clock();
			t_ini = omp_get_wtime();

			for( i=0; i<NUMSEM; i++ )
			if( semctl( semid, i, SETVAL, seminit[i] ) == -1 )
				perror( "Carga.semctl" ); 
			else
				printf( "Semáforo %d cargado OK\n", i );
			
			break;

		case 'u':  // Hace uso del recurso indicado o pregunta cuál
			if( argc < 3 ) {
				printf( "Usa recurso: " );
				scanf( "%d", &recurso );
			}
			else
				recurso = atoi( argv[2] );

			/* Comienza cuenta de tiempo. */
			t_ini2 = clock();
			t_ini = omp_get_wtime();

			// Carga de la estructura sembuf
			misem.sem_num = recurso;
			misem.sem_op = -1;
			misem.sem_flg = 0;  //espera el recurso

			if( semop( semid, &misem, 1 ) == -1 )
				perror( "semop" ); 
			else
				printf( "Recurso %d agregado una utilización\n", recurso );
			
			break;

		case 'e':  // Lista el estado de los semáforos
			/* Comienza cuenta de tiempo. */
			t_ini2 = clock();
			t_ini = omp_get_wtime();

			for( i=0; i<NUMSEM; i++ ) {
				carga = semctl( semid, i, GETVAL, 0 );
				if( carga == -1 )
					perror( "semctl" ); 
				else
					printf( "Semáforo %d cargado con el valor %d\n", i, carga );
			}
			break;

		case 'b':  // Elimina el conjunto de semáforos
			/* Comienza cuenta de tiempo. */
			t_ini2 = clock();
			t_ini = omp_get_wtime();

			if( semctl( semid, 0, IPC_RMID, 0 ) == -1 )
				perror( "Elimina.semctl" ); 
			else
				printf( "Conjunto de semáforos eliminado!\n" );
			
			break;

		case 'l': // Libera el recurso indicado o pregunta cuál
			if( argc < 3 ) {
				printf( "Libera recurso: " );
				scanf( "%d", &recurso );
			}
			else
				recurso = atoi( argv[2] );
  
			/* Comienza cuenta de tiempo. */
			t_ini2 = clock();
			t_ini = omp_get_wtime();

			// Carga de la estructura sembuf
			misem.sem_num = recurso;
			misem.sem_op = 1;
			misem.sem_flg = 0;  //espera el recurso

			if( semop( semid, &misem, 1 ) == -1 )
				perror( "semop" ); 
			else
				printf( "Recurso %d liberada una utilización\n", recurso );
			
			break;

		default: 
			printf( "La operación indicada NO existe\n" );
			exit(1);
			break;
	}
	
	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();
	
	/* Muestra de resultados de tiempos. */
	printf("  Tiempo de respuesta de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
	printf("  Tiempo de respuesta de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));

	return 0;
}
