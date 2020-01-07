/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Device Driver Linux MatrixmodG (archivo fuente - source file).
 * 
 * 
 * Este archivo forma parte del codigo fuente del Device Driver Linux MatrixmodG. Este driver se encarga de gestionar Redes de Petri Generalizadas (RDPG) 
 * en eñ kernel de Linux mediante las solicitudes que indican las aplicaciones de usuario al archivo de dispositivo "/proc/matrixmod_fd".
 * 
 * Las RDPG se tratan como objetos RDPG_o creado como clase desde la libreria de kernel RDPG_object.h/.c.
 * 
 * La libreria fue testeada por un conjunto de pruebas unitarias e integrales mediante el framework Kernel Test Framework (KTF). Es por lo cual la libreria 
 * durante un proceso de ejecucion de pruebas hace uso de las librerias KTF y habilita todas las funcionalidades del framework, en caso contrario por 
 * defecto, en proceso de ejecucion estandar, se ignoran todas las definiciones de KTF.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "matrixmodG.h"            /* Inclusion de enumeraciones, funciones, y variables publicas utlizadas por matrixmod.c */

char kbuf[BUFFER_LENGTH_RD];

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE FUNCIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
* Llamada cuando  un proceso intentó abrir el archivo de dispositivo, como
* "cat /dev/mycharfile".
*/
static int matrixmodG_open(struct inode *inode, struct file *file)
{
	if (Device_Open > 10)
	        return -EBUSY;
 
    	Device_Open++;
 
    	try_module_get(THIS_MODULE);
    	return SUCCESS;
}

