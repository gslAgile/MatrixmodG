//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/ipc.h>
//#include <sys/sem.h>

/* Librerias propias*/
#include "IUTS_matrixmod.h"

#define USR_BUF_SIZE	16384 			/* 2 ^ 14 */

int main()
{
	int i;

	// Variables para file descriptor (de driver del kernel)
	char cadena[USR_BUF_SIZE];
	int n, sel_menu, opcion_Msem, opcion_Mrdp, opcion_MTEST, entrada;
	unsigned int nHilos;

	/* Creacion de objeto de driver MatrixmodG */
	DriverRDPG_o driver;

	/* Variables semaforos */
	char sem_cmd_sel[256]; /* Seleccion de comando de semaforo. */

	printf("\n--------------------------------------------------------------------------------\n");
	printf("\t\tIUTS: Interfaz de usuario de debug \n");
	printf("--------------------------------------------------------------------------------\n");
	printf(" 1- Importar RDPG desde archivos de extencion [txt]. \n\n");
	printf(" 2- Iniciar conexion con driver directa (recomendado si ya existe cargada una RDPG en el driver MatrixmodG). \n\n");
	printf(" 3- Iniciar IUTS para PRUEBAS DE SISTEMA AUTOMATIZADAS. \n\n\n");
	printf(" --> Seleccione una opcion: ");
	scanf( "%d", &entrada );

	if(entrada==1)
	{
		/* Importo matrices desde archivos, y realizo conexion con driver. Mediante file descriptor con device file /proc/matrixmodG_fd*/
		//new_MatrixmodGDriver(&driver, "../Caso_Aplicacion/mII.txt", "mIH.txt", "mIR.txt", NULL, "vMI.txt");
		//new_MatrixmodGDriver(&driver, "./RDPG/identity_1000x1000/mII.txt", "./RDPG/identity_1000x1000/mIH.txt", "./RDPG/identity_1000x1000/mIR.txt", "./RDPG/identity_1000x1000/mIRe.txt", "./RDPG/identity_1000x1000/vMI.txt");
		new_MatrixmodGDriver(&driver, "./RDPG/identity_1000x1000/mII.txt", NULL, NULL, NULL, "./RDPG/identity_1000x1000/vMI.txt");

		printf("\n\n");

		//debug_check_point("create RDPG ");

		printf("\n\n");
	}
	else if(entrada == 2) /* OPCION 2 */
	{
		/* Solo realizo conexion con driver. Mediante file descriptor con device file /proc/matrixmodG_fd */
		new_MatrixmodGDriver(&driver, NULL, NULL, NULL, NULL, NULL); /* No se vincula ningun archivo ya que la red del driver maneja una RDPG previamente cargada.*/
	}

	/* Inicio de variables*/
	sel_menu=0; /* Inicia menu principal. */
	memset(cadena, '\0', USR_BUF_SIZE);
	driver.LIBMATRIXMODG_MSG_TIMEOP = MESSAGES_ON; /* Visualizo mensaes de tiempos de operaciones. */

	if(entrada < 3)
	{
		/* --- MENU DE OPCIONES ---	*/
	    while ( (sel_menu != 3) && (db_exit==0) )
	    {
	        if(sel_menu == 0 || sel_menu > 3)
	        {
	        	printf("\n--------------------------------------------------------------------------------\n");
	        	printf( "\t\tIUTS MatrxiModG - MENU PRINCIPAL\n");
	        	printf("--------------------------------------------------------------------------------\n");
		        printf( "\n   %d. Ingresar a MENU DE OPERACIONES SEMAFOROS.", OPCION_1);
		        printf( "\n   %d. Ingresar a MENU DE OPERACIONES RDPG.", OPCION_2);
				printf( "\n   %d. Salir." , OPCION_3);
		        printf( "\n\n   Ingrese numero de opcion (%d-%d): ", OPCION_1, OPCION_3);

	        	scanf( "%d", &sel_menu );
	        }
	        else if(sel_menu == 1)
	        {
	        	printf("\n--------------------------------------------------------------------------------\n");
	        	printf( "\t\tIUTS MatrxiModG - MENU DE OPERACIONES SEMAFOROS \n");
	        	printf("--------------------------------------------------------------------------------\n");
		        printf( "\n   %d. Iniciar conjunto de semaforos.", OPCION_0);
		        printf( "\n   %d. Utilizar recurso de semaforo.", OPCION_1);
		        printf( "\n   %d. Liberar recurso de semaforo.", OPCION_2);
		        printf( "\n   %d. Borrar conjunto de semaforos.", OPCION_3);
		        printf( "\n   %d. Ver estado del conjunto de semaforos.", OPCION_4);
				printf( "\n   %d. Volver a MENU PRINCIPAL." , OPCION_5);
				printf( "\n   %d. Salir." , OPCION_6);
		        printf( "\n\n   Ingrese numero de opcion (%d-%d): ", OPCION_0, OPCION_6);

	        	scanf( "%d", &opcion_Msem );
	        	
	        	memset(sem_cmd_sel, '\0', 256);// se limpia cadena
	        	strcpy(sem_cmd_sel, program_sem_path);

	        	switch ( opcion_Msem )
		        {
		            case OPCION_0: printf( "\n   Inicializando conjunto de semaforos...   \n " );
		            		strcat(sem_cmd_sel, sem_cmd[OPCION_0]);
		                    system(sem_cmd_sel);
		                    break;

					case OPCION_1: printf("\n   Ocupando recurso de semaforo:  \n" );
							strcat(sem_cmd_sel, sem_cmd[OPCION_1]);
		                    system(sem_cmd_sel);
		                    break;
					
					case OPCION_2: printf("\n  Liberando recurso de semaforo: \n" );
							strcat(sem_cmd_sel, sem_cmd[OPCION_2]);
		                    system(sem_cmd_sel);
		                    break;

		            case OPCION_3: printf("\n  Borrando conjunto de semaforos... \n" );
							strcat(sem_cmd_sel, sem_cmd[OPCION_3]);
		                    system(sem_cmd_sel);
		                    break;

		            case OPCION_4: printf("\n  Capturando estado de semaforos... \n" );
							strcat(sem_cmd_sel, sem_cmd[OPCION_4]);
		                    system(sem_cmd_sel);
		                    break;

		            case OPCION_5: printf( "\n   Volviendo a menu principal...\n\n");
							sel_menu = 0; /* menu principal. */
		                    break;

		            case OPCION_6: printf( "\n   Saliendo de aplicacion.\n\n");
							sel_menu = 3; /* Sale del bucle while y sale de la aplicacion. */
		                    break;

		            default: printf("\n   Comando no valido. Intente nuevamente segun opciones de menu.\n " );

		        }
	        }
	        else if(sel_menu == 2)
	        {
	        	printf("\n--------------------------------------------------------------------------------\n");
	        	printf( "\t\tIUTS MatrxiModG - MENU DE OPERACIONES RDPG \n");
	        	printf("--------------------------------------------------------------------------------\n");
		        printf( "\n   %d. Ver informacion de RDPG del kernel.", OPCION_0);
		        printf( "\n   %d. Ver informacion de componente de RDPG del kernel.", OPCION_1);
		        printf( "\n   %d. Ver todos los componentes de la RDPG.", OPCION_5);
		        printf( "\n   %d. Ver matriz de incidencia de la RDPG.", OPCION_6);
		        printf( "\n   %d. Ver matriz de incidencia H de la RDPG.", OPCION_7);
		        printf( "\n   %d. Ver matriz de incidencia R de la RDPG.", OPCION_8);
		        printf( "\n   %d. Ver marcado inicial de la RDPG.", OPCION_9);
		        printf( "\n   %d. Ver marcado actual de la RDPG.", OPCION_10);
		        printf( "\n   %d. Ver marcado nuevo de la RDPG.", OPCION_11);
		        printf( "\n   %d. Ver vector E de transicicones sensibilizadas en la RDPG.", OPCION_12);
		        printf( "\n   %d. Ver vector B de transicicones inhibidas por arco inhibidor en la RDPG.", OPCION_13);
		        printf( "\n   %d. Ver vector L de transicicones inhibidas por arco lector en la RDPG.", OPCION_14);
		        printf( "\n   %d. Ver vector Ex de transicicones sensibilizadas extendido en la RDPG.", OPCION_15);
		        printf( "\n   %d. Ver vector W de funcion cero en la RDPG.", OPCION_16);
		        printf( "\n   %d. Ver vector Q de funcion cero en la RDPG.", OPCION_17);
		        printf( "\n   %d. Ver vectores de disparos posibles en la RDPG.", OPCION_18);
		        printf( "\n   %d. Disparar transicion sobre RDPG.", OPCION_19);
		        printf( "\n   %d. Disparar transicion con varios hilos.", OPCION_20);
		        printf( "\n   %d. Ver comandos de matrixmod." , OPCION_21);
		        printf( "\n   %d. Volver a MENU PRINCIPAL." , OPCION_22);
		        printf( "\n   %d. Salir." , OPCION_MAX);
		        printf( "\n\n   Ingrese numero de opcion (%d-%d): ", OPCION_0, OPCION_MAX);

		        scanf( "%d", &opcion_Mrdp );

	    	    /* Inicio del anidamiento */

		        switch ( opcion_Mrdp )
		        {
		            case OPCION_0: printf( "\n   \n" );
		                    driver.methods->view_RDPGinfo(&driver);
		                    break;

		            case OPCION_1: printf( "\n   \n" );
		                    driver.methods->view_compRDPG(&driver,view_infoComponent, cadena);
		                    break;

		            case OPCION_2: printf( "\n   \n" );
		                    driver.methods->view_compRDPG(&driver,view_infoTransitions, cadena);
		                    break;

		            case OPCION_3: printf( "\n   \n" );
		                    driver.methods->view_compRDPG(&driver,view_infoShots, cadena);
		                    break;

		            case OPCION_4: printf( "\n   \n" );
		                    driver.methods->view_compRDPG(&driver,view_infoMemory, cadena);
		                    break;

		            case OPCION_5: printf( "\n   \n" );
		                    driver.methods->view_allCompRDPG(&driver);
		                    break;

					case OPCION_6: printf("\n  Matriz de incidencia de la RDPG:\n" );
							//printf(" %s \n", n_transiciones);
							driver.methods->view_compRDPG(&driver,view_mII, cadena);
		                    break;                    

		            case OPCION_7: printf("\n  Matriz de incidencia de brazos inhibidores de la RDPG:\n" );
							//printf(" %s \n", n_transiciones);
						    driver.methods->view_compRDPG(&driver,view_mIH, cadena);
		                    break;

		            case OPCION_8: printf("\n  Matriz de incidencia de brazos lectores de la RDPG:\n" );
						    //printf(" %s \n", n_transiciones);
						    driver.methods->view_compRDPG(&driver,view_mIR, cadena);
		                    break;

		            case OPCION_9: printf("\n  Marcado inicial de la RDPG:\n" );
						    //printf(" %s \n", n_plazas);
						    driver.methods->view_compRDPG(&driver,view_vMI, cadena);
		                    break;

		            case OPCION_10: printf("\n  Marcado actual de la RDPG:\n" );
							//printf(" %s \n", n_plazas);
						    driver.methods->view_compRDPG(&driver,view_vMA, cadena);
		                    break;

		            case OPCION_11: printf("\n  Marcado nuevo de la RDPG:\n" );
						    //printf(" %s \n", n_plazas);
		            		driver.methods->view_compRDPG(&driver,view_vMN, cadena);
		                    break;

		            case OPCION_12: printf("\n  Vector E de transiciones sensibilizadas:\n" );
							//printf(" %s \n", n_transiciones);
						    driver.methods->view_compRDPG(&driver,view_vE, cadena);
		                    break;

		            case OPCION_13: printf("\n  Vector B de transiciones des-sensibilizadas por arco inhibidor:\n" );
						    //printf(" %s \n", n_transiciones);
						    driver.methods->view_compRDPG(&driver,view_vB, cadena);
		                    break;

		            case OPCION_14: printf("\n  Vector L de transiciones des-sensibilizadas por arco lector:\n" );
						    //printf(" %s \n", n_transiciones);
						    driver.methods->view_compRDPG(&driver,view_vL, cadena);
		                    break;

		            case OPCION_15:printf("\n  Vector Ex de transiciones sensibilizadas extendido: \n" );
							//printf(" %s \n", n_transiciones);
							driver.methods->view_compRDPG(&driver,view_vEx, cadena);
				            break;

				    case OPCION_16:printf("\n  Vector W de funcion uno: \n" );
							//printf(" %s \n", n_plazas);
							driver.methods->view_compRDPG(&driver,view_vW, cadena);
				            break;

				    case OPCION_17:printf("\n  Vector Q de funcion cero: \n" );
						    //printf(" %s \n", n_plazas);
							driver.methods->view_compRDPG(&driver,view_vQ, cadena);
				            break;

				    case OPCION_18:printf("\n  Vectores de disparos posibles: \n" );
						    //printf(" %s \n", n_disparos);
							driver.methods->view_compRDPG(&driver, view_mD, cadena);
							break;

		            case OPCION_19:printf( "\n   Ingrese numero de transicion a disparar: ");
		            		int n_transicion;
		            		scanf( "%d", &n_transicion);
		                    driver.methods->shoot_RDPG(&driver, n_transicion);
		                    break;

		            case OPCION_20: printf( "\n   Ingrese numero de transicion a disparar: ");
									scanf( "%s", cadena);
									printf( "\n   Ingrese numero de hilos/subprocesos paralelos: ");
									scanf( "%d", &nHilos);
									disparar_trasnsicion_nHilos(&driver,cadena, nHilos);
		            				break;
		            case OPCION_21:
		            		printf( "\n   Comandos disponibles: \n\n");
		            		driver.methods->view_allcmdRDPG();
		                    break;

		            case OPCION_22: printf( "\n   Volviendo a menu principal...\n\n");
							sel_menu = 0; /* menu principal. */
		                    break;

		            case OPCION_MAX:printf( "\n   Saliendo de aplicacion.\n\n");
		            				sel_menu = 3; /* Sale del bucle while y sale de la aplicacion. */
		            				break;

		            default: printf("\n   Comando no valido. Intente nuevamente segun opciones de menu.\n" );
		         }
		         /* Fin del anidamiento */
	        }
	    }
    }
    else
    {
		//st_cargar_componentes_v2(&DTests);
		st_cargar_componentes(&DTests);
		st_iniciar_vmuestra();
		const int N_P = 10; /* Numero de plazas para pruebas que lo utilizan. */
		const int M_T = 10; /* Numero de transiciones para pruebas que lo utilizan. */
		//size_t i;

        /* --- MENU DE OPCIONES ---	*/
	    while ( (opcion_MTEST != 10) && (db_exit==0) )
	    {
			printf("\n--------------------------------------------------------------------------------\n");
			printf( "\t\tIUTS MatrxiModG - PRUEBAS DE SISTEMA AUTOMATIZADAS \n");
			printf("--------------------------------------------------------------------------------\n");
	        printf( "\n   %d. Ejecutar CPS201_alloc_mem.", OPCION_1);
	        printf( "\n   %d. Ejecutar CPS201_alloc_mem_nxm.", OPCION_2);
	        printf( "\n   %d. Ejecutar CPS202_add_comps.", OPCION_3);
	        printf( "\n   %d. Ejecutar CPS203_create_rdp.", OPCION_4);
	        printf( "\n   %d. Ejecutar CPS204_delete_rdp.", OPCION_5);
	        printf( "\n   %d. Ejecutar CPS204_delete_rdp_nxm.", OPCION_6);
	        printf( "\n   %d. Ejecutar CPS205_shoot.", OPCION_7);
	        printf( "\n   %d. Ejecutar CPS206_readAllComp.", OPCION_8);
	        printf( "\n   %d. Ejecutar CPS207_readRDPGinfo.", OPCION_9);
			printf( "\n   %d. Salir." , OPCION_10);
	        printf( "\n\n   Ingrese numero de opcion (%d-%d): ", OPCION_0, OPCION_10);

        	scanf( "%d", &opcion_MTEST );

        	switch ( opcion_MTEST )
	        {
	            case OPCION_1: printf( "\n   Ejecutando CPS201_alloc_mem...   \n" );
	            		CPS201_alloc_mem(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_2: printf( "\n   Ejecutando CPS201_alloc_mem_nxm. Para RDPG con %d plazas y %d transiciones.   \n", N_P, M_T);
	            		CPS201_alloc_mem_nxm(&DTests, N_P, M_T);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_3: printf( "\n   Ejecutando CPS202_add_comps...   \n" );
	            		CPS202_add_comps(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_4: printf( "\n   Ejecutando CPS203_create_rdp...   \n" );
	            		CPS203_create_rdp(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_5: printf( "\n   Ejecutando CPS204_delete_rdp...   \n" );
	            		CPS204_delete_rdp(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_6: printf( "\n   Ejecutando CPS204_delete_rdp_nxm. Para RDPG con %d plazas y %d transiciones.   \n", N_P, M_T);
	            		CPS204_delete_rdp_nxm(&DTests, N_P, M_T);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_7: printf( "\n   Ejecutando CPS205_shoot.   \n");
	            		CPS205_shoot(&DTests);
	            		st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_8: printf( "\n   Ejecutando CPS206_readAllComp.   \n");
	            		CPS206_readAllComp(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_9: printf( "\n   Ejecutando CPS207_readRDPGinfo.   \n");
	            		CPS207_readRDPGinfo(&DTests);
	            		//st_muestras(); /* Descomentar para ver muestras de tiempos. */
	            		st_promedio();
	                    break;

	            case OPCION_10: printf( "\n   Saliendo de aplicacion.\n\n");
						opcion_MTEST = 10; /* Sale del bucle while y sale de la aplicacion. */
	            		//DTests.methods->matrixmodG_delRDPG(&DTests);
	            		//DTests.methods->disconnect_driverMatrixmodG(&DTests);
	            		st_liberar_componentes(&DTests);
	                    break;

	            default: printf("\n   Comando no valido. Intente nuevamente segun opciones de menu.\n " );

	        }
        }
    }

    if(entrada < 3)
    {
    	/* Consulta de eliminacion de RDPG cargada en kernel. */ 
    	driver.methods->matrixmodG_delRDPG(&driver);

    	/* Close de files descriptors asociado a modulo */
		driver.methods->disconnect_driverMatrixmodG(&driver);
    }

    return 0;
}



/*
* 
*/
void debug_check_point(char * p_comando)
{
	if(db_exit==0)
	{
		int entrada;
		printf("\n   Proximo comando: %s\n", p_comando);
		printf("   Debug_matrixmod_input (0 para continuar, 1 para finalizar): ");
		scanf( "%d", &entrada );

		if(entrada == 0)
			return;
		
		else
			db_exit=1;
	}
	else
		printf("\n   Debug_matrixmod_check_poin: Ignorado!\n");
}


/**
 * @brief      Esta funcion se encarga de disparar una transicion por n hilos paralelamente.
 *
 * @param      cadena  The cadena
 * @param[in]  pfd     The pfd
 */
void disparar_trasnsicion_nHilos(DriverRDPG_o *p_DriverObj, char *cadena, unsigned int p_nHilos)
{
	//
	char comando[256] = "RDPG shoot "; // comando para realizar disparo
	int id_thread, nThreads;

	strcat(comando, cadena); // se agrega cadena(nro transicion) al final de comando

	if((p_nHilos > 4) || (p_nHilos == 0))
	{
		omp_set_num_threads(4);
	}
	else 
	{
		omp_set_num_threads(p_nHilos);
	}

	nThreads = omp_get_max_threads(); /* Calculo de numero de hilos configurados paralelamente.*/

	printf("   Comando de disparo: %s\n", comando);
	printf("   Numero de hilos/subprocesos paralelos: %d\n\n", nThreads);

	/* Variables para medicion de tiempos. */
	double t_ini = 0;
	double t_fin = 0;
	clock_t t_ini2 = 0;
	clock_t t_fin2 = 0;

	/* Comienza cuenta de tiempo. */
	t_ini = omp_get_wtime();
	t_ini2 = clock();

	switch (nThreads)
    {
        case 2: printf( "\n   Ejecutando disparo con dos subprosesos paralelos...  \n");
        		disparo_dos_hilos(p_DriverObj, comando);
                break;

        case 3: printf( "\n   Ejecutando disparo con tres subprosesos paralelos...  \n");
        		disparo_tres_hilos(p_DriverObj, comando);
                break;

        case 4: printf( "\n   Ejecutando disparo con cuatro subprosesos paralelos...  \n");
        		disparo_cuatro_hilos(p_DriverObj, comando);
                break;

        default: printf( "\n   Falla de numero de hilos.  \n " );
                 break;
    }


	/* Finaliza cuenta de tiempo. */
	t_fin2 = clock();
	t_fin = omp_get_wtime();

	printf("   Tiempo de operacion (con API openMP): %lf seg. \n", (t_fin-t_ini));
	printf("   Tiempo de operacion (con API timer): %lf seg. \n\n", ((double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC));

	memset(cadena, '\0', USR_BUF_SIZE);// se limpia cadena
}


/**
  * @brief    Esta funcion forma parte del caso de prueba de sistema CPS101. Se encarga de probar a nivel de sistema si el disparo de
  * una transicion se efectua correctamente en la RDPG del modulo cuando se dispara la transicion por dos hilos/subprocesos paralelamente.
 *
 * @param      p_comando  Puntero a la cadena que contiene el comando del numero de transicion a disparar en el modulo.
 * @param[in]  pfd        Este parametro contiene el numero de file descriptor del archivo de dispositivo del modulo.
 * 
 */
void disparo_dos_hilos(DriverRDPG_o *p_DriverObj, char * p_comando)
{
	int id_thread;

	#pragma omp parallel private(id_thread)
    {
 
    id_thread = omp_get_thread_num();
 
    #pragma omp sections
    {
    	#pragma omp section
      	{
          	printf("   Hilo %d ejecutando disparo... \n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj, p_comando);
			//verificar_disparo(pfd);
			//system("cat /sys/kernel/debug/ktf/run/BPU3_FuncionesDinamicaRDP-tests/CPU3_1_RDP_dispararT0");
      	}
 
	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj, p_comando);
			//verificar_disparo(pfd);
			//system("cat /sys/kernel/debug/ktf/run/BPU3_FuncionesDinamicaRDP-tests/CPU3_1_RDP_dispararT0");
	    }
 
    }//Fin Sections
 
    }//Fin Parallel
}


/**
  * @brief    Esta funcion forma parte del caso de prueba de sistema CPS101. Se encarga de probar a nivel de sistema si el disparo de
  * una transicion se efectua correctamente en la RDPG del modulo cuando se dispara la transicion por tres hilos/subprocesos paralelamente.
  *
  * @param      p_comando  Puntero a la cadena que contiene el comando del numero de transicion a disparar en el modulo.
  * @param[in]  pfd        Este parametro contiene el numero del file descriptor del archivo de dispositivo del modulo.
  * 
 **/
void disparo_tres_hilos(DriverRDPG_o *p_DriverObj, char * p_comando)
{
	int id_thread;

	#pragma omp parallel private(id_thread)
    {
 
    id_thread = omp_get_thread_num();
 
    #pragma omp sections
    {
    	#pragma omp section
      	{
          	printf("   Hilo %d ejecutando disparo... \n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj, p_comando);
      	}
 
	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj, p_comando);
	    }

	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj, p_comando);
	    }
 
    }//Fin Sections
 
    }//Fin Parallel
}


/**
  * @brief    Esta funcion forma parte del caso de prueba de sistema CPS101. Se encarga de probar a nivel de sistema si el disparo de
  * una transicion se efectua correctamente en la RDPG del modulo cuando se dispara la transicion por cuatro hilos/subprocesos paralelamente.
  *
  * @param      p_comando  Puntero a la cadena que contiene el comando del numero de transicion a disparar en el modulo.
  * @param[in]  pfd        Este parametro contiene el numero del file descriptor del archivo de dispositivo del modulo.
  * 
 **/
void disparo_cuatro_hilos(DriverRDPG_o *p_DriverObj, char * p_comando)
{
	int id_thread;

	#pragma omp parallel private(id_thread)
    {
 
    id_thread = omp_get_thread_num();
 
    #pragma omp sections
    {
    	#pragma omp section
      	{
          	printf("   Hilo %d ejecutando disparo... \n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj,p_comando);
      	}
 
	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj,p_comando);
	    }

	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj,p_comando);
	    }

	    #pragma omp section
		{
	        printf("   Hilo %d ejecutando disparo...\n", id_thread);
          	// Write sobre driver -> se realiza disparo
			write_matrixmodG(p_DriverObj,p_comando);
	    }
 
    }//Fin Sections
 
    }//Fin Parallel
}