/*
* Se llama cuando un proceso cierra el archivo de dispositivo.
*/
int matrixmodG_release(struct inode *inode, struct file *file)
{
    	Device_Open--;
    	/* No estamos listos para nuestro siguiente llamada */
    	/*
    	* Decrementar el contador de uso, o de lo contrario una vez que ha
    	* abierto el archivo, usted nunca se deshacera del módulo.
    	*/
    	module_put(THIS_MODULE);
    	return SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion realiza la lectura de los datos del espacio usuario. Toda escritura de un usuario al file device del driver es lo que el kernel
 * lee para extraer los datos escritos por el usuario. Los datos que puede escribir el usuario son: los diferentes comandos con la informacion asociada
 * para que el driver la procese y trabaje de acuerdo a cada caso en particular relacionado con la RDPG gestionada por el driver.
 *
 * @param      filp         Puntero a la estructura file del archivo de dispositivo utilizado por el driver.
 * @param[in]  __user *buf  Puntero al buffer del usuario de donde provienen los datos hacia el kernel. Es el buffer que escribe el usurio y que el 
 *                          kernel lee y copia para extraer los datos del usuario.
 * @param[in]  len          Tamaño de la transferencia de datos solicitada hacia el espacio kernel. Es el tamaño de datos que brinda el usuario, o del
 *                          buffer de usuario, para que se copien los datos en el kernel.
 * @param      off          Puntero a la posicion del archivo a la que el kernel va accediendo.
 *
 * @return     Se retorna la cantidad de caracteres escritos por el usuario, o el numero de tokens de cualquiera de las plazas de la RDPG que gestiona
 * el driver. En caso de error se retorna un numero menor que cero asociado al codigo de error.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t matrixmodG_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {

  int available_space = BUFFER_LENGTH_WR; /* espacio disponible */
  int rt_op = -EC_falloADD; /* retorno entero de operaciones. */
  
  char kbuf[BUFFER_LENGTH_WR];
  char entrada[COMMANDSIZE];
  char entrada2[COMMANDSIZE];
  char entrada3[COMMANDSIZE];

  /* Limpio entrada de capturas. */
  memset(kbuf, '\0', BUFFER_LENGTH_WR);
  memset(entrada, '\0', COMMANDSIZE);
  memset(entrada2, '\0', COMMANDSIZE);
  memset(entrada3, '\0', COMMANDSIZE);
  //int error=0;
  /* if ((*off) > 0)  La aplicación puede escribir en esta entrada una sola vez !! 
    return 0; */
  
  if (len > available_space) {
    printk(KERN_INFO "matrixmodG_error: No hay espacio sufuciente!!\n");
    return -ENOSPC; // No queda espacio en el dispositivo
  }

  /* Transferencia de datos de usuario al espacio kernel 
     ptr destino : kbuf
     ptr origen : buf
     nrbytes: len
  */
  if (copy_from_user( kbuf, buf, len ))  
    return -EFAULT; // Direccion incorrecta
  
  kbuf[len]='\0'; // Añadimos el fin de la cadena al copiar los datos from user space.
  *off+=len;            /* Actualizar el puntero del archivo */

  
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * COMANDOS DE CONFIGURACION DE PARAMETROS ASOCIADOS A LA RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
  if ( sscanf(kbuf,"RDPG config alloc_mode %s\n", entrada) == 1){
  
    RDP1.methods->set_MemAllocMode(&RDP1, entrada);

  }else if ( sscanf(kbuf,"RDPG config posVP %s", entrada) == 1){
  
    RDP1.methods->set_posVP(&RDP1, entrada);

  }else if ( sscanf(kbuf,"RDPG config posVT %s", entrada) == 1){
  
    RDP1.methods->set_posVT(&RDP1, entrada);

  }else if ( sscanf(kbuf,"RDPG config vdim %s", entrada) == 1){
  
    RDP1.methods->set_vdim(&RDP1, entrada);

  }

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * COMANDOS DE CONFIGURACION DE LA RDPG Y SUS COMPONENTES
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
  /* sscanf() return : el nº de elementos asignados */
  else if( sscanf(kbuf,"create RDPG %s",entrada) == 1) {

    RDP1.methods->create_rdpg(&RDP1, entrada); /* Se crean todos los componentes de la RDPG si esta no existe previamente. */
    
  }else if( sscanf(kbuf,"RDPG add %s", entrada) == 1){

    if( sscanf(kbuf,"RDPG add mII %s", entrada2) == 1){
    
      (void)RDP1.methods->add_value_in_mcomponent(&(RDP1.mII), entrada2);

    }else if( sscanf(kbuf,"RDPG add mIH %s", entrada2) == 1){
      
      (void)RDP1.methods->add_value_in_mcomponent(&(RDP1.mIH), entrada2);

    }else if( sscanf(kbuf,"RDPG add mIRe %s", entrada2) == 1){
      
      (void)RDP1.methods->add_value_in_mIRe(&RDP1, entrada2);

    }else if( sscanf(kbuf,"RDPG add mIR %s", entrada2) == 1){
      
      (void)RDP1.methods->add_value_in_mcomponent(&(RDP1.mIR), entrada2);

    }else if( sscanf(kbuf,"RDPG add vMI %s", entrada2) == 1){

      (void)RDP1.methods->add_value_in_vcomponent(&(RDP1.vMI), entrada2);
      (void)RDP1.methods->add_value_in_vcomponent(&(RDP1.vMA), entrada2); 

    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "matrixmodG_error: fallo comando add, componete '%s' no valido!!!\n", entrada);
    }
  }else if( sscanf(kbuf,"RDPG set vG %s", entrada) == 1){
      
      (void)RDP1.methods->add_value_vG(&RDP1, entrada);

  }else if( ( sscanf(kbuf,"RDPG inc vHQCV %s\n", entrada) == 1)){
      
      RDP1.methods->SMPs_inc_vHQCV(&RDP1, entrada);

  }else if( ( sscanf(kbuf,"RDPG dec vHQCV %s\n", entrada) == 1)){
      
      RDP1.methods->SMPs_dec_vHQCV(&RDP1, entrada);

  }else if ( strcmp(kbuf,"RDPG confirm\n") == 0){ /* strcmp() return : 0 -> si son iguales */

    RDP1.methods->load_vcomp_with_vcomp(&RDP1, vMA, vMI);   /* Solo por primera vez se carga vMA como vMI.*/
    RDP1.methods->update_work_components(&RDP1);

  }else if ( strcmp(kbuf,"RDPG delete\n") == 0){

    RDP1.methods->delete_rdpg(&RDP1);

  }

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * COMANDOS DEL CONTROL DEL COMPORTAMIENTO DE LA RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
else if ( sscanf(kbuf,"RDPG shoot%s", entrada) == 1){
  
    return (ssize_t)RDP1.methods->SMPs_shoot_rdpg_s(&RDP1, entrada, SHOT_MODE_E); /* Casteo el retorno entero a void para ignorarlo. */
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * COMANDOS DE LECTURA DE COMPONENTES
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
else if( ( sscanf(kbuf,"RDPG cat %s\n", entrada) == 1)){
     
      if( strcmp(entrada,"mII") == 0){

        RDP1.methods->SMPs_set_catComp(&RDP1, cat_mII);

      }else if ( strcmp(entrada,"mIH") == 0){

        RDP1.methods->SMPs_set_catComp(&RDP1, cat_mIH);

      }else if ( strcmp(entrada,"mIR") == 0){
  
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_mIR);

      }else if ( strcmp(entrada,"mIRe") == 0){
  
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_mIRe);

      }else if ( strcmp(entrada,"mD") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_mD);

      }else if ( strcmp(entrada,"vMA") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vMA);

      }else if ( strcmp(entrada,"vMI") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vMI);

      }else if ( strcmp(entrada,"vMN") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vMN);

      }else if ( strcmp(entrada,"vE") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vE);

      }else if ( strcmp(entrada,"vL") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vL);

      }else if ( strcmp(entrada,"vB") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vB);

      }else if ( strcmp(entrada,"vG") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vG);

      }else if ( strcmp(entrada,"vA") == 0){
      
         RDP1.methods->SMPs_set_catComp(&RDP1, cat_vA);

      }else if ( strcmp(entrada,"vQ") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vQ);

      }else if ( strcmp(entrada,"vW") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vW);

      }else if ( strcmp(entrada,"vUDT") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vUDT);

      }else if ( strcmp(entrada,"vEx") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vEx);

      }else if ( strcmp(entrada,"vHQCV") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vHQCV);

      }else if ( strcmp(entrada,"vHD") == 0){
      
        RDP1.methods->SMPs_set_catComp(&RDP1, cat_vHD);

      }
      else
      {
        RDP1.methods->SMPs_set_read_mode(&RDP1, ID_ERROR_MODE);
      }
  }
  

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * COMANDOS DE REPORTE DE INFORMACION DE LA RDPG Y SUS COMPONENTES
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
  else if ( sscanf(kbuf,"RDPGinfo %s\n", entrada) == 1){

    if ( strcmp(entrada,"name") == 0){
  
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_NAME);

    }else if ( strcmp(entrada,"places") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_PLACES);

    }else if ( strcmp(entrada,"transitions") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_TRANSITIONS);

    }else if ( strcmp(entrada,"shots") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_SHOTS);

    }else if ( strcmp(entrada,"memory") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_MEMORY);

    }else if ( strcmp(entrada,"comp") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_COMP);

    }else if ( strcmp(entrada,"shot_result") == 0){
    
      RDP1.methods->set_read_mode(&RDP1, ID_INFO_SHOT_RESULT);

    }else
    {
      RDP1.methods->set_read_mode(&RDP1, ID_ERROR_MODE);
    }
  }else if( ( sscanf(kbuf,"RDPG get tokens %s\n", entrada) == 1)){
      
      return (ssize_t)RDP1.methods->SMPs_get_TokensPlace(&RDP1, entrada);

  }else if( ( sscanf(kbuf,"RDPG get vHD %s\n", entrada) == 1)){
      
      return (ssize_t)RDP1.methods->SMPs_get_vHDelement(&RDP1, entrada);

  }else if( strcmp(kbuf,"returnNum\n") == 0){

      return -10;

  }else if( strcmp(kbuf,"return20\n") == 0){

      return 20;

  }else{
	    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "matrixmodG_error: comando: '%s' no es un comando valido!!!\n", kbuf);

      printk(KERN_INFO "matrixmodG_error: comando: '%s' no es un comando valido!!!\n", kbuf);
      //return -1; /* Comando no valido. */
    }

  //count_read = 0; /* Reinicia el contador de lectura para que se puede leer ante la nueva escritura */

  return len;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion realiza la escritura de datos al espacio usuario, de esta manera es como un usuario lee lo que escribe el driver en el file
 * device asociado al driver. Los datos que se tranfieren al espacio usuario en este driver pueden ser datos de informacion de la RDPG gestionada por
 * el driver o datos del estado de cualquiera de los componentes de la RDPG gestionada en el driver.
 *
 * @param      filp         Puntero a la estructura file del archivo de dispositivo utilizado por el driver.
 * @param[in]  __user *buf  Puntero al buffer del usuario en donde se almacenaran los datos del kernel. Es el buffer que lee el usurio luego de que el 
 *                          kernel escribe los datos en el mismo.
 * @param[in]  len          Tamaño de la transferencia de datos solicitada hacia el espacio usuario. Es el tamaño que brinda el usuario para que se escriban los
 *                          datos del kernel.
 * @param      off          Puntero a la posicion del archivo a la que el usuario va accediendo.
 *
 * @return     Se retorna el numero de caracteres leidos por el espacio usuario, equivalen a los caracteres escritos por el driver al buffer de espacio
 * usuario.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ssize_t matrixmodG_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	int nr_bytes = 0;

  if(RDP1.read_mode < ID_INFO_MODE) /* Modo lectura RDPcomponent*/
  {
    (void)RDP1.methods->SMPs_read_rdpg_component(&RDP1, kbuf, RDP1.select_comp, RDP1.read_mode); /* Se realiza lectura de componente de RDP1 seleccionada para imprimir al usuario. */
    nr_bytes=print_to_user(kbuf, buf, len);
  }
  else if(RDP1.read_mode <= ID_ERROR_MODE) /* Modo lectura RDPInfo*/
  {
    (void)RDP1.methods->SMPs_read_rdpg_info(&RDP1, kbuf); /* Se realiza lectura de informaciion general de RDP1 para imprimir al usuario. */
    nr_bytes=print_to_user(kbuf, buf, len);
  }

  if(nr_bytes < 1)
  {
    return 0;
  }

  /* Se indica a proceso actual que ya no hay nada que leer  "Para no copiar basura si llamas otra vez", debido a funcion reentrante. */
  if ((*off) > 0){
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "matrixmodG_notify: no hay nada que leer. \n");
    
    return 0;
  }

	(*off)+=len;  /* Actualizo el puntero de archivo. */
	return nr_bytes; 
}


static int __init init_matrixmodG(void)
{
	int ret = 0;

	/* en matrixmod defenido en /proc, solo podemos usar las funciones
   defenidas en proc_entry_fops */
	proc_entry = proc_create( "matrixmodG_fd", 0666, NULL, &proc_entry_fops); 
	
  if(proc_entry == NULL) {
  		ret = -ENOMEM;          /* No hay bastante espacio */
  		printk(KERN_ERR "matrixmodG_error: No se puede crear la entrada /proc/matrixmod, no hay espacio suficiente. \n");
	} 
  else {
  	RDPG_firm();
    printk(KERN_INFO "matrixmodG_info: Modulo cargado en kernel.\n");
    
    new_RDPG(&RDP1, "RDP1"); /* Construccion de un nuevo objeto RDPG_o con nombre RDP1. */
	}

#ifdef MT_TEST_ON            /* Si modulo esta en proceso de prueba, se adicionan todas las pruebas sobre el modulo. */

  add_tests();

#endif

	return ret;
}


static void __exit exit_matrixmodG(void)
{
  RDP1.methods->delete_rdpg(&RDP1);         /* Elimino objeto RDPG_o RDP1. */

  remove_proc_entry("matrixmodG_fd", NULL); /* Elimino la entrada del /proc */
  RDPG_firm();

#ifdef MT_TEST_ON /* Si modulo esta en proceso de prueba, se finaliza el proceso de prueba con el framework KTF. */
  
  KTF_CLEANUP(); /* Finalizo testeo de modulo con framework KTF. */

#endif

  printk(KERN_INFO "matrixmodG_info: Modulo descargado de kernel.\n");
}

module_init(init_matrixmodG);
module_exit(exit_matrixmodG);
