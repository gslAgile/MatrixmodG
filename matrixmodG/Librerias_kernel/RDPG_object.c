/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria RDPG_object.c (archivo fuente - source file).
 * 
 * 
 * Esta libreria contiene las declaraciones y definiciones de la entidad RDPG_o y sus metodos asociados. Esta entidad permite
 * gestionar las RDPG como objetos en el kernel de Linux. 
 * 
 * La libreria gestiona objetos matrices y vectores declarados y definidos en la libreria MV_object.h/.c, entidades que se utilizan como objetos que componen
 * una RDPG.
 * 
 * La libreria proporciona control de errores de datos y formatos de datos propio determinado por la libreria sformat_control.h/.c.
 * 
 * La libreria provee de un conjunto de metodos de codigo SMPs para proteger las RDPG de los problemas de concurrencia. El mecanismo utilizado para brindar
 * sincronizacion entre procesos concurrentes es mediante spinlocks lector-escritor. Se recomienda hacer uso de estos metodos para evitar errores de datos
 * desde las aplicaciones de usuario.
 * 
 * La libreria fue testeada por un conjunto de pruebas unitarias e integrales mediante el framework Kernel Test Framework (KTF). Es por lo cual la libreria 
 * durante un proceso de ejecucion de pruebas hace uso de las librerias KTF y habilita todas las funcionalidades del framework, en caso contrario por 
 * defecto, en proceso de ejecucion estandar, se ignoran todas las definiciones de KTF.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "RDPG_object.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE VARIABLES GLOBALES PARA OBJETOS RDPG_o
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

const char *s_space1 = " ";       /* String con 1 espacio. */
const char *s_space2 = "  ";      /* String con 2 espacios. */
const char *s_space3 = "   ";     /* String con 3 espacios. */
const char *s_space4 = "    ";    /* String con 4 espacios. */
const char *s_space5 = "     ";   /* String con 5 espacios. */
const char *s_space6 = "      ";  /* String con 6 espacios. */
const char *s_newLine = "\n";     /* String de salto de linea. */

const char name_error[] = "-Sin nombre-";
static short int count_global_RDPG_o = 0; /* Contador global del numero de objetos RDPG_o creados por la libreria. */

/* Inicio de spinlocks */
//DEFINE_SPINLOCK(mtx_RDP1); /* mutex para control de funcion de disparos en RDPG_o */

/* Declaracion global de vtable para objetos RDPG_o. Todos los objetos RDPG_o tendran los mismos metodos. */
struct RDPG_methods RDPG_vtable = 
{
  .create_rdpg = create_rdpg,
  .delete_rdpg = delete_rdpg,
  .add_value_in_mcomponent = add_value_in_mcomponent,
  .add_value_in_vcomponent = add_value_in_vcomponent,
  .add_value_in_mIRe = add_value_in_mIRe,
  .add_value_vG = add_value_vG,
  .update_work_components = update_work_components,
  .update_vG = update_vG,
  .load_vcomp_with_vcomp = cargar_Dvcomp_con_Ovcomp,
  .shoot_rdpg = disparar_rdpg,
  .read_rdpg_component = read_rdpg_component,
  .read_rdpg_info = read_rdpg_info,
  .rdpg_empty = rdpg_empty,
  .get_TokensPlace = get_TokensPlace,
  .get_vHDelement = get_vHDelement,
  
  /* Asociacion de puntero de funcion con definicion de funciones de seteo de campos. */
  .set_MemAllocMode = set_MemAllocMode,
  .set_select_comp = set_select_comp,
  .set_read_mode = set_read_mode,
  .set_read_smode = set_read_smode,
  .set_posVP = set_posVP,
  .set_posVT = set_posVT,
  .set_vdim = set_vdim,
  .set_catComp = set_catComp,
  .inc_vHQCV = inc_vHQCV,
  .dec_vHQCV = dec_vHQCV,

  /* Asociacion de punteros de funcion con definicion de funciones con control de entrada de caracter (sformat_control.h).*/
  .shoot_rdpg_s = disparar_rdpg_s,

  /* Asociacion de punteros de funcion con definicion de funciones SMP-secure.*/
  .SMPs_shoot_rdpg_s = SMPs_disparar_rdpg_s,
  .SMPs_shoot_rdpg = SMPs_disparar_rdpg,
  .SMPs_add_value_vG = SMPs_add_value_vG,
  .SMPs_update_work_components = SMPs_update_work_components,
  .SMPs_load_vcomp_with_vcomp = SMPs_load_vcomp_with_vcomp,
  .SMPs_rdpg_empty = SMPs_rdpg_empty,
  .SMPs_get_TokensPlace = SMPs_get_TokensPlace,
  .SMPs_get_vHDelement = SMPs_get_vHDelement,
  .SMPs_set_MemAllocMode = SMPs_set_MemAllocMode,
  .SMPs_set_select_comp = SMPs_set_select_comp,
  .SMPs_set_read_mode = SMPs_set_read_mode,
  .SMPs_set_read_smode = SMPs_set_read_smode,
  .SMPs_set_posVP = SMPs_set_posVP,
  .SMPs_set_posVT = SMPs_set_posVT,
  .SMPs_set_vdim = SMPs_set_vdim,
  .SMPs_set_catComp = SMPs_set_catComp,
  .SMPs_inc_vHQCV = SMPs_inc_vHQCV,
  .SMPs_dec_vHQCV = SMPs_dec_vHQCV,
  .SMPs_read_rdpg_component = SMPs_read_rdpg_component,
  .SMPs_read_rdpg_info = SMPs_read_rdpg_info

  
  /* Otra forma de asignar las funciones a los punteros de funcion.
  &create_rdpg,
  &delete_rdpg,
  &delete_rdpg_mcomponent,
  &add_value_in_mcomponent,
  &cargar_Dvcomp_con_Ovcomp,
  &update_work_components,
  &disparar_rdpg,
  &set_read_mode,
  &read_rdpg_component
  &SMPs_disparar_rdpg
   */
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE FUNCIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*
* Descripcion: Esta funcion...
* @param:
*/
extern void RDPG_firm(void)
{ 
  if(MATRIXMODG_DB_MSG) printk(KERN_INFO "***************** RDPG Hello FIRM ******************* \n\n");
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion es el contructor de un nuevo objeto RDPG_o. Se encarga de asignar todos los metodos que puede realizar un objeto
 * RDPG_o a traves de una vtable RDPG_methods y de inicializar todos los campos del objeto RDPG_o.
 *
 * @param      p_rdp   Puntero a la direccion de memoria de un objeto RDPG_o sobre el que se desea realizar la asignacion e inicializaciones.
 * @param      p_name  Puntero a la direccion de memoria de una cadena de caracteres utilizada para inicializar el nombre de la RDPG_o enviada por parametro.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void new_RDPG(RDPG_o *p_rdp, char *p_name)
{
  if(p_rdp->mc != OBJ_CREATED) /* Si objeto RDPG_o p_rdp no fue creado previamente. Se inicializan sus campos.*/
  {
    /* Primero que todo, asigno metodos a la vtable del objeto RDPG_o para que conozca todos sus metodos. */
    p_rdp->methods = &RDPG_vtable;

    /* Inicio protector de recurso para la RDPG. */
    rwlock_init(&p_rdp->lock_RDPG);

    /* Inicializo por defecto los campos del objeto matrix_o. */
    new_matrix(&(p_rdp->mII), "< Matriz de incidencia I >");
    new_matrix(&(p_rdp->mIH), "< Matriz de incidencia H >");
    new_matrix(&(p_rdp->mIR), "< Matriz de incidencia R >");
    new_matrix(&(p_rdp->mIRe), "< Matriz de incidencia Re >");
    new_matrix(&(p_rdp->mD), "< Matriz de vectores de disparos >");

    /* Inicializo por defecto los campos del objeto matrix_o. */
    new_vector(&(p_rdp->vMI), "< Vector de marcado inicial >");
    new_vector(&(p_rdp->vMA), "< Vector de marcado actual >");
    new_vector(&(p_rdp->vMN), "< Vector de marcado nuevo >");
    new_vector(&(p_rdp->vE), "< Vector de transiciones sensibilizadas E >");
    new_vector(&(p_rdp->vQ), "< Vector de funcion Q >");
    new_vector(&(p_rdp->vW), "< Vector de funcion W >");
    new_vector(&(p_rdp->vL), "< Vector de transiciones des-sensibilizadas L >");
    new_vector(&(p_rdp->vB), "< Vector de transiciones des-sensibilizadas B >");
    new_vector(&(p_rdp->vG), "< Vector de transiciones des-sensibilizadas G >");
    new_vector(&(p_rdp->vA), "< Vector de transiciones reset A >");
    new_vector(&(p_rdp->vUDT), "< Vector UDT >");
    new_vector(&(p_rdp->vEx), "< Vector de transiciones sensibilizadas generalizado Ex >");
    new_vector(&(p_rdp->vHQCV), "< Vector de hilos en cola de variables de condicion >");
    new_vector(&(p_rdp->vHD), "< Vector de hilos a despertar >");

    /* Definicion de ID de objetos matrix_o. */
    p_rdp->mII.obj_id= mII;
    p_rdp->mIH.obj_id= mIH;
    p_rdp->mIR.obj_id= mIR;
    p_rdp->mIRe.obj_id= mIRe;
    p_rdp->mD.obj_id= mD;

    /* Definicion de ID de objetos vector_o. */
    p_rdp->vMI.obj_id= vMI;
    p_rdp->vMA.obj_id= vMA;
    p_rdp->vMN.obj_id= vMN;
    p_rdp->vE.obj_id= vE;
    p_rdp->vQ.obj_id= vQ;
    p_rdp->vW.obj_id= vW;
    p_rdp->vL.obj_id= vL;
    p_rdp->vB.obj_id= vB;
    p_rdp->vG.obj_id= vG;
    p_rdp->vA.obj_id= vA;
    p_rdp->vEx.obj_id= vEx;
    p_rdp->vUDT.obj_id= vUDT;
    p_rdp->vHD.obj_id= vHD;
    p_rdp->vHQCV.obj_id= vHQCV;
    

    set_read_mode(p_rdp, ID_MCOMPONENT_MODE); /* Se configura por defecto el modo de lectura de componente matriz. */
    set_read_smode(p_rdp, ID_MCOMP_SMODE); /* Se configura por defecto el sub-modo de lectura de informacion de componente matriz. */
    set_select_comp(p_rdp, (int)mII); /* Se muestra por defecto la matriz de incidencia. */
    p_rdp->posVP = 0;
    p_rdp->posVT = 0;
    p_rdp->vdim = MAX_VDIM;
    p_rdp->shot_result = (int)SHOT_INIT;
    p_rdp->error_code = EC_NULL;

    /*Asignacion de nombre de RDP. */
    if(p_name != NULL)
    {
      strcpy(p_rdp->name, p_name);
    }
    else
    {
      strcpy(p_rdp->name, "<Sin definir>");
    }
  }
  else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Error al inicializar nueva RDPG, la red con nombre %s - id: %d ya fue inicializada !!!\n", p_rdp->name, p_rdp->obj_id);
  }   
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion crea todos los componentes asociados a una RDPG_o. Los componetes son las matrices y vectores asociados a la RDPG, todos 
 * los componentes se crean a partir de la cantidad de plazas y transiciones de la RDPG_o, que es equivalene a las dimensiones (filas y columnas) de
 * la matriz de incidencia I.
 * Inicia la creacion de la matriz de incidencia I. Luego de su creacion se crean todas las matrices y vectores que pueden crearce conociendo el numero de 
 * plazas y transiciones de la RDPG.
 * 
 * @note La creacion de las matrices y vectores se puede realizar una sola vez en una misma RDPG_o, y esto es cuando el objeto RDPG_o no conoce su estructura 
 * de red es decir cuando I.mc==OBJ_NO_CREATED.
 * mc: marca de creacion, es una variable que indica si un objeto esta creadado o no.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el cual se crean todos sus componentes.
 * @param      p_entrada  Putero a la direccion de memoria de la cadena de caracteres que contiene la cantdad de plazas y transiciones en caracteres.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void create_rdpg(RDPG_o *p_rdp, char * p_entrada)
{
  int capt_f, capt_c; /* captura de filas y columnas*/

  /* Tomar valores de filas y columnas segun la entrada */
  if(extraer_enterox2(&capt_f, &capt_c, p_entrada) == ERROR_EXTRAER_FC)
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo crear RDPG. Error de datos en comando ingresado. \n");
  }
  else
  {
    if((capt_f<=0) || (capt_c<=0))
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo crear RDPG, se ingreso numeros negativos o nulos. \n");
      return;
    }
    else if((capt_f > MAX_PLACES) || (capt_c > MAX_TRANSITIONS))
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo crear RDPG, la dimension maxima soportada por driver es de 1000. \n");
      return;
    }
    else if(p_rdp->mII.mc==OBJ_NO_CREATED) /* Si matriz I no fue creada anteriormente. Se crea.*/
    {
      /* Creamos matriz I segun entrada recibida */
      create_rdpg_mcomponent(&(p_rdp->mII), capt_f, capt_c); /* 0: hace referencia a mc[0] para detectar que se creo
                                      una matriz en referencia a esa posicion, en este caso I */
      
      if(p_rdp->mII.mc) // Si se creo exitosamente Matriz I
      { 
        int ff, cc, ffi, cci;

        //p_rdp->cd = capt_c; // Se asigna a cd la cantidad de columnas, indica la cantidad de disparos posibles en la red.
        create_rdpg_mdisparos(&(p_rdp->mD), capt_c); /* creo matriz identidad, para obtener todos los disparos posibles segun la cantidad de transiciones (columnas)*/
        ff = 1;
        cc = p_rdp->mII.filas;
        ffi = p_rdp->mII.filas;
        cci = p_rdp->mII.columnas;

        /* Creacion de matrices de incidencia H y R. */
        create_rdpg_mcomponent(&(p_rdp->mIH), ffi, cci);

        create_rdpg_mcomponent(&(p_rdp->mIR), ffi, cci);

        create_rdpg_mcomponent(&(p_rdp->mIRe), ffi, cci);


        /* Crea vector de marcado inicial VMI, por defecto en cero sus valores hasta iniciar vMI. */
        create_rdpg_vcomponent(&(p_rdp->vMI), cc);

        /* Crea vector de marcado actual MA, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vMA), cc);

        /* Creamos vector de marcado nuevo MN, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vMN), cc);


        /* Creamos vector E de transisiones sensibilizadas, por defecto en cero hasta conocer vMI */
        cc = p_rdp->mII.columnas; /* Se asigna cantidad de columnas cc a la cantidad de columnas de I (equivalente a la cantidad de transiciones)*/

        create_rdpg_vcomponent(&(p_rdp->vE), cc);

        /* Creamos vector L, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vL), cc);

        /* Creamos vector B, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vB), cc);

        /* Creo vector G. */
        create_rdpg_vcomponent(&(p_rdp->vG), cc);
        p_rdp->vG.vo_methods->LoadLiteral_vector(&(p_rdp->vG), 1); /* Inicializo todos los valores de vG a uno por defecto.*/

        /* Creamos vector UDT, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vUDT), cc);

        /* Creamos vector Ex, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vEx), cc);

        /* Creamos vector A, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vA), cc);

        /* Creamos vector vHQCV, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vHQCV), cc);

        /* Creamos vector vHD, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vHD), cc);

        /* Creamos vector Q, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vQ), ffi);

        /* Creamos vector W, por defecto en cero sus valores hasta crear vMI */
        create_rdpg_vcomponent(&(p_rdp->vW), ffi);

        p_rdp->mc = OBJ_CREATED; /* Marco creacion de objeto RDPG garantizando creacion del resto de componentes. */
        count_global_RDPG_o++;
        p_rdp->obj_id = count_global_RDPG_o;
        //p_rdp->posVP = 0;
        //p_rdp->posVT = 0;
        config_default_mode(p_rdp);
        itoa(p_rdp->mII.filas, p_rdp->s_plazas, N_BYTES);
        itoa(p_rdp->mII.columnas, p_rdp->s_transiciones, N_BYTES);
        index_components(p_rdp);
        calculate_sizes(p_rdp);
        getString_allocMode(p_rdp);
        define_headers(p_rdp);
       }
    }
    else
    {
      p_rdp->error_code = -EC_dobleCreacionDeRDPG;
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se puede iniciar la creacion de la RDPG. Ya existe una RDPG creada en el driver.\n");
    }
  }  
}


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief  Esta funcion crea un componente matriz asociado a una RDPG. Un componente de una rdpg puede ser una matriz o un vector, esta funcion gestiona
 * la creacion de una matriz matrix_o. La funcion verifica que los parametros de filas y columnas, tomados desde una entrada del usuario, sean correctos.
 * (entrada: es una cadena de caracter con los datos ingresado por el usuario).
 * 
 * @param   p_filas     Numero entero que indica la cantidad de filas.
 * @param   p_columnas    Numero entero que indica la cantidad de columnas.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void create_rdpg_mcomponent(matrix_o *p_mo, int p_filas, int p_columnas)
{
    /* Verificamos filas y columnas */
    if(p_filas < 1 || p_columnas < 1)
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Filas o columnas mal ingresada.\n");
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo crear matriz con nombre : %s.\n", p_mo->name);
    }
    else if (p_mo->mc != OBJ_CREATED) /* Si matriz no existe. */
    {
    /* Creo la matriz de nuestro objeto con inicio de todos los valores a cero. */
    if(p_mo->mo_methods->create_zero_matrix(p_mo, p_filas, p_columnas) == CREATE_OK)
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s: creada exitosamente!!!\n", p_mo->name);
          if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Memoria reservada en %s: [%d] bytes reservados!!!\n", p_mo->name, p_mo->tam_bytes);
          
          /* Reporte de memoria real ocupada en el sistema por asignaciones de familia kmalloc. */
          if( (p_mo->alloc_mode != M_VMALLOC) && (p_mo->alloc_mode != M_VZALLOC) )
            if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Memoria real ocupada en sistema por %s: [%d] bytes !!!\n", p_mo->name, p_mo->realTam_bytes);
    }
        else
        {
          if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: Fallo de creacion de componente %s !!!\n", p_mo->name);
        }
    }
    else if (p_mo->mc == 1){ /* Si matriz existe */
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El componente %s ya se encuentra creado!!!\n", p_mo->name);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief  Esta funcion crea un componente vector asociado a una RDPG. Un componente de una rdp puede ser una matriz o un vector, esta funcion gestiona
 * la creacion de un vector vector_o. La funcion verifica que el parametro elements tomado desde una entrada de usuario, sea correcto.
 * (entrada: es una cadena de caracteres con el dato ingresado por el usuario).
 * 
 * @param   p_elements    Numero entero que indica la cantidad de elementos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void create_rdpg_vcomponent(vector_o *p_vo, int p_elements)
{
  /* Verificamos valor de elements */
    if(p_elements < 1)
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Numero de elementos incorrecto.\n");
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo crear vector : %s.\n", p_vo->name);
    }
    else if (p_vo->mc != OBJ_CREATED) /* Si vector no existe. */
    {
    /* Creo el vector de nuestro objeto con inicio de todos los valores a cero. */
    if(p_vo->vo_methods->create_zero_vector(p_vo, p_elements) == CREATE_OK)
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s: creado exitosamente!!!\n", p_vo->name);
          if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Memoria reservada en %s: [%d] bytes reservados!!!\n", p_vo->name, p_vo->tam_bytes);
          
          /* Reporte de memoria real ocupada en el sistema por asignaciones de familia kmalloc. */
          if( (p_vo->alloc_mode != M_VMALLOC) && (p_vo->alloc_mode != M_VZALLOC) )
            if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Memoria real ocupada en sistema por %s: [%d] bytes !!!\n", p_vo->name, p_vo->realTam_bytes);
    }
        else
        {
          if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: Fallo de creacion de componente %s !!!\n", p_vo->name);
        }
    }
  else if (p_vo->mc == OBJ_CREATED){ /* Si vector existe */
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El componente %s ya se encuentra creado!!!\n", p_vo->name);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion crea una matriz de disparos asociados a la matriz de incidencia I, donde las columnas indica la cantidad de 
 * disparos a crear. Tanto las filas de la matriz como sus columnas representan cada uno de los disparos posibles en una RDPG_o.
 * En resumen la matriz creada se corresponde con la matriz identidad de dimension p_columnas.
 * 
 * @param      p_mo        Puntero a la direccion de memoria del objeto matrix_o donde se crea la matriz.
 * @param[in]  p_columnas  Numero entero que indica la dimension de la matriz disparo a crear.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void create_rdpg_mdisparos(matrix_o *p_mo, int p_columnas)
{
  if (p_mo->mc != 1) /* Si matriz no existe. */
  {
    /* Creo la matriz identidad. Cada fila o columna representa cada uno de los disparo de las transiciones de la red. */
    if(p_mo->mo_methods->create_identity_matrix(p_mo, p_columnas) == CREATE_OK)
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s creado exitosamente!!!\n", p_mo->name);
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s reservo [%d] bytes de memoria!!!\n", p_mo->name, p_mo->tam_bytes);
    }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: Fallo de creacion de componente %s !!!\n", p_mo->name);
      }
  }
  else if (p_mo->mc == 1){ /* Si matriz existe */
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: %s: se encuentra creada!!!\n", p_mo->name);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de referenciar cada uno de los componentes matrix_o y vector_o de la RDPG_o. Las referencias de los 
 * componentes se realizan sobre vectores de punteros creados en RDPG_o para cada tipo de objeto que puede ser un componente.
 * Los vectores de punteros en la RDPG_o son mcomponent[N_MCOMPONENTS] y vcomponent[N_VCOMPONENTS].
 * 
 * @param      p_rdp  Puntero a la RDPG_o sobre la que se desea referenciar sus componentes en una sola tabla por tipo de componentes.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void index_components(RDPG_o *p_rdp)
{
  /* Indice a matrices en vector de punteros mcomponent */
  p_rdp->mcomponent[p_rdp->mII.obj_id] = &(p_rdp->mII);
  p_rdp->mcomponent[p_rdp->mIH.obj_id] = &(p_rdp->mIH);
  p_rdp->mcomponent[p_rdp->mIR.obj_id] = &(p_rdp->mIR);
  p_rdp->mcomponent[p_rdp->mIRe.obj_id] = &(p_rdp->mIRe);
  p_rdp->mcomponent[p_rdp->mD.obj_id] = &(p_rdp->mD);

  /* Indice a vectores en vector de punteros vcomponent */
  p_rdp->vcomponent[p_rdp->vMI.obj_id] = &(p_rdp->vMI);
  p_rdp->vcomponent[p_rdp->vMA.obj_id] = &(p_rdp->vMA);
  p_rdp->vcomponent[p_rdp->vMN.obj_id] = &(p_rdp->vMN);
  p_rdp->vcomponent[p_rdp->vE.obj_id] = &(p_rdp->vE);
  p_rdp->vcomponent[p_rdp->vQ.obj_id] = &(p_rdp->vQ);
  p_rdp->vcomponent[p_rdp->vW.obj_id] = &(p_rdp->vW);
  p_rdp->vcomponent[p_rdp->vL.obj_id] = &(p_rdp->vL);
  p_rdp->vcomponent[p_rdp->vB.obj_id] = &(p_rdp->vB);
  p_rdp->vcomponent[p_rdp->vG.obj_id] = &(p_rdp->vG);
  p_rdp->vcomponent[p_rdp->vA.obj_id] = &(p_rdp->vA);
  p_rdp->vcomponent[p_rdp->vUDT.obj_id] = &(p_rdp->vUDT);
  p_rdp->vcomponent[p_rdp->vEx.obj_id] = &(p_rdp->vEx);
  p_rdp->vcomponent[p_rdp->vHQCV.obj_id] = &(p_rdp->vHQCV);
  p_rdp->vcomponent[p_rdp->vHD.obj_id] = &(p_rdp->vHD);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion agrega un valor sobre un componente matrix_o en las posiciones indicadas por parametro. Las posiciones y el valor se envian 
 * desde una cadena de caracteres.
 *
 * @param      *p_m       Puntero a la direccion de memoria del objeto matrix_o sobre el cual agregar un valor.
 * @param      p_entrada  Puntero a la cadena de caracteres que contiene la informacion de los datos de posiciones (fila y columna) y el valor a agregar.
 *
 * @return    -EC_falloADD: si no se pudo agregar el valor correctamente en la matriz indicada por parmetro.
 * @return    valor agregado: si se pudo agragar el valor adecuadamente en la matriz y posiciones indicadas por parametro.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int add_value_in_mcomponent(matrix_o *p_m, char *p_entrada)
{
  int pos_f=0, pos_c=0, valor=0;
  
  /* Si existe matriz*/
  if(p_m->mc == 1){ /* Verifico si existe matriz a traves de marca de creacion de matriz.*/

    /* Extraccion de posicion de fila, posicion de columna y valor desde cadena a variables enteras.*/
    if(extraer_fcv(&pos_f, &pos_c, &valor, p_entrada)==EXTRACCION_OK) /* Si extraccion es exitosa. Agrego valor en la posicion indicada. */
    {
      if((pos_f < p_m->filas) && (pos_c < p_m->columnas)) /* Validacion de direcciones correctas en matriz. */
      {
        p_m->matriz[pos_f][pos_c] = valor;
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se actualizo valor de %s con matriz[%d][%d] = %d. \n", p_m->name, pos_f, pos_c, valor);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de fila/columna fuera de limites (direccion incorrecta). \n", p_m->name);
        return -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para matriz %s. \n", p_m->name);
      return -EC_falloADD;
    }

    return valor;
  }
  else 
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. La matriz %s no fue creada. \n", p_m->name);
    return -EC_falloADD;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion agrega un valor sobre un componente vector_o en la posicion indicada por parametro. La posicion y el valor se envian 
 * desde una cadena de caracteres.
 *
 * @param      *p_vo       Puntero a la direccion de memoria del objeto vector_o sobre el cual agregar un valor.
 * @param      p_entrada  Puntero a la cadena de caracteres que contiene la informacion de los datos de posicion y valor a agregar.
 *
 * @return    -EC_falloADD: si no se pudo agregar el valor correctamente en el vector indicado por parmetro.
 * @return    valor agregado: si se pudo agragar el valor adecuadamente en el vector y la posicion indicadas por parametro.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int add_value_in_vcomponent(vector_o *p_vo, char *p_entrada)
{
  int pos_e=0, valor=0;
  
  /* Si existe vector_o*/
  if(p_vo->mc == OBJ_CREATED){ /* Verifico si existe vector a traves de marca de creacion de vector_o.*/

    /* Extraccion de posicion de elemento y valor desde cadena de caracter a variables enteras.*/
    if(extraer_enterox2(&pos_e, &valor, p_entrada)==EXTRACCION_OK) /* Si extraccion es exitosa. Agrego valor en la posicion indicada. */
    {
      if((pos_e >= 0) && (pos_e < p_vo->elements)) /* Validacion de direccion correcta en vector. */
      {
        p_vo->vector[pos_e] = valor;
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se actualizo valor de %s con vector[%d] = %d. \n", p_vo->name, pos_e, valor);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de elemento fuera de limite (direccion incorrecta). \n", p_vo->name);
        return -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para vector %s. \n", p_vo->name);
      return -EC_falloADD;
    }

    return valor;
  }
  else 
  {
     if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. El vector %s no fue creado. \n", p_vo->name);
    return -EC_falloADD;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion agrega un valor sobre el componente matrix_o mIRe en las posiciones indicadas por parametro, para la RDPG_o indicada. La funcion 
 * tambien asigna el valor -1 implicitamente sobre la matriz mII, ya que esto es necesario para el correcto funcionamiento del arco reset de una RDPG. 
 * Las posiciones y el valor se envian desde una cadena de caracteres.
 *
 * @param      *p_m       Puntero a la direccion de memoria del objeto RDPG_o sobre el cual agregar un nuevo valor de su matriz mIRe.
 * @param      p_entrada  Puntero a la cadena de caracteres que contiene la informacion de los datos de posiciones (fila y columna) y el valor a agregar.
 *
 * @return    -EC_falloADD: si no se pudo agregar el valor correctamente en la matriz indicada por parmetro.
 * @return    valor agregado: si se pudo agragar el valor adecuadamente en la matriz y posiciones indicadas por parametro.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int add_value_in_mIRe(RDPG_o *p_rdp, char *p_entrada)
{ 
  int pos_f=0, pos_c=0, valor=0;
  
  /* Si existe matriz*/
  if((p_rdp->mIRe.mc == OBJ_CREATED) && (p_rdp->mII.mc == OBJ_CREATED)){ /* Verifico si existen matrices a traves de marca de creacion.*/

    /* Extraccion de posicion de fila, posicion de columna y valor desde cadena a variables enteras.*/
    if(extraer_fcv(&pos_f, &pos_c, &valor, p_entrada)==EXTRACCION_OK) /* Si extraccion es exitosa. Agrego valor en la posicion indicada. */
    {
      if((pos_f < p_rdp->mII.filas) && (pos_c < p_rdp->mII.columnas)) /* Validacion de direcciones correctas en matriz. */
      {
        p_rdp->mIRe.matriz[pos_f][pos_c] = valor;
        p_rdp->mII.matriz[pos_f][pos_c] = -1;      /* Se configura implicitamente un arco entre la plaza y transicion asociada. Con este arco el brazo reset funciona correctamente.*/
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se actualizo valor de %s con matriz[%d][%d] = %d. \n", p_rdp->mIRe.name, pos_f, pos_c, valor);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de fila/columna fuera de limites (direccion incorrecta). \n", p_rdp->mIRe.name);
        return -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para matriz %s. \n", p_rdp->mIRe.name);
      return -EC_falloADD;
    }

    return valor;
  }
  else 
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. La matriz %s no fue creada. \n", p_rdp->mIRe.name);
    return -EC_falloADD;
  }
}


int add_value_vG(RDPG_o *p_rdp, char *p_entrada)
{
  int pos_e=0, valor=0;

  /* Si existe vector_o*/
  if(p_rdp->vG.mc == OBJ_CREATED){ /* Verifico si existe vector a traves de marca de creacion de vector_o.*/

    /* Extraccion de posicion de elemento y valor desde cadena de caracter a variables enteras.*/
    if(extraer_enterox2(&pos_e, &valor, p_entrada)==EXTRACCION_OK) /* Si extraccion es exitosa. Agrego valor en la posicion indicada. */
    {
      if((pos_e >= 0) && (pos_e < p_rdp->vG.elements)) /* Validacion de direccion correcta en vector. */
      {
        p_rdp->vG.vector[pos_e] = valor;
        update_vEx(p_rdp);                /* Se actualiza vector Ex para considerar cambio de vG. */
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se actualizo valor de %s con vector[%d] = %d. \n", p_rdp->vG.name, pos_e, valor);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de elemento fuera de limite (direccion incorrecta). \n", p_rdp->vG.name);
        return -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para vector %s. \n", p_rdp->vG.name);
      return -EC_falloADD;
    }

    return valor;
  }
  else 
  {
     if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. El componente %s no fue creado. \n", p_rdp->vG.name);
    return -EC_falloADD;
  }

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief  Esta funcion es el destructor de una RDPG_o, se verifica la existencia de los componentes (matrices y vectores) de una RDP, y los elimina si 
 * estan creados.
 * Es decir que la funcion se encarga de verificar la existencia de las matrices y vectores asociados a una RDP, y eliminar las matrices y vectores que
 * hayan sido creados por el driver. Despues de ejecutar esta funcion es posible cargar una nueva RDPG con nueva estructura.
 * 
 * @param   p_rdp Puntero a la direccion de memoria del objeto RDPG_o sobre el cual se desea realizar la destruccion.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void delete_rdpg(RDPG_o *p_rdp)
{
  /*if(p_rdp->mc == OBJ_CREATED) // Esta condicion, provoca memory leaks si se crean solo componentes individuales. Conviene ignorarla, ya que al liberar mem se verifica direccion NULL (direccion protegida).*/
      
  /* Eliminacion de componentes matrices. */
  delete_rdpg_mcomponent(&(p_rdp->mII));
  delete_rdpg_mcomponent(&(p_rdp->mIH));
  delete_rdpg_mcomponent(&(p_rdp->mIR));
  delete_rdpg_mcomponent(&(p_rdp->mIRe));
  delete_rdpg_mcomponent(&(p_rdp->mD));

  /* Eliminacion de componentes vectores. */
  delete_rdpg_vcomponent(&(p_rdp->vMI));
  delete_rdpg_vcomponent(&(p_rdp->vMA));
  delete_rdpg_vcomponent(&(p_rdp->vMN));
  delete_rdpg_vcomponent(&(p_rdp->vE));
  delete_rdpg_vcomponent(&(p_rdp->vQ));
  delete_rdpg_vcomponent(&(p_rdp->vW));
  delete_rdpg_vcomponent(&(p_rdp->vL));
  delete_rdpg_vcomponent(&(p_rdp->vB));
  delete_rdpg_vcomponent(&(p_rdp->vG));
  delete_rdpg_vcomponent(&(p_rdp->vA));
  delete_rdpg_vcomponent(&(p_rdp->vUDT));
  delete_rdpg_vcomponent(&(p_rdp->vEx));
  delete_rdpg_vcomponent(&(p_rdp->vHQCV));
  delete_rdpg_vcomponent(&(p_rdp->vHD));

  p_rdp->obj_id = 0;
  p_rdp->mc = OBJ_NO_CREATED;
  count_global_RDPG_o--; /* Se decrementa la cantidad de objetos RDPG_o creados por la libreria. */
  
  if(count_global_RDPG_o < 0)
  {
    p_rdp->error_code = - EC_dobleEliminacionDeRDPG;
    count_global_RDPG_o = 0;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de eliminar un objeto matrix_o liberando la memoria previamente asignada.
 *
 * @param      p_mo  Puntero a la direccion de memoria del objeto matrix_o que se desea eliminar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void delete_rdpg_mcomponent(matrix_o *p_mo)
{
  int aux_tam_bytes = p_mo->tam_bytes; /* Guardo memoria utilizada por p_mo antes de liberarla. */

  /* Liberamos memoria de p_mo*/
  if(p_mo->mo_methods->free_matrix_mem(p_mo) == FREE_OK)
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se libero [%d] bytes de componente %s.\n", aux_tam_bytes, p_mo->name);
  else if (p_mo->matriz != NULL)
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se pudo liberar memoria de componente %s. Fallo funcion de liberar memoria.\n", p_mo->name);
  else
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: El componente %s no reservo memoria o ya fue liberado.\n", p_mo->name);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de eliminar un objeto vector_o liberando la memoria previamente asignada.
 *
 * @param      p_vo  Puntero a la direccion de memoria del objeto vector_o que se desea eliminar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void delete_rdpg_vcomponent(vector_o *p_vo)
{
  int aux_tam_bytes = p_vo->tam_bytes; /* Guardo memoria utilizada por p_vo antes de liberarla. */

  /* Liberamos memoria de objeto p_vo*/
  if(p_vo->vo_methods->free_vector_mem(p_vo) == FREE_OK)
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se libero [%d] bytes de componente %s.\n", aux_tam_bytes, p_vo->name);
    else if (p_vo->vector != NULL)
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se pudo liberar memoria de componente %s. Fallo funcion de liberar memoria.\n", p_vo->name);
    else
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: El componente %s no reservo memoria o ya fue liberada.\n", p_vo->name);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Setea el modo de asignacion de memoria para todos los componentes de un objeto RDPG_o.
 *
 * @param      p_rdp   Puntero a la direccion de memoria que referencia al objeto RDPG_o sobre el cual se realiza el seteo del modo de asignacion de memoria.
 * @param      c_mode  Puntero a la direccion de memoria de la cadena de caracteres que contiene el numero entero a extraer, dicho entero extraido es el
 *         nemero identificador del modo a configurar, debera coincidir con alguno de los modos definidos en la enum MEM_ALLOC_MODE de MV_obect.h.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_MemAllocMode(RDPG_o *p_rdp, char *c_mode)
{
  int mode;
  int aux_mode; /* variable que almacena el valor entero. */
  
  
  if( extraer_entero(c_mode, &aux_mode) == ERROR_EXTRAER_ENTERO)
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se puede configurar el modo de reserva de memoria, falla en extraccion de entero.\n");
  }
  else /* Seteo de modo de asignacion de memoria en todos los componentes de la RDPG_o. */
  {
    mode = aux_mode;

    /* Configuracion de modo de reserva en todas las matrices de RDPG_o */
    p_rdp->mII.mo_methods->set_mem_alloc_mode(&(p_rdp->mII), mode);
    p_rdp->mIH.mo_methods->set_mem_alloc_mode(&(p_rdp->mIH), mode);
    p_rdp->mIR.mo_methods->set_mem_alloc_mode(&(p_rdp->mIR), mode);
    p_rdp->mIRe.mo_methods->set_mem_alloc_mode(&(p_rdp->mIRe), mode);
    p_rdp->mD.mo_methods->set_mem_alloc_mode(&(p_rdp->mD), mode);

    /* Configuracion de modo de reserva en todos los vectores de RDPG_o */
    p_rdp->vMI.vo_methods->set_mem_alloc_mode(&(p_rdp->vMI), mode);
    p_rdp->vMA.vo_methods->set_mem_alloc_mode(&(p_rdp->vMA), mode);
    p_rdp->vMN.vo_methods->set_mem_alloc_mode(&(p_rdp->vMN), mode);
    p_rdp->vE.vo_methods->set_mem_alloc_mode(&(p_rdp->vE), mode);
    p_rdp->vQ.vo_methods->set_mem_alloc_mode(&(p_rdp->vQ), mode);
    p_rdp->vW.vo_methods->set_mem_alloc_mode(&(p_rdp->vW), mode);
    p_rdp->vL.vo_methods->set_mem_alloc_mode(&(p_rdp->vL), mode);
    p_rdp->vB.vo_methods->set_mem_alloc_mode(&(p_rdp->vB), mode);
    p_rdp->vG.vo_methods->set_mem_alloc_mode(&(p_rdp->vG), mode);
    p_rdp->vA.vo_methods->set_mem_alloc_mode(&(p_rdp->vA), mode);
    p_rdp->vUDT.vo_methods->set_mem_alloc_mode(&(p_rdp->vUDT), mode);
    p_rdp->vEx.vo_methods->set_mem_alloc_mode(&(p_rdp->vEx), mode);
    p_rdp->vHQCV.vo_methods->set_mem_alloc_mode(&(p_rdp->vHQCV), mode);
    p_rdp->vHD.vo_methods->set_mem_alloc_mode(&(p_rdp->vHD), mode);

    getString_allocMode(p_rdp);

    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se configuro < %s > como modo de reserva de memoria para la gestion de RDPG en el kernel.\n", p_rdp->s_allocMode);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion se encarga de calcular el tamaño de bytes reservados por una RDPG_o luego de la asignacion de memoria de cada
 * uno de sus componentes. Se determinan dos tamaños de bytes:
 *  - El primero es la cantidad de memoria reservada por todos los componentes segun lo que solicito un usuario.
 *  - El segundo es la cantidad de memoria reservada real en el sistema para satisfacer la solicitud del usuario.
 *    Por la memoria real ocupada en el sistema es mayor que la solicitada por un usuario.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que realiza el calculo de los tamaños en bytes.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void calculate_sizes(RDPG_o *p_rdp)
{
  size_t aux_size=0;
  size_t aux_realSize=0;
  size_t i, j;

  /* Calculo de bytes almacenados por las matrices de la RDP. */
  for(i=0; i< ID_MC_END; i++)
  {
    aux_size += p_rdp->mcomponent[i]->tam_bytes;
    aux_realSize += p_rdp->mcomponent[i]->realTam_bytes;
  }

  /* Calculo de bytes almacenados por los vectores de la RDP. */
  for(j=0; j< ID_VC_END; j++)
  {
    aux_size += p_rdp->vcomponent[j]->tam_bytes;
    aux_realSize += p_rdp->vcomponent[j]->realTam_bytes;
  }

  itoa(aux_size, p_rdp->s_size, N_BYTES);
  itoa(aux_realSize, p_rdp->s_realSize, N_BYTES);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * @brief      Esta funcion define las cabeceras de las plazas, transiciones y de la cantidad de disparos de un RDPG_o en cadenas de 
 * caracteres que tiene como atributo el objeto RDPG_o.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o en el cual se definen las cabeceras.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void define_headers(RDPG_o *p_rdp)
{
  size_t i, vp, vt;
  char c_aux[N_BYTES];

  if(p_rdp->mc == OBJ_CREATED)
  {
    memset(p_rdp->s_header_transiciones, '\0', N_HEADER);
    memset(p_rdp->s_header_disparos, '\0', N_HEADER);
    memset(p_rdp->s_header_plazas, '\0', N_HEADER);
    
    /* Definicion de la cabecera de transiciones y numero de disparos. */
    for(i=p_rdp->posVT, vt=0; (i < p_rdp->mII.columnas) && (vt < p_rdp->vdim); i++, vt++)
    {
      itoa(i, c_aux, N_BYTES);
      if(vt==0)
      {
        (void)concat_x3(p_rdp->s_header_transiciones, s_space2, "T", c_aux);
        (void)concat_x3(p_rdp->s_header_disparos, s_space2, "D", c_aux);
      }
      else
      {
        (void)concat_x3(p_rdp->s_header_transiciones, s_space1, "T", c_aux);
        (void)concat_x3(p_rdp->s_header_disparos, s_space1, "D", c_aux);
      }
    }

    /* Definicion de cabecera de plazas. */
    for(i=p_rdp->posVP, vp=0; (i < p_rdp->mII.filas) && (vp < p_rdp->vdim); i++, vp++)
    {
      itoa(i, c_aux, N_BYTES);
      if(vp==0)
      {
        (void)concat_x3(p_rdp->s_header_plazas, s_space2, "P", c_aux);
      }
      else
      {
        (void)concat_x3(p_rdp->s_header_plazas, s_space1, "P", c_aux);
      }
    }
  }
  else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Error en la definicion de cabeceras de componentes, la RDPG no esta creada!\n");
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion realiza el disparo de una transicion sobre una RDPG si la entrada de la cadena (p_entrada) respeta el formato
 * de ingreso (reconocido por la libreria RDPG_object.h).
 * 
 * @note: El nombre de la funcion contiene _s al final ya que la entrada del numero de transicion se realiza por medio de una
 * cadena de caracter (string). Esta cadena se debe validar si respeta el formato reconocido por la la libreria RDPG_object.h.
 *
 * @param      p_rdp      Puntero de la direccion de memoria del objeto RDPG_o sobre el que se desea realizar el disparo de la transicion enviada por 
 *              parametro.
 * @param      p_entrada  Puntero de la direccion de memoria de la cadena de caracter que contiene el numero de transicion a disparar en formato char.
 * @param[in]  p_mode   Indica el modo de disparo de acuerdo a la enum SHOT_MODE.
 *                    - SHOT_MODE_I: modo implicito, no actualiza el vector de marcado actual vMA.
 *                    - SHOT_MODE_E: modo explicito, actualiza el vector de marcado actual vMA.
 *
 * @return El retorno es:
 *      SHOT_FAILED: si no se puede realizar disparo sobre la RDPG_o enviada por parametro (la transicion enviada no esta sensibilizada para disparar en la RDPG).
 *      SHOT_OK: si puedo realizar el disparo de la transicion enviada sobre RDPG (la transicion enviada estaba sensibilizada para ese disparo en la RDPG).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int disparar_rdpg_s(RDPG_o *p_rdp, char *p_entrada, SHOT_MODE p_mode)
{
  int transicion; /* variable que almacena el valor entero del numero de transicion a disparar. */

  if( extraer_entero(p_entrada, &transicion) == ERROR_EXTRAER_ENTERO)
  {
    p_rdp->shot_result = (int)SHOT_FAILED;
    return SHOT_FAILED;
  }
  else /* Disparar transicion */
  {
    return p_rdp->methods->shoot_rdpg(p_rdp, transicion, SHOT_MODE_E);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de realizar el disparo de una transicion (p_idT) sobre una RDPG enviada por parametro. La funcion analiza si el
 * numero de la transicion enviada por parametro p_idT existe en la RDPG, en caso se existir se valida si la transicion esta sensibilizada y en caso
 * de estar sensibilizada se dispara la transicion indicada. Para cualquier otro caso se detalla el inconveniente de la falla del disparo de la transicion.
 * 
 * @param      p_rdp      Puntero de la direccion de memoria del objeto RDPG_o sobre el que se desea realizar el disparo de la transicion enviada por 
 *              parametro.
 * @param[in]  p_idT      Entero que reresenta el numero de transicion a disparar sobre la RDPG enviada por parametro.
 * @param[in]  p_mode   Indica el modo de disparo de acuerdo a la enum SHOT_MODE.
 *                    - SHOT_MODE_I: modo implicito, no actualiza el vector de marcado actual vMA.
 *                    - SHOT_MODE_E: modo explicito, actualiza el vector de marcado actual vMA.
 *
 * @return El retorno es:
 *      SHOT_FAILED: si no se puede realizar disparo sobre la RDPG_o enviada por parametro (la transicion enviada no esta sensibilizada para disparar en la RDPG).
 *      SHOT_OK: si puedo realizar el disparo de la transicion enviada sobre RDPG (la transicion enviada estaba sensibilizada para ese disparo en la RDPG).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int disparar_rdpg(RDPG_o *p_rdp, int p_idT, SHOT_MODE p_mode)
{  
  /* Si existen objetos RDPG afectado. Ejecuto operacion. La creacion de la RDPG garatiza la creacion del resto de objetos afectados. */
  if(p_rdp->mc==OBJ_CREATED)
  {
    if((p_idT > -1) && (p_idT < p_rdp->mII.columnas))  /* Existe transicion indicada ? Disparar p_idT */
    {
      if(p_rdp->vEx.vector[p_idT] == ST_SENSITIZED)    /* Transicion sensibilizada ? */
      {
        int j, auxMN;

        if(p_rdp->mD.matriz[p_idT][p_idT]==1)         /* Verificacion de disparo valido. */
        {
          for (j = 0; j < p_rdp->mII.filas; j++)      /* Actualizacion de marcas de acuerdo a disparo de transicion p_idT. */
          {
            if(p_mode == SHOT_MODE_E) // modo de disparo explicito: se actualiza el nuevo estado en el vector MN, si el disparo es exitoso se actualiza MA posteriormente.
            {
                //p_rdp->vMN.vector[j] = p_rdp->vMA.vector[j] + p_rdp->mII.matriz[j][i]*p_rdp->mD.matriz[p_idT][i]*p_rdp->vEx.vector[i];
                p_rdp->vMN.vector[j] = p_rdp->vMA.vector[j] + ( (p_rdp->mII.matriz[j][p_idT]) * ( ((p_rdp->mD.matriz[p_idT][p_idT]) & (p_rdp->vEx.vector[p_idT])) * p_rdp->vA.vector[p_idT]) );
                if(p_rdp->vMN.vector[j]< 0)
                {
                  if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo disparo de transicion T%d!!!\n", p_idT);
                  p_rdp->shot_result = (int)SHOT_FAILED;
                  p_rdp->vUDT.vector[p_idT] = (int)SHOT_FAILED;
                  return SHOT_FAILED;
                }
            }
            else // modo de disparo implicito: no interesa almacenar el estado de MN pero si saberlo.
            {
                // Se almacena de manera auxiliar los valores del nuevo estado de la transicion, para verificar si esta sensibilizada
                //auxMN = p_rdp->vMA.vector[j] + p_rdp->mII.matriz[j][i]*p_rdp->mD.matriz[p_idT][i]*p_rdp->vEx.vector[i];
                auxMN = p_rdp->vMA.vector[j] + ( (p_rdp->mII.matriz[j][p_idT]) * ( ((p_rdp->mD.matriz[p_idT][p_idT]) & (p_rdp->vEx.vector[p_idT])) * p_rdp->vA.vector[p_idT]) );
                if(auxMN < 0)
                  return SHOT_FAILED;
            }
          }
        }

        /* Actualizacion de marcado actual y vectores*/
        if(p_mode == SHOT_MODE_E)
        {
          cargar_Dvcomp_con_Ovcomp(p_rdp, vMA, vMN);

          /* Se actualiza el resto de vectores de RDPG si el disparo es explicito. */
          p_rdp->methods->update_work_components(p_rdp);

          if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se disparo exitosamente la transicion T%d!!!\n", p_idT);

          p_rdp->shot_result = (int)SHOT_OK;
          p_rdp->vUDT.vector[p_idT] = (int)SHOT_OK;
        }

        return SHOT_OK;
      }
      else
      { 
        /* Se limpia vMN a cero ... */
        p_rdp->vMN.vo_methods->clean_vector(&(p_rdp->vMN));
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Transicion T%d no sensibilizada!!!\n", p_idT);
        p_rdp->shot_result = (int)SHOT_FAILED;
        p_rdp->vUDT.vector[p_idT] = (int)SHOT_FAILED;
        //SHOT_OK;
        return SHOT_FAILED; 
      }   
    }
    else
    {
      p_rdp->error_code = -EC_transicionInexistente;
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El disparo no fue exitoso. Se indico una transicion que no existe en la RDPG cargada en el kernel!!!\n");
      p_rdp->shot_result = (int)SHOT_FAILED;
      p_rdp->vUDT.vector[p_idT] = (int)SHOT_FAILED;
      return SHOT_FAILED;
    }
  }

  /* Si no existe RDPG afectada al disparo de transicion indicada se retorna fallo de disparo. */
  return SHOT_FAILED;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion realiza el disparo sobre la RDP de acuerdo a un vector de disparo indicado por parametro. Se verifica la sensibilidad de la
 * RDP para el disparo de la transicion y se actualiza o no el marcado actual de acuerdo a si el disparo es exitoso o no.
 * Se permite realizar el disparo de una transicion en dos modos, implicita o explicitamente indicado por parametro.
 * 
 * @param       p_rdp   Puntero de la direccion de memoria del objeto RDPG_o sobre el que se desea disparar una transicion.
 * @param[in]   id_d    Identificador de la transicion a disparar sobre RDP.
 * @param[in]   p_mode  Modo con el que se desea realizar el disparo sobre la RDPG_o. Los modos pueden ser:
        - SHOT_MODE_I: modo implicito, no actualiza el vector de marcado actual MA. Permite visualizar el estado de cualquiera de los disparos
        sin actualizar los vectores de marcado.
                - SHOT_MODE_E: modo explicito, actualiza el vector de marcado actual MA ante cualquier disparo a realizar.
 *
 * @return     El retorno es:
 *        - SHOT_FAILED: si no se puede realizar disparo sobre RdP (transicion no sensibilizada en la RDPG_o).
 *        - SHOT_OK: si puedo realizar disparo sobre RDP (transicion sensibilizada en la RDPG_o).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int disparar(RDPG_o *p_rdp, int id_d, SHOT_MODE p_mode)
{
  /* Disparar */
  int j, auxMN;

  /* Si existen objetos RDPG afectado. Ejecuto operacion. La creacion de la RDPG garatiza la creacion del resto de objetos afectados. */
  if(p_rdp->mc==OBJ_CREATED)
  {
    if( (id_d < (p_rdp->mII.columnas)) && (id_d >= 0) ) /* Si id_d es valido para RDP. */
    {
    /* Se valida la transicion a disparar en transiciones de RDP. */
    if(p_rdp->mD.matriz[id_d][id_d]==1)
    {
      /* Se actualizan todas las plazas afectados por el disparo de transicion, todo se registra en vector vMN de la RDP.*/
      for (j = 0; j < p_rdp->mII.filas; j++)
      {
        if(p_mode == SHOT_MODE_E)
        {
            p_rdp->vMN.vector[j] = p_rdp->vMA.vector[j] + p_rdp->mII.matriz[j][id_d]*p_rdp->mD.matriz[id_d][id_d];
            if(p_rdp->vMN.vector[j]< 0)
              return SHOT_FAILED;
        }
        else /* modo de disparo implicito no intera almacenar el estado de MN ni MA pero si saberlo */
        {
            /* Se almacena de manera auxiliar los valores del nuevo estado de la transicion, para verificar si esta sensibilizada */
            auxMN = p_rdp->vMA.vector[j] + p_rdp->mII.matriz[j][id_d]*p_rdp->mD.matriz[id_d][id_d];
            if(auxMN < 0)
              return SHOT_FAILED;
        }
      }
    }
  
    /* Si el disparo es en modo explicito se actualiza marcado actual. */
    if(p_mode == SHOT_MODE_E)
    {
      for (j = 0; j <p_rdp->mII.filas ; j++){
        p_rdp->vMA.vector[j] = p_rdp->vMN.vector[j];
      }
    }
      
      //p_rdp->shot_result = (int)SHOT_OK; /* No se tiene en cuenta resultado de disiparo ya que estos disparos son todos implicitos. */
      return SHOT_OK;
    }
  }
  
  /* Si id_d es un numero de transicion no valido se retorna sin ninguna modificacion.*/
  /* Si no existen objetos RDPG afectado a un disparo de red no se realiza ninguna operacion. */
  return SHOT_FAILED;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
* @brief Esta funcion se encarga de cargar el componente vector destino con ID p_vcomp_dst con los valores del componente vector origen con ID p_vcomp_src.
*
* @param      p_rdp        Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar la operacion.
* @param[in]  p_vcomp_dst  Es el ID del componente vector destino sobre el que se realiza la modificacion de sus valores.
* @param[in]  p_vcomp_src  Es el ID del componente vector origen con el cual se obtienen los valores a copiar en el componente vector destino.
*---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void cargar_Dvcomp_con_Ovcomp(RDPG_o *p_rdp, int p_vcomp_dst, int p_vcomp_src)
{
  int i;

  if((valid_vcomp_id(p_vcomp_dst) == VALID_ID) && (valid_vcomp_id(p_vcomp_src) == VALID_ID)){
    if((p_rdp->vcomponent[p_vcomp_dst]->mc == OBJ_CREATED) && (p_rdp->vcomponent[p_vcomp_src]->mc == OBJ_CREATED))  /* Si existen ambos componentes. */
    {
      /* for con detecion de fin de vector por posibles diferencias de longitudes entre vectores. */
      for (i = 0; (i < p_rdp->vcomponent[p_vcomp_dst]->elements) && (i < p_rdp->vcomponent[p_vcomp_src]->elements) ; i++)
      { /* code */
        p_rdp->vcomponent[p_vcomp_dst]->vector[i] = p_rdp->vcomponent[p_vcomp_src]->vector[i];
      }
    }
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vE, una vez que se completo la carga del vector vMI. La funcion dispara todas las transiciones 
 * en modo implicito para determinar que transiciones estan sensibilizadas y cuales no lo estan, de esta manera se obtiene el vector de transiciones 
 * sensibilizadas.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar la carga de su vector vE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vE(RDPG_o *p_rdp)
{
  int i;

  if(p_rdp->vE.mc) /* Si existe vector vE. Actualizo valores. */
  {
      for (i = 0; i < p_rdp->mII.columnas; i++)
      {
          if(disparar(p_rdp,i,SHOT_MODE_I) == SHOT_OK){
            p_rdp->vE.vector[i] = 1;
          }

          else{
            p_rdp->vE.vector[i] = 0;
          }
      }
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de componente %s con exito!!!\n", p_rdp->vE.name);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vQ, cuyos elementos qi son qi -> 0 si M(pi) != 0 o qi -> 1 en el resto de los casos estos 
 * valores se toman a partir del vector de marcado actual vMA.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cargar su componente vector vQ.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vQ(RDPG_o *p_rdp)
{
  int i;

  if((p_rdp->vQ.mc) && (p_rdp->vMA.mc)) /* Si existen vectores afectados. Actualizo valores de vQ. */
  {
      for (i = 0; i < p_rdp->vMA.elements; i++)
      {
          if(p_rdp->vMA.vector[i] != 0)
          {
            p_rdp->vQ.vector[i] = 0;
          }

          else
          {
            p_rdp->vQ.vector[i] = 1;
          }
      }
  }
  else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vQ.name);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vW, cuyos elementos wi son wi -> 1 si M(pi) != 0 o wi-> 0 en el resto de los casos. Los
 * valores se toman a partir del vector de marcado actual vMA.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cargar su componente vector vW.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vW(RDPG_o *p_rdp)
{
  int i;

  if((p_rdp->vW.mc) && (p_rdp->vMA.mc)) /* Si existen componentes afectados. Actualizo valores de vW. */
  {
      for (i = 0; i < p_rdp->vMA.elements; i++)
      {
          if(p_rdp->vMA.vector[i] != 0){
            p_rdp->vW.vector[i] = 1;
          }

          else
          {
            p_rdp->vW.vector[i] = 0;
          }
      }
  }
  else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vW.name);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vB, una vez que se completo la carga de vMI, mH y vQ.
 * El vector vB que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la RDP) que
 * indica con un cero cuales transiciones estan inhibidas por el arco inhibidor y con un uno las que no.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cargar su componente vector vB.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vB(RDPG_o *p_rdp)
{
    int i;
    int j;

   if((p_rdp->vB.mc) && (p_rdp->vQ.mc) && (p_rdp->mIH.mc)) /* Si existen componentes afectados. Actualizo valores vB. */
   {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector B*/
      for(i=0; i < p_rdp->mIH.columnas; i++){
        if(p_rdp->vB.vector[i]!=1)
        {
          p_rdp->vB.vector[i]=1;
        }
      }

      /* Calculo de valores inhibidos (cero binario) sobre vector B*/
      for(i=0; i< p_rdp->mIH.columnas; i++){  /* lectura de transiciones de matriz H*/
        for(j=0; j< p_rdp->mIH.filas; j++){   /* lectura de plazas de matriz H*/
            if(p_rdp->mIH.matriz[j][i] == 1)
            {
               p_rdp->vB.vector[i] = (p_rdp->mIH.matriz[j][i]) & (p_rdp->vQ.vector[j]);
               if(p_rdp->vB.vector[i] == 0)   /* Si se detecto inhibicion, salir! */
                break;
            }
        }
      }

      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de vector B con exito!!!\n");
  }
  else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vB.name);
  }

}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vL de una RDPG_o, una vez que se completo la carga de vMI, mR y vW. El vector
 * vL que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la
 * RDP) que indica con un cero cuales transiciones estan inhibidas por el arco lector y un uno las que no.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cargar su componente vector vL.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vL(RDPG_o *p_rdp)
{
    int i;
    int j;

    if((p_rdp->vL.mc) && (p_rdp->vW.mc) && (p_rdp->mIR.mc)) /* Si existen componentes afectados. Actualizo valores vL. */
    {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector L*/
      for(i=0; i < p_rdp->mIR.columnas; i++){
        if(p_rdp->vL.vector[i]!=1)
        {
          p_rdp->vL.vector[i]=1;
        }
      }

      /* Calculo de valores inhibidos (cero binario) sobre vector L*/
      for(i=0; i< p_rdp->mIR.columnas; i++){  /* lectura de transiciones de matriz R*/
        for(j=0; j< p_rdp->mIR.filas; j++){   /* lectura de plazas de matriz R*/
            if(p_rdp->mIR.matriz[j][i] == 1)
            {
              p_rdp->vL.vector[i] = p_rdp->mIR.matriz[j][i] & p_rdp->vW.vector[j];
              if(p_rdp->vL.vector[i] == 0)    /* Si se detecto inhibicion, salir! */
                break;
            }
        }
      }

      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de vector L con exito!!!\n");
    }
    else
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vL.name);
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de actualizar el vector vG de una RDPG_o enviada por parametro. Se inicializan todos los elementos del vector a uno.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea actualizar su componente vector vG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vG(RDPG_o *p_rdp)
{
    int i;
    int j;

    if(p_rdp->vG.mc) /* Si existen componentes afectados. Actualizo valores vG. */
    {
      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector G. */
      p_rdp->vG.vo_methods->LoadLiteral_vector(p_rdp->vcomponent[vG], 1);
      
      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de vector G exitosamente!!!\n");
    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vG.name);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de actualizar el vector vA de transiciones reset, de acuerdo con la teoria de las RDPG. Donde sus valores son todos
 *             uno salvo para las transiciones con arco reset entrante, cuyo valor es el numero indicado por la plaza que se conecta con la transicion (vMA[plaza]).
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea actualizar su componente vector vA.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vA(RDPG_o *p_rdp)
{
    int i;
    int j;

    if((p_rdp->vA.mc) && (p_rdp->mIRe.mc) && (p_rdp->vMA.mc)) /* Si existen componentes afectados. Actualizo valores vA. */
    {
      int v_marca[p_rdp->vA.elements];                /* Creo vector auxiliar para funcion de marca. */
      rellenar_vector(v_marca,p_rdp->vA.elements,0);  /* inicializo vector "marca" todo a cero. */

      /* Inicializacion de valores en sensibilizados (uno binario) sobre vector A. */
      p_rdp->vA.vo_methods->LoadLiteral_vector(p_rdp->vcomponent[vA], 1);

      /* Calculo de plazas con arco reset saliente.*/
      for(i=0; i< p_rdp->mIRe.columnas; i++)          /* lectura de transiciones de matriz Re*/
      {
        for(j=0; j< p_rdp->mIRe.filas; j++)           /* lectura de plazas de matriz Re*/
        {
            if(p_rdp->mIRe.matriz[j][i] == 1)         /* Si existe plaza con arco reset saliente.*/
            {
              v_marca[i] = p_rdp->vMA.vector[j];      /* Guardo valor en vector marca. */
            }
        }
      }

      /* Calculo de vector A. */
      for(i=0; i< p_rdp->mIRe.columnas; i++)  /* lectura de transiciones de matriz Re*/
      {
        if(v_marca[i] > 1)  /* Elemento de vector A es componente v_marca[i] si no es cero y mayor que 1. */
          p_rdp->vA.vector[i]=v_marca[i];
        
        /* Elemento de vector A es uno si v_marca[i] es cero. Esto se realiza en inicializacion de vA a uno. */  
      }

      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de vector A exitosamente!!!\n");
    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No es posible actualizar componente %s!!!\n", p_rdp->vA.name);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de cargar el vector vEx de una RDPG_o, una vez que se completo la carga de los vectores vE, vB y vL.
 * El vector vEx que carga la funcion, es un vector de valores binarios de dimension mx1 (siendo m la cantidad de transiciones existentes en la RDP)
 * que indica con un cero cuales transiciones estan inhibidas por la conjuncion de todos arcos de la RDP y con un uno las que no.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cargar su componente vector vEx.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void update_vEx(RDPG_o *p_rdp)
{
    int i;

    if((p_rdp->vEx.mc) && (p_rdp->vE.mc) && (p_rdp->vB.mc) && (p_rdp->vL.mc) && (p_rdp->vG.mc)) /* Si existen componentes afectados. Actualizo valores de vEx. */
    {
      /* Calculo de la conjuncion logica de todos los vectores para generar el vector Ex*/
      for(i=0; i < p_rdp->mII.columnas; i++)
      {
          p_rdp->vEx.vector[i] = (p_rdp->vE.vector[i] & p_rdp->vB.vector[i]) & (p_rdp->vL.vector[i] & p_rdp->vG.vector[i]);
      }

      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de %s con exito!!!\n", p_rdp->vEx.name);
    }
}


extern void update_vHD(RDPG_o *p_rdp)
{
    int i;

    if((p_rdp->vEx.mc) && (p_rdp->vHD.mc) && (p_rdp->vHQCV.mc)) /* Si existen componentes afectados. Actualizo valores de vHD. */
    {
      /* Calculo de la conjuncion logica de vEx con vHQCV para generar el vector vHD. */
      for(i=0; i < p_rdp->mII.columnas; i++)
      {
          p_rdp->vHD.vector[i] = (int)(p_rdp->vEx.vector[i] * p_rdp->vHQCV.vector[i]);
      }

      if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Actualizacion de %s con exito!!!\n", p_rdp->vHD.name);
    }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo actualiza los componentes vMA, vE, vW, vQ, vB, vL, vA, vEx y vHD de una RDPG_o luego de que se asignan los componentes mII, mIR,
 * mIH y vMI.
 *
 * @param      p_rdp  Puntero la direccion de memoria del objeto RDPG_o sobre el que se desea actualizar sus componentes.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void update_work_components(RDPG_o *p_rdp)
{
  update_vE(p_rdp);
  update_vW(p_rdp);
  update_vQ(p_rdp);
  update_vB(p_rdp);
  update_vL(p_rdp);
  update_vA(p_rdp);
  update_vEx(p_rdp);
  update_vHD(p_rdp);
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el campo read_mode de una RDPG_o. Esto equivale a que la RDPG_o cambia su modo de lectura y cuando un usuario realiza
 * una solicitud de lectura al driver, segun el modo configurado se mostraran los datos de acuerdo a cada caso.
 *
 * @param      p_rdp   Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar la modificacion del campo read_mode.
 * @param[in]  p_mode  Es el numero identificador del modo a asignar en el campo read_mode de la RDPG_o enviada por parametro.
 * Detalles de modos validos en enum ID_READ_MODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_read_mode(RDPG_o *p_rdp, ID_READ_MODE p_mode)
{
  if(p_mode != p_rdp->read_mode) /* Si se trata del mismo modo, no se realiza ningun cambio.*/
  {
    if(valid_read_mode_id(p_mode) == VALID_ID)
    {
      p_rdp->read_mode = p_mode;
      if((p_rdp->read_mode) == ID_MCOMPONENT_MODE)
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se asigna modo de lectura de componentes matriz.\n");
        set_read_smode(p_rdp, ID_MCOMP_SMODE);
        p_rdp->select_comp = mII; /* Por defecto se establese componente mII como seleccionado. */
      }
      else if((p_rdp->read_mode) == ID_VCOMPONENT_MODE)
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se asigna modo de lectura de componentes vector.\n");
        set_read_smode(p_rdp, ID_VCOMP_SMODE);
        p_rdp->select_comp = vMA; /* Por defecto se establese componente vMA como seleccionado. */
      }
      else if( p_rdp->read_mode < ID_ERROR_MODE)
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se asigna modo de lectura de informacion de RDPG. RDPGinfo.\n");
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se configuro RDPErrorMode. Ingrese un modo de lectura valido.\n");  
      }
    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El ID de modo de lectura %d no existe como un ID valido dentro de la libreria RDPG_o.\n", p_mode);
    }
  } 
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el campo read_smode de una RDPG_o. En algunos casos un modo de lectura en la RDPG_o tiene diferentes submodos de lectura 
 * para seleccionar. Cuando un usuario realiza una solicitud de lectura al driver, segun el modo y submodo de lectura configurado, se mostraran los datos
 * que se correnponda en cada caso.
 *
 * @param      p_rdp    Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar la modificacion del campo read_smode.
 * @param[in]  p_smode  Es el numero identificador del sub-modo a asignar en el campo read_smode de la RDPG_o enviada por parametro. 
 * Detalles de submodos validos en enum ID_READ_SUBMODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_read_smode(RDPG_o *p_rdp, ID_READ_SUBMODE p_smode)
{
  if(p_smode != p_rdp->read_smode) /* Si se trata del mismo sub-modo, no se realiza ningun cambio.*/
  {
    if(valid_read_smode_id(p_smode) == VALID_ID)
    {
      p_rdp->read_smode = p_smode;
      if((p_rdp->read_smode) == ID_MCOMP_SMODE)
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se asigna sub-modo de lectura a informacion de componentes matriz.\n");
      }
      else if( p_rdp->read_smode == ID_VCOMP_SMODE)
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se asigna sub-modo de lectura a informacion de componentes vector.\n");
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se configuro RDPErrorSMode. Ingrese un modo de lectura valido.\n"); 
      }
    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El ID de sub-modo de lectura %d no existe como un ID valido dentro de la libreria RDPG_o.\n", p_smode);
    }
  } 
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el valor de la variable posVP, que representa la posicion de la vista de plazas que se desean mostrar en los componentes
 * de una RDPG_o. El valor se cambia siempre que sea menor a la cantidad de plazas que es el tope maximo al que se puede cambiar.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar el cambio de la posicion de vista de plazas.
 * @param[in]  p_entrada    Puntero a la direccion de memoria de la cadena de caracteres que contiene el numero entero de la plaza desde la que se desea 
 * comenzar a ver cada uno de los componentes de la RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_posVP(RDPG_o *p_rdp, char *p_entrada)
{
  int aux_n; /* variable que almacena el valor entero. */
  //size_t p_pos;
  
  if( extraer_entero(p_entrada, &aux_n) == ERROR_EXTRAER_ENTERO)
  {
    p_rdp->error_code = -EC_falloPosVP;
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se puede configurar la variable posVP, falla en extraccion de entero.\n");
  }
  else
  {
    //p_pos = (size_t)aux_n;
    if(aux_n < p_rdp->mcomponent[mII]->filas)
    {
      p_rdp->posVP = aux_n;
    }
    else
    {
      p_rdp->error_code = -EC_falloPosVP;
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo el cambio de variable posVP, el valor ingresado supera la cantidad de plazas. \n");
    }
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el valor de la variable posVT, que representa la posicion de la vista de transiciones que se desean mostrar en los
 * componentes de una RDPG_o. El valor se cambia siempre que sea menor a la cantidad de transiciones que es el tope maximo al que se puede cambiar.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar el cambio de la posicion de vista de transiciones.
 * @param[in]  p_entrada    Puntero a la direccion de memoria de la cadena de caracteres que contiene el numero entero de la transicion desde el que se
 * desea comenzar a ver cada uno de los componentes de la RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_posVT(RDPG_o *p_rdp, char *p_entrada)
{
  int aux_n; /* variable que almacena el valor entero. */
  //size_t p_pos;
  
  if( extraer_entero(p_entrada, &aux_n) == ERROR_EXTRAER_ENTERO)
  {
    p_rdp->error_code = -EC_falloPosVT;
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se puede configurar la variable posVT, falla en extraccion de entero.\n");
  }
  else
  {
    //p_pos = (size_t)aux_n;
    if(aux_n < p_rdp->mcomponent[mII]->columnas)
    {
      p_rdp->posVT = aux_n;
    }
    else
    {
      p_rdp->error_code = -EC_falloPosVT;
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo el cambio de variable posVT, el valor ingresado supera la cantidad de transiciones. \n");
    }
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el valor de la variable vdim, que representa el numero de plazas y transiciones habilitado para la visualizacion de los
 * componentes de una RDPG_o. El valor se cambia siempre que sea menor al valor maximo admitido por la libreria (macro MAX_VDIM).
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea realizar el cambio de la variable.
 * @param[in]  p_entrada    Puntero a la direccion de memoria de la cadena de caracteres que contiene el valor al que se desea cambiar la variable vdim
 * de la RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_vdim(RDPG_o *p_rdp, char *p_entrada)
{
  int aux_n;    /* variable que almacena el valor entero. */
  size_t p_pos;
  
  if( extraer_entero(p_entrada, &aux_n) == ERROR_EXTRAER_ENTERO)
  {
    p_rdp->error_code = -EC_falloVdim;
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_error: No se puede configurar la variable vdim de %s, falla en extraccion de entero.\n", p_rdp->name);
  }
  else
  {
    p_pos = (size_t)aux_n;
    if((p_pos >= MIN_VDIM) && (p_pos <= MAX_VDIM))
    {
      p_rdp->vdim = p_pos;
    }
    else
    {
        p_rdp->error_code = -EC_falloVdim;
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo el cambio de variable vdim de %s, el valor ingresado supera el maximo o no supera el minimo admitido. \n", p_rdp->name);
    }
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de realizar el cambio del modo de lectura y el componente seleccionado para ser mostrado al usuario cuando
 * se solicita una lectura.
 *
 * @param      p_rdp   Puntero a la direccion de memoria del obeto RDPG_o sobre le ques se desea realizar el cambio.
 * @param[in]  p_comp  Numero del componente que se desea seleccionar para ser tenido en cuanta en la proxima lectura.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_catComp(RDPG_o *p_rdp, int p_comp)
{
  switch(p_comp)
  {
    case cat_mII:
            set_read_mode(p_rdp, ID_MCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)mII);  /*selecciona matriz de incidencia I cuando el driver es leido. */
            break;

    case cat_mIH:
            set_read_mode(p_rdp, ID_MCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)mIH);  /*selecciona matriz de incidencia H cuando el driver es leido. */
            break;
    
   case cat_mIR:
            set_read_mode(p_rdp, ID_MCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)mIR);  /*selecciona matriz de incidencia R cuando el driver es leido. */
            break;
    
    case cat_mIRe:
            set_read_mode(p_rdp, ID_MCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)mIRe);  /*selecciona matriz de incidencia Re cuando el driver es leido. */
            break;
    
    case cat_mD:
            set_read_mode(p_rdp, ID_MCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)mD);  /*selecciona matriz de incidencia D cuando el driver es leido. */
            break;
    
    case cat_vMI:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vMI);  /*selecciona vector de marcado inicial vMI. */
            break;
    
    case cat_vMA:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vMA);  /*selecciona vector de marcado actual. */
            break;
    
    case cat_vMN:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vMN);  /*selecciona vector de marcado nuevo. */
            break;
    
    case cat_vE:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vE);  /*selecciona vector E. */
            break;
    
    case cat_vQ:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vQ);  /*selecciona vector Q. */
            break;
    
    case cat_vW:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vW);  /*selecciona vector W. */
            break;
    
    case cat_vB:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vB);  /*selecciona vector B. */
            break;
    
    case cat_vL:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vL);  /*selecciona vector L. */
            break;
    
    case cat_vG:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vG);  /*selecciona vector G. */
            break;
    
    case cat_vA:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vA);  /*selecciona vector A. */
            break;
    
    case cat_vUDT:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vUDT);  /*selecciona vector UDT. */
            break;
    
    case cat_vEx:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vEx);  /*selecciona vector Ex. */
            break;
    
    case cat_vHQCV:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vHQCV);  /*selecciona vector HQCV. */
            break;

    case cat_vHD:
            set_read_mode(p_rdp, ID_VCOMPONENT_MODE);
            set_select_comp(p_rdp, (int)vHD);  /*selecciona vector HD. */
            break;

    default: 
          if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo realizar cambio de componente para funcion cat. Codigo de lectura %d no valido.\n", p_comp);
          p_rdp->error_code = -EC_CodigoCatComp;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion incrementa en uno la cola de la variable de condicion asociada a la transicion que se desea disparar (vector vHQCV).
 *             El numero de transicion es enviado por parametro dentro de una cadena de caracteres.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre le ques se desea realizar la operacion.
 * @param      p_entrada  Puntero a la cadena de caracter que contiene el numero de transicion asociado a la cola de la variable de transicion.
 *                        El numero de transicion enviado coincide con la posicion del vector vHQCV[pos] que se desea modificar, ya que existe una CV por 
 *                        transicion de la RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void inc_vHQCV(RDPG_o *p_rdp, char *p_entrada)
{
  int pos_e=0;
  p_rdp->error_code = EC_NULL;

  /* Si existe vector_o*/
  if(p_rdp->vHQCV.mc == OBJ_CREATED){ /* Verifico si existe vector a traves de marca de creacion de vector_o.*/

    /* Extraccion de posicion de elemento y valor desde cadena de caracter a variables enteras.*/
    if(extraer_entero(p_entrada, &pos_e)==EXTRACCION_OK) /* Si extraccion es exitosa. Incremento a uno elemento de posicion indicada para vHQCV. */
    {
      if((pos_e >= 0) && (pos_e < p_rdp->vHQCV.elements)) /* Validacion de direccion correcta en vector. */
      {
        p_rdp->vHQCV.vector[pos_e]++;
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Nuevo hilo en cola, componente (%s) con vector[%d] = %d hilos en espera. \n", p_rdp->vHQCV.name, pos_e, p_rdp->vHQCV.vector[pos_e]);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de elemento fuera de limite (direccion incorrecta). \n", p_rdp->vHQCV.name);
        p_rdp->error_code = -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para vector %s. \n", p_rdp->vHQCV.name);
      p_rdp->error_code = -EC_falloADD;
    }

    return;
  }
  else 
  {
     if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. El componente %s no fue creado. \n", p_rdp->vHQCV.name);
     p_rdp->error_code = -EC_falloADD;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion decrementa en uno la cola de la variable de condicion asociada a la transicion que se desea disparar (vector vHQCV).
 *             El numero de transicion es enviado por parametro dentro de una cadena de caracteres.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre le ques se desea realizar la operacion.
 * @param      p_entrada  Puntero a la cadena de caracter que contiene el numero de transicion asociado a la cola de la variable de transicion.
 *                        El numero de transicion enviado coincide con la posicion del vector vHQCV[pos] que se desea modificar, ya que existe una CV por 
 *                        transicion de la RDPG.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dec_vHQCV(RDPG_o *p_rdp, char *p_entrada)
{
  int pos_e=0;
  p_rdp->error_code = EC_NULL;

  /* Si existe vector_o*/
  if(p_rdp->vHQCV.mc == OBJ_CREATED){ /* Verifico si existe vector a traves de marca de creacion de vector_o.*/

    /* Extraccion de posicion de elemento y valor desde cadena de caracter a variables enteras.*/
    if(extraer_entero(p_entrada, &pos_e)==EXTRACCION_OK) /* Si extraccion es exitosa. Incremento a uno elemento de posicion indicada para vHQCV. */
    {
      if((pos_e >= 0) && (pos_e < p_rdp->vHQCV.elements)) /* Validacion de direccion correcta en vector. */
      {
        p_rdp->vHQCV.vector[pos_e]--;
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Nuevo hilo en cola, componente (%s) con vector[%d] = %d hilos en espera. \n", p_rdp->vHQCV.name, pos_e, p_rdp->vHQCV.vector[pos_e]);
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo asignacion de elemento en componente %s. Referencia de elemento fuera de limite (direccion incorrecta). \n", p_rdp->vHQCV.name);
        p_rdp->error_code = -EC_falloADD;
      }
    }
    else /* Si extraccion de valores falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de datos para vector %s. \n", p_rdp->vHQCV.name);
      p_rdp->error_code = -EC_falloADD;
    }

    return;
  }
  else 
  {
     if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No se pudo agregar valor. El componente %s no fue creado. \n", p_rdp->vHQCV.name);
     p_rdp->error_code = -EC_falloADD;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si el identificador del modo de lectura enviado por parametro pertenece al conjunto de ids definidos en la
 * enumeracion ID_READ_MODE.
 *
 * @param[in]  p_mode  Es el id del modo que se valida si pertenece al conjunto ID_READ_MODE.
 *
 * @return     El retorno es:
 *        VALID_ID: Si el identificador del modo de lectura enviado por parametro pertenece al conjunto ID_READ_MODE.
 *        INVALID_ID: Si el identificador del modo de lectura enviado por parametro no pertenece al conjunto ID_READ_MODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int valid_read_mode_id(ID_READ_MODE p_mode)
{
  size_t i;
  for(i=ID_MCOMPONENT_MODE; i < ID_RM_END; i++)
  {
    if(p_mode==i)
    {
      return VALID_ID;
    }
  }
  return INVALID_ID;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si el identificador del sub-modo de lectura enviado por parametro pertenece al conjunto de ids definidos en la
 * enumeracion ID_READ_SUBMODE.
 *
 * @param[in]  p_smode  Es el id del sub-modo que se valida si pertenece al conjunto ID_READ_SUBMODE.
 *
 * @return     El retorno es:
 *        VALID_ID: Si el identificador del sub-modo de lectura enviado por parametro pertenece al conjunto ID_READ_SUBMODE.
 *        INVALID_ID: Si el identificador del sub-modo de lectura enviado por parametro no pertenece al conjunto ID_READ_SUBMODE.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int valid_read_smode_id(ID_READ_SUBMODE p_smode)
{
  size_t i;
  for(i=ID_RSM_INIT; i < ID_RSM_END; i++)
  {
    if(p_smode==i)
    {
      return VALID_ID;
    }
  }
  return INVALID_ID;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion retorna la direccion de la cadena de caracteres del nombre de un componente de una RDPG_o que coincida con el valor 
 * del campo select_comp de la RDPG_o.
 *
 * @param      p_rdp    Puntero a la direccion de memoria del objeto RDPG_o sobre el cual se desea conocer el nombre del componente indicado por el campo
 * select_comp de la RDPG_o.
 *
 * @return     El retorno es:
 *        La direccion de la cadena de caracteres del nombre del componente seleccionado con el campo select_comp de la RDPG_o.
 *        En caso de no haber ninguna concidencia se retorna la direccion por defecto de error (name_error).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rdpg_component_name(RDPG_o *p_rdp, int p_comp)
{
  char *p_name; /* puntero a la direccion de nombre.*/

  p_name = name_error; /* Inicializo direccion de puntero en caso de error.*/

  switch(p_rdp->read_mode) {
    case ID_MCOMPONENT_MODE:
      switch(p_comp)
      {
        case mII:
            if(&(p_rdp->mII.name) != NULL)
              p_name = p_rdp->mII.name;
            break;

        case mIH:
              if(&(p_rdp->mIH.name) != NULL)
                  p_name = p_rdp->mIH.name;
                break;
        
        case mIR:
                if(&(p_rdp->mIR.name) != NULL)
                  p_name = p_rdp->mIR.name;
                break;

        case mIRe:
                if(&(p_rdp->mIRe.name) != NULL)
                  p_name = p_rdp->mIRe.name;
                break;

        case mD:
          if(&(p_rdp->mD.name) != NULL)
                  p_name = p_rdp->mD.name;
                break;  
      }
      break;
    
      case ID_VCOMPONENT_MODE:
        switch(p_comp)
      {
          case vMA:
              if(&(p_rdp->vMA.name) != NULL)
                  p_name = p_rdp->vMA.name;
                break;

          case vMI:
              if(&(p_rdp->vMI.name) != NULL)
              p_name = p_rdp->vMI.name;
                break;

          case vMN:
              if(&(p_rdp->vMN.name) != NULL)
                  p_name = p_rdp->vMN.name;
                break;


          case vE:
                if(&(p_rdp->vE.name) != NULL)
                  p_name = p_rdp->vE.name;
                break;


          case vQ:
                if(&(p_rdp->vQ.name) != NULL)
                  p_name = p_rdp->vQ.name;
                break;

          case vW:
                if(&(p_rdp->vW.name) != NULL)
                  p_name = p_rdp->vW.name;
                break;

          case vL:
                if(&(p_rdp->vL.name) != NULL)
                  p_name = p_rdp->vL.name;
                break;

          case vB:
                if(&(p_rdp->vB.name) != NULL)
                  p_name = p_rdp->vB.name;
                break;

          case vG:
                if(&(p_rdp->vG.name) != NULL)
                  p_name = p_rdp->vG.name;
                break;

          case vA:
                if(&(p_rdp->vA.name) != NULL)
                  p_name = p_rdp->vA.name;
                break;

          case vEx:
                if(&(p_rdp->vEx.name) != NULL)
                  p_name = p_rdp->vEx.name;
                break;

          case vUDT:
              if(&(p_rdp->vUDT.name) != NULL)
                  p_name = p_rdp->vUDT.name;
                break;

          case vHQCV:
                if(&(p_rdp->vHQCV.name) != NULL)
                  p_name = p_rdp->vHQCV.name;
                break;
          
          case vHD:
                if(&(p_rdp->vHD.name) != NULL)
                  p_name = p_rdp->vHD.name;
                break;
          
      }
      break;        
    }

    return p_name;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion cambia el valor del campo select_comp de una RDPG_o para cambiar de componente en componente de una RDPG_o. De acuerdo al
 *
 * @param      p_rdp        Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea cambiar el campo select_comp.
 * @param[in]  p_component  Es el numero entero indentificador de componente a asignar en select_comp.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
void set_select_comp(RDPG_o *p_rdp, int p_component)
{
  if(p_component != p_rdp->select_comp) /* Si se trata del mismo componente asignado actualmente, no ai modificaciones.*/
  {
    if(p_rdp->read_mode == ID_MCOMPONENT_MODE)
    {
      if(valid_mcomp_id(p_component) == VALID_ID)
      {
        p_rdp->select_comp = p_component;   
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s (ID: %d) seleccionado para mostrar en funcion read().\n", rdpg_component_name(p_rdp, p_rdp->select_comp), p_rdp->select_comp);
      }
    }
    else if(p_rdp->read_mode == ID_VCOMPONENT_MODE)
    {
      if(valid_vcomp_id(p_component) == VALID_ID)
      {
        p_rdp->select_comp = p_component;   
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Componente %s (ID: %d) seleccionado para mostrar en funcion read().\n", rdpg_component_name(p_rdp, p_rdp->select_comp), p_rdp->select_comp);
      }
    }
    else
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El ID de componente %d no existe como un ID valido dentro de la libreria RDPG_o.\n", p_component);
    }
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si el identificador de componente matrix_o enviado por parametro pertenece al conjunto de ids definidos en la
 * enumeracion ID_MCOMPONENT.
 *
 * @param[in]  p_mcomp_id  Es el identificador de componente matrix_o a validar.
 *
 * @return     El retorno es:
 *        VALID_ID: Si el identificador de componente enviado por parametro pertenece a la enumeracion ID_MCOMPONENT.
 *        INVALID_ID: Si el identificador de componente enviado por parametro no pertenece a la enumeracion ID_MCOMPONENT.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int valid_mcomp_id(int p_mcomp_id)
{
  size_t i;
  for(i=ID_MC_INIT; i < ID_MC_END; i++)
  {
    if(p_mcomp_id==(int)i)
    {
      return VALID_ID;
    }
  }
  return INVALID_ID;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si el identificador de componente vector_o enviado por parametro pertenece al conjunto de ids definidos en la
 * enumeracion ID_VCOMPONENT.
 *
 * @param[in]  p_vcomp_id  Es el identificador de componente vector_o a validar.
 *
 * @return     El retorno es:
 *        VALID_ID: Si el identificador de componente enviado por parametro pertenece a la enumeracion ID_VCOMPONENT.
 *        INVALID_ID: Si el identificador de componente enviado por parametro no pertenece a la enumeracion ID_VCOMPONENT.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int valid_vcomp_id(int p_vcomp_id)
{
  size_t i;
  for(i=ID_VC_INIT; i < ID_VC_END; i++)
  {
    if(p_vcomp_id==(int)i)
    {
      return VALID_ID;
    }
  }
  return INVALID_ID;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion escribe un componente del objeto RDPG_o enviado por parametro. La funcion se ejecuta cuando un usuario realiza una solicitud
 * de lectura al driver. De acuerdo al modo de lectura y el campo selec_comp configurado en la RDPG_o mostrara el componente correspondiente.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se realiza la lectura del componente.
 * @param      p_buf  Es el puntero al buffer sobre el que se realiza la escritura del componente de la RDPG_o cuando un usuario solicita una lectura
 * al driver.
 * @param[in]  p_len  Es el tamaño disponible que se tiene para escribir en el buffer del espacio usuario.
 *
 * @return     El retorno es el numero de bytes escritos por la lectura de un usuario.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int read_rdpg_component(RDPG_o *p_rdp, char *p_kbuf, int p_comp, int p_read_mode)
{
  int nr_bytes=0;
  memset(p_kbuf, '\0', strlen(p_kbuf));

  /* Se redifinen los headers de acuerdo los valores de posVP y posVT. */
  define_headers(p_rdp);

  switch(p_read_mode) {
    case ID_MCOMPONENT_MODE:
      switch(p_comp)
      {
        case mII:
            (void)concat_x1(p_kbuf, s_space6);
            (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
            nr_bytes = print_rdpg_mcomponent(&(p_rdp->mII), p_kbuf, p_rdp->posVP, p_rdp->posVT, p_rdp->vdim);
            //return print_rdpg_mcomponent(&(p_rdp->mII), p_kbuf, p_rdp->posVP, p_rdp->posVT, p_rdp->vdim);
            break;

        case mIH:
            (void)concat_x1(p_kbuf, s_space6);
            (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
            nr_bytes = print_rdpg_mcomponent(&(p_rdp->mIH), p_kbuf, p_rdp->posVP, p_rdp->posVT, p_rdp->vdim);
            break;

        case mIR:
            (void)concat_x1(p_kbuf, s_space6);
            (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
            nr_bytes = print_rdpg_mcomponent(&(p_rdp->mIR), p_kbuf, p_rdp->posVP, p_rdp->posVT, p_rdp->vdim);
            break;

        case mIRe:
            (void)concat_x1(p_kbuf, s_space6);
            (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
            nr_bytes = print_rdpg_mcomponent(&(p_rdp->mIRe), p_kbuf, p_rdp->posVP, p_rdp->posVT, p_rdp->vdim);
            break;

        case mD:
            (void)concatn_x2(p_kbuf, p_rdp->s_header_disparos, s_newLine);
            nr_bytes = print_rdpg_mcomponent(&(p_rdp->mD), p_kbuf, p_rdp->posVT, p_rdp->posVT, p_rdp->vdim);
            break;
      }
      break;

    case ID_VCOMPONENT_MODE:
      switch(p_comp)
      {
        case vMA:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_plazas, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vMA), p_kbuf, p_rdp->posVP, p_rdp->vdim);
           break;        

        case vMI:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_plazas, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vMI), p_kbuf, p_rdp->posVP, p_rdp->vdim);
           break;

        case vMN:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_plazas, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vMN), p_kbuf, p_rdp->posVP, p_rdp->vdim);
           break;

        case vE:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vE), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vQ:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_plazas, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vQ), p_kbuf, p_rdp->posVP, p_rdp->vdim);
           break;

        case vW:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_plazas, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vW), p_kbuf, p_rdp->posVP, p_rdp->vdim);
           break;

        case vL:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vL), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vB:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vB), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vG:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vG), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vA:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vA), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vEx:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vEx), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;
        
        case vUDT:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vUDT), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;

        case vHQCV:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vHQCV), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;
        
        case vHD:
           (void)concatn_x2(p_kbuf, p_rdp->s_header_transiciones, s_newLine);
           nr_bytes = print_rdpg_vcomponent(&(p_rdp->vHD), p_kbuf, p_rdp->posVT, p_rdp->vdim);
           break;
      }
      break;

    default:
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo de lectura. Modo de componente seleccionado desconocido ID_READ_MODE = %d. \n", p_rdp->read_mode);
        return 0;
    }

    return nr_bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion escribe en el buffer del espacio usuario el componente matrix_o de una RDPG_o que se indica por parametro.
 * La funcion tiene las siguientes limitaciones de escritura:
 * - No se podran escribir mas de MAX_VDIM filas y columnas del componente matrix_o indicado por parametro.
 * - Se escriben los elementos del componente matrix_o comenzando por la fila posVP indicada por parametro.
 * - Se escriben los elementos del componente matrix_o comenzando por la columna posVT indicada por parametro.
 *
 * @param      p_mo    Puntero a la direccion de memoria del objeto matrix_o sobre el que se escribe en el buffer de usuario.
 * @param      kbuf    Puntero direccion de memoria de la cadena de caracteres del espacio kernel sobre la que se organizan los datos del componente
 *             matrix_o indicado por parametro, para copiar en el buffer de usuario.
 * @param      p_buf   Puntero a la direccion de memoria de la cadena de caracteres del espacio usuario en donde se copiaran los datos desde el espacio kernel.
 * @param[in]  p_len   Tamaño maximo de la cadena de caracteres del espacio usuario.
 * @param[in]  p_posP  Numero entero positivo que representa la posicion de plaza con la que se debe comenzar a mostrar los datos del componente matrix_o
 *             indicado por parametro.
 * @param[in]  p_posT  Numero entero positivo que representa la posicion de transicion con la que se debe comenzar a mostrar los datos del componente matrix_o
 *             indicado por parametro.
 *
 * @return     Se retorna el numero de bytes escritos por el kernel al espacio usuario. Equivalente al numero leido por el espacio usuario en el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int print_rdpg_mcomponent(matrix_o *p_mo, char *p_kbuf, size_t p_posP, size_t p_posT, size_t p_vdim)
{
  char c_aux[N_BYTES];          /* Cadena auxiliar que almacena los enteros en una cadena de caracteres (string). */
  memset(c_aux, '\0', N_BYTES);
  int i;
  int j;                   /* i,j : recorren la matriz. */
  int nr_bytes;            /* nr_bytes: numero de bytes leidos. */
  size_t vp, vt;           /* vp: plazas visualizadas. vt: transiciones visualizadas. */
  nr_bytes = 0;

  if(p_mo->mc == OBJ_CREATED){

    for(i = p_posP, vp=0; (i<p_mo->filas) && (vp < p_vdim); i++, vp++)
    {
      /* Generacion de cabecera vertical*/
      if( (p_mo->obj_id == mII) || (p_mo->obj_id == mIH) || (p_mo->obj_id == mIR) || (p_mo->obj_id == mIRe))
      {
        itoa(i, c_aux, N_BYTES);                     /* Se convierte entero a ascii. */
        if(i<10)
          (void)concat_x2(p_kbuf, "    P", c_aux);   /* Se concantena entero con dos espacios previos. */

        else if(i<100)
          (void)concat_x2(p_kbuf, "   P", c_aux);    /* Se concantena entero con dos espacios previos. */

        else
          (void)concat_x2(p_kbuf, "  P", c_aux);     /* Se concantena entero con dos espacios previos. */
      }


      for(j = p_posT, vt=0; (j<p_mo->columnas) && (vt < p_vdim); j++, vt++)
      {
        if(p_mo->matriz[i][j] >= 0) {
          
          itoa(p_mo->matriz[i][j], c_aux, N_BYTES);   /* Se convierte entero a ascii. */

          if(j<10)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con cuatro espacios previos. */

          else if(j<100)
            (void)concat_x2(p_kbuf, s_space3, c_aux); /* Se concantena entero con tres espacios previos. */

          else
            (void)concat_x2(p_kbuf, s_space4, c_aux); /* Se concantena entero con dos espacios previos. */
        
        } else {
          itoa(p_mo->matriz[i][j], c_aux, N_BYTES);   /* Se convierte entero a ascii. */

          if(j<10)
            (void)concat_x2(p_kbuf, s_space1, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */

          else if(j<100)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */

          else
            (void)concat_x2(p_kbuf, (char *)s_space3, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */
        }
      } // Fin for 1
      
      (void)concat_x1(p_kbuf, s_newLine);             /* Se concantena i fila con un salto de linea. */

    } // Fin for 2
    
    nr_bytes= strlen(p_kbuf);
    //nr_bytes=print_to_user(kbuf, p_buf, p_len);
    //return print_to_user(kbuf, p_buf, p_len);
   }
   else
   {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El componente %s no existe. \n", p_mo->name);
   }
  
  return nr_bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion escribe en el buffer del espacio usuario el componente matrix_o de una RDPG_o que se indica por parametro.
 * Esta funcion es una version de print_rdpg_mcomponent pero con menos parametros.
 * La funcion tiene las siguientes limitaciones de escritura:
 * - No se podran escribir mas de MAX_VDIM filas y columnas del componente matrix_o indicado por parametro.
 * - Se escriben los elementos del componente matrix_o comenzando por la fila posVP, se extrae el dato desde parametro p_rdp.
 * - Se escriben los elementos del componente matrix_o comenzando por la columna posVT, , se extrae el dato desde parametro p_rdp.
 *
 * @param      p_rdp       Puntero a la direccion de memoria del objeto RDPG_o del cual se escribe el componente p_mcomp_id al espacio usuario.
 * @param[in]  p_mcomp_id  Es el numero identificador del componente matrix_o que se desea escribir en el espacio usuario.
 * @param      kbuf    Puntero direccion de memoria de la cadena de caracteres del espacio kernel sobre la que se organizan los datos del componente
 *             matrix_o indicado por parametro, para copiar en el buffer de usuario.
 * @param      p_buf   Puntero a la direccion de memoria de la cadena de caracteres del espacio usuario en donde se copiaran los datos desde el espacio kernel.
 * @param[in]  p_len   Tamaño maximo de la cadena de caracteres del espacio usuario.
 *
 * @return     Se retorna el numero de bytes escritos por el kernel al espacio usuario. Equivalente al numero leido por el espacio usuario en el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int print_rdpg_mcomponent2(RDPG_o *p_rdp, int p_mcomp_id, char *p_kbuf)
{
  char c_aux[N_BYTES];  /* Cadena auxiliar que almacena los enteros en una cadena de caracteres (string). */
  memset(c_aux, '\0', N_BYTES);
  int i;                /* i,j : recorren la matriz */
  int j;                /* k: va corriendo el puntero del buffer del kernel kbuf que luego se copia al buffer del espacio usuario;*/
  int nr_bytes;         /* nr_bytes: numero de bytes leidos.*/
  size_t vp, vt;        /* vp: plazas visualizadas. vt: transiciones visualizadas. */
  nr_bytes = 0;

  if( (valid_mcomp_id(p_mcomp_id)==VALID_ID) && (p_rdp->mcomponent[p_mcomp_id]->mc == OBJ_CREATED) ){

    for(i = p_rdp->posVP, vp=0; (i<p_rdp->mcomponent[p_mcomp_id]->filas) && (vp < p_rdp->vdim); i++, vp++)
    {
      /* Generacion de cabecera vertical*/
      if( (p_rdp->mcomponent[p_mcomp_id]->obj_id == mII) || (p_rdp->mcomponent[p_mcomp_id]->obj_id == mIH) || (p_rdp->mcomponent[p_mcomp_id]->obj_id == mIR) || (p_rdp->mcomponent[p_mcomp_id]->obj_id == mIRe))
      {
        itoa(i, c_aux, N_BYTES); /* Se convierte entero a ascii. */
        if(i<10)
            (void)concat_x2(p_kbuf, "    P", c_aux); /* Se concantena entero con dos espacios previos. */

          else if(i<100)
            (void)concat_x2(p_kbuf, "   P", c_aux);  /* Se concantena entero con dos espacios previos. */

          else
            (void)concat_x2(p_kbuf, "  P", c_aux);   /* Se concantena entero con dos espacios previos. */
      }


      for(j = p_rdp->posVT, vt=0; (j<p_rdp->mcomponent[p_mcomp_id]->columnas) && (vt < p_rdp->vdim); j++, vt++)
      {
        if(p_rdp->mcomponent[p_mcomp_id]->matriz[i][j] >= 0) {
          
          itoa(p_rdp->mcomponent[p_mcomp_id]->matriz[i][j], c_aux, N_BYTES); /* Se convierte entero a ascii. */

          if(j<10)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con cuatro espacios previos. */

          else if(j<100)
            (void)concat_x2(p_kbuf, s_space3, c_aux); /* Se concantena entero con tres espacios previos. */

          else
            (void)concat_x2(p_kbuf, s_space4, c_aux); /* Se concantena entero con dos espacios previos. */
        
        } else {
          itoa(p_rdp->mcomponent[p_mcomp_id]->matriz[i][j], c_aux, N_BYTES); /* Se convierte entero a ascii. */

          if(j<10)
            (void)concat_x2(p_kbuf, s_space1, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */

          else if(j<100)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */

          else
            (void)concat_x2(p_kbuf, s_space3, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */
        }
      } // Fin for 1
      
      (void)concat_x1(p_kbuf, s_newLine); /* Se concantena i fila con un salto de linea. */

    } // Fin for 2
    
    nr_bytes= strlen(p_kbuf);
    //nr_bytes=print_to_user(kbuf, p_buf, p_len);
    //return print_to_user(kbuf, p_buf, p_len);
   }
   else
   {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El componente %s no existe. \n", p_rdp->mcomponent[p_mcomp_id]->name);
   }
  
  return nr_bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion escribe en el buffer del espacio usuario el componente vector_o de una RDPG_o que se indica por parametro.
 * La funcion tiene las siguientes limitaciones de escritura:
 * - No se podran escribir mas de MAX_VDIM elementos del componente vector_o indicado por parametro.
 * - Se escriben los elementos del componente vector_o comenzando por el elemento posV indicado por parametro.
 *
 * @param      p_vo   Puntero a la direccion de memoria del objeto vector_o sobre el que se escribe en el buffer de usuario.
 * @param      p_kbuf   Puntero direccion de memoria de la cadena de caracteres del espacio kernel sobre la que se organizan los datos del componente
 *            vector_o indicado por parametro, para copiar en el buffer de usuario.
 * @param      p_buf  Puntero a la direccion de memoria de la cadena de caracteres del espacio usuario en donde se copiaran los datos desde el espacio kernel.
 * @param[in]  p_len  Tamaño maximo de la cadena de caracteres del espacio usuario.
 * @param[in]  posV   Numero entero positivo que representa la posicion del elemento con el que se debe comenzar a mostrar los datos del componente vector_o
            indicado por parametro.
 *
 * @return     Se retorna el numero de bytes escritos por el kernel al espacio usuario. Equivalente al numero de bytes leido por el espacio usuario en
 *         el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int print_rdpg_vcomponent(vector_o *p_vo, char *p_kbuf, size_t posV, size_t p_vdim)
{
  char c_aux[N_BYTES];          /* Cadena auxiliar que almacena los enteros en una cadena de caracteres (string). */
  memset(c_aux, '\0', N_BYTES);
  int i;              /* i : se usa para recorrer vector. */
  int nr_bytes;       /* nr_bytes: numero de bytes leidos. */
  size_t ve;
  nr_bytes = 0;

  if(p_vo->mc == OBJ_CREATED){

    for(i = posV, ve=0; (i<p_vo->elements) && (ve < p_vdim); i++, ve++)
    {
        if(p_vo->vector[i] >= 0) {
          
          itoa(p_vo->vector[i], c_aux, N_BYTES);      /* Se convierte entero a ascii. */

          if(i<10)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con dos espacios previos. */

          else if(i<100)
            (void)concat_x2(p_kbuf, s_space3, c_aux); /* Se concantena entero con tres espacios previos. */

          else
            (void)concat_x2(p_kbuf, s_space4, c_aux); /* Se concantena entero con cuatro espacios previos. */
        
        } else {
          itoa(p_vo->vector[i], c_aux, N_BYTES);      /* Se convierte entero a ascii. */
          
          if(i<10)
            (void)concat_x2(p_kbuf, s_space1, c_aux); /* Se concantena entero con un espacio previo ya que es negativo y utiliza un carater -. */

          else if(i<100)
            (void)concat_x2(p_kbuf, s_space2, c_aux); /* Se concantena entero con dos espacios previos ya que es negativo y utiliza un carater -. */

          else
            (void)concat_x2(p_kbuf, s_space3, c_aux); /* Se concantena entero con tres espacios previos ya que es negativo y utiliza un carater -. */
        }

    } // Fin for

    (void)concat_x1(p_kbuf, s_newLine); /* Se concantena i fila con un salto de linea. */

    nr_bytes= strlen(p_kbuf);
    //nr_bytes=print_to_user(kbuf, p_buf, p_len);
   }
   else
   {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El componente %s no existe. \n", p_vo->name);
   }
  
  return nr_bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de escribir en el espacio usuario informacion asociada a una RDPG_o indicada por parametro cuando se solicita una
 * lectura sobre el archivo de dispositivo del driver.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG_o sobre el que se desea escribir los datos.
 * @param      p_buf  Puntero a la direccion de memoria del buffer de usuario sobre el que se realiza la escritura de la informacion de la RDPG_o.
 * @param[in]  p_len  Tamaño disponible que se tiene para escribir en el buffer del espacio usuario.
 *
 * @return     Se retorna el numero de bytes escritos por el kernel al espacio usuario. Equivalente al numero de bytes leido por el espacio usuario en
 *         el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int read_rdpg_info(RDPG_o *p_rdp, char *p_kbuf)
{
  memset(p_kbuf, '\0', strlen(p_kbuf));
  int nr_bytes=0;
  char c_numaux[N_BYTES];

  if(p_rdp->mc == OBJ_CREATED){

    switch(p_rdp->read_mode)
    {
      case ID_INFO_NAME:
        (void)concat_x3(p_kbuf, "   - Nombre de RDPG: ", p_rdp->name, s_newLine);
        break;

      case ID_INFO_PLACES:
        (void)concat_x3(p_kbuf, "   - Numero de plazas de la RDPG: ", p_rdp->s_plazas, s_newLine);
        //(void)concat_x3(p_kbuf, "   - Plazas visibles de RDPG: ", p_rdp->s_header_plazas, s_newLine);   
        break;

      case ID_INFO_TRANSITIONS:
        (void)concat_x3(p_kbuf, "   - Numero de transiciones de la RDPG: ", p_rdp->s_transiciones, s_newLine);
        //(void)concat_x3(p_kbuf, "   - Transiciones visibles de RDPG: ", p_rdp->s_header_transiciones, s_newLine);
        break;

      case ID_INFO_SHOTS:
        itoa(p_rdp->posVP, c_numaux, N_BYTES);
        (void)concat_x3(p_kbuf, "   - Posicion de plaza inicial para visualizacion: ", c_numaux, s_newLine);
        itoa(p_rdp->posVT, c_numaux, N_BYTES);
        (void)concat_x3(p_kbuf, "   - Posicion de transicion inicial para visualizacion: ", c_numaux, s_newLine);
        itoa(p_rdp->vdim, c_numaux, N_BYTES);
        (void)concat_x3(p_kbuf, "   - Dimension de visualizacion de componentes: ", c_numaux, s_newLine);
        //(void)concat_x3(p_kbuf, "   - Disparos visibles de RDPG: ", p_rdp->s_header_disparos, s_newLine);
        break;

      case ID_INFO_MEMORY:
        (void)concat_x3(p_kbuf, "   - Modo de reserva de memoria configurado en la RDPG: ",  p_rdp->s_allocMode, s_newLine);
        (void)concat_x3(p_kbuf, "   - Memoria reservada en el sistema: ",  p_rdp->s_size, " bytes \n");
        (void)concat_x3(p_kbuf, "   - Memoria real reservada en el sistema: ", p_rdp->s_realSize, " bytes \n");
        break;

      case ID_INFO_COMP:
        if((p_rdp->select_comp < ID_MC_END) && (p_rdp->read_smode == ID_MCOMP_SMODE)) /* selector en MCOMP*/
        {
          (void)concat_x3(p_kbuf, "   - Nombre de componente: ", p_rdp->mcomponent[p_rdp->select_comp]->name, s_newLine);
          itoa(p_rdp->mcomponent[p_rdp->select_comp]->tam_bytes, c_numaux, N_BYTES);
          (void)concat_x3(p_kbuf, "   - Memoria reservada por componente: ", c_numaux, " bytes \n");
          itoa(p_rdp->mcomponent[p_rdp->select_comp]->realTam_bytes, c_numaux, N_BYTES);
          (void)concat_x3(p_kbuf, "   - Memoria real reservada por componente: ", c_numaux, " bytes \n");
        }
        else if((p_rdp->select_comp < ID_VC_END) && (p_rdp->read_smode == ID_VCOMP_SMODE)) /* selector en VCOMP*/
        {
          (void)concat_x3(p_kbuf, "   - Nombre de componente: ", p_rdp->vcomponent[p_rdp->select_comp]->name, s_newLine);
          itoa(p_rdp->vcomponent[p_rdp->select_comp]->tam_bytes, c_numaux, N_BYTES);
          (void)concat_x3(p_kbuf, "   - Memoria reservada por componente: ", c_numaux, " bytes \n");
          itoa(p_rdp->vcomponent[p_rdp->select_comp]->realTam_bytes, c_numaux, N_BYTES);
          (void)concat_x3(p_kbuf, "   - Memoria real reservada por componente: ", c_numaux, " bytes \n");
        }
        else /* ID_ERROR_SMODE */
        {
          (void)concat_x1(p_kbuf, "   ERROR: Fallo de lectura.\n"); 
        }
        break;

      case ID_INFO_SHOT_RESULT:
        if(p_rdp->shot_result == (int)SHOT_OK){
          (void)concat_x1(p_kbuf, "   Disparo de transicion exitoso!\n");

        }else if(p_rdp->shot_result == (int)SHOT_FAILED){
          (void)concat_x1(p_kbuf, "   Disparo de transicion fallido!\n");

        }else{
        (void)concat_x1(p_kbuf, "   - Aun no se han efectuado disparos en la RDPG_o cargada en el kernel!\n");          
        }
        break;

      default: /* ID_ERROR_MODE */
        (void)concat_x1(p_kbuf, "   Modo de lectura desconocido.\n");
    }

    nr_bytes = strlen(p_kbuf);
    //nr_bytes+= print_to_user(kbuf, p_buf, p_len);
    //return print_to_user(kbuf, p_buf, p_len);
   }
   else
   {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: No existe ninguna RDPG cargada en el kernel. \n");
   }

   return nr_bytes;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de copiar informacion de una cadena de caracteres desde el espacio kernel a una cadena de caracteres del espacio
 * usuario.
 *
 * @param      kbuf   Puntero a la direccion de memoria de la cadena de caracteres del espacio kernel.
 * @param      p_buf  Puntero a la direccion de memoria de la cadena de caracteres del espacio usuario.
 * @param[in]  p_len  Tamaño maximo de la cadena de caracteres del espacio usuario.
 *
 * @return     Se retorna el numero de bytes escritos por el kernel al espacio usuario. Equivalente al numero de bytes leido por el espacio usuario en
 *         el kernel.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int print_to_user(char *kbuf, char *p_buf, int p_len)
{
  int nr_bytes;

  nr_bytes=strlen(kbuf);
      
  if (p_len< nr_bytes) {
  //if (p_len < BUFFER_LENGTH_RD) {
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_notify: No queda espacio en el archivo de usuario. \n");
    return -ENOSPC; // No queda espacio en el dispositivo
  }

  /* Transferencia de datos desde el espacio kernel al espacio de usuario */  
  if(copy_to_user(p_buf, kbuf, nr_bytes)) {
  //if(copy_to_user(p_buf, kbuf, BUFFER_LENGTH_RD)) { 
    if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_notify: Argumento invalido\n");
    return -EINVAL; // Argumento invalido
  }

  return nr_bytes;
  //return BUFFER_LENGTH_RD;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de retornar el modo de asignacion de memoria que tiene configurada la RDPG_o en una cadena de
 * caracteres sobre el atributo s_allocMode del objeto RDPG_o.
 *
 * @param      p_rdp  Puntero a la RDPG_o sobre la que se desea obtener el modo de reserva de memoria en una cadena de caracteres (String).
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void getString_allocMode(RDPG_o *p_rdp)
{
  memset(p_rdp->s_allocMode, '\0', N_NAME);// se limpia cadena
  switch(p_rdp->mII.alloc_mode)
  {
    case M_KMALLOC: 
          strcpy(p_rdp->s_allocMode, "KMALLOC");
          break;

    case M_KZALLOC: 
          strcpy(p_rdp->s_allocMode, "KZALLOC");
          break;

    case M_KCALLOC: 
          strcpy(p_rdp->s_allocMode, "KCALLOC");
          break;

    case M_VMALLOC: 
          strcpy(p_rdp->s_allocMode, "VMALLOC");
          break;

    case M_VZALLOC: 
          strcpy(p_rdp->s_allocMode, "VZALLOC");
          break;

    default: 
          strcpy(p_rdp->s_allocMode, "KMALLOC"); /* por defecto se configura M_KMALLOC. */

  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Este metodo indica si el objjeto RDPG enviado por parametro tiene cargada una RDPG o se encuentra vacia.
 *
 * @param      p_rdp  Puntero a la direccion de memoria del objeto RDPG sobre el que se consulta.
 *
 * @return     El retorno sera:
 *              - 0 (false): si la RDPG enviada por parametro se encuentra cargada en el kernel.
 *              - 1 (true): si la RDPG enviada por parametro se encuentra vacia.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rdpg_empty(RDPG_o *p_rdp)
{
  if(p_rdp->mc)
    return 0;

  return 1;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion retorna el numero de tokens de una plaza de la RDPG. La plaza es indicada por a cadena de caracter enviada por parametro.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el que se realiza la consulta.
 * @param      p_entrada  Puntero a la cadena de caracter que contiene el numero de la plaza a consultar los tokens.
 *
 * @return     Se retorna el numero de tokens de la plaza consultada.
 *             Se retorna un numero < 0, asociado al error por el cual la consulta falla. 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int get_TokensPlace(RDPG_o *p_rdp, char *p_entrada)
{
  int pos_e=0;

  /* Si existe vMA. */
  if(p_rdp->vMA.mc == OBJ_CREATED){ /* Verifico si existe vector de marcado actual.*/

    if( extraer_entero(p_entrada, &pos_e) == ERROR_EXTRAER_ENTERO) /* Si extraccion de valor falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de numero de plaza desde vector %s. \n", p_rdp->vMA.name);
      p_rdp->error_code = -EC_extraccionDato;
      return -EC_extraccionDato;
    }
    else /* Si extraccion fue exitosa. Verifico dato extraido. */
    {
      if((pos_e >= 0) && (pos_e < p_rdp->vMA.elements)) /* Validacion de direccion correcta en vector. */
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se retorna numero de tokens de plaza P%d. \n", pos_e);
        return p_rdp->vMA.vector[pos_e]; 
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Numero de plaza incorrecto. La plaza P%d no existe en la RDPG cargada. \n", pos_e);
        p_rdp->error_code = -EC_datoIncorrecto;
        return -EC_datoIncorrecto;
      }
    }
  }
  else 
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El vector %s no fue creado. \n", p_rdp->vMA.name);
    p_rdp->error_code = -EC_componenteNoCreado;
    return -EC_componenteNoCreado;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion retorna el numero de hilos a despertar de una transicion de la RDPG. La transicion es indicada por la cadena de caracter enviada
 *             por parametro.
 *
 * @param      p_rdp      Puntero a la direccion de memoria del objeto RDPG_o sobre el que se realizar la consulta.
 * @param      p_entrada  Puntero a la cadena de caracter que contiene el numero de la transicion a consultar los hilos a despertar.
 *
 * @return     Se retorna el numero de hilos a despertar para la transicion solicitada.
 *             Se retorna un numero < 0, asociado al error por el cual la consulta falla. 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
int get_vHDelement(RDPG_o *p_rdp, char *p_entrada)
{
  int pos_e=0;

  /* Si existe vMA. */
  if(p_rdp->vHD.mc == OBJ_CREATED){ /* Verifico si existe vector de hilos a despertar.*/

    if( extraer_entero(p_entrada, &pos_e) == ERROR_EXTRAER_ENTERO) /* Si extraccion de valor falla. Notificamos inconveniente. */
    {
      if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Fallo extraccion de numero de transicion desde vector %s. \n", p_rdp->vHD.name);
      p_rdp->error_code = -EC_extraccionDato;
      return -EC_extraccionDato;
    }
    else /* Si extraccion fue exitosa. Verifico dato extraido. */
    {
      if((pos_e >= 0) && (pos_e < p_rdp->vHD.elements)) /* Validacion de direccion correcta en vector. */
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_INFO "MatrixmodG_info: Se retorna numero de hilos a despertar para transicion T%d. \n", pos_e);
        return p_rdp->vHD.vector[pos_e]; 
      }
      else
      {
        if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: Numero de transicion incorrecto. La transicion T%d no existe en la RDPG cargada. \n", pos_e);
        p_rdp->error_code = -EC_datoIncorrecto;
        return -EC_datoIncorrecto;
      }
    }
  }
  else 
  {
    if(MATRIXMODG_DB_MSG) printk(KERN_ERR "MatrixmodG_error: El vector %s no fue creado. \n", p_rdp->vHD.name);
    p_rdp->error_code = -EC_componenteNoCreado;
    return -EC_componenteNoCreado;
  }
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de configurar el modo por defecto la RDPG_o enviada por parametro.
 *
 * @param      rdp   Puntero de la direccion de memoria del RDPG_o que se desea configurar.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void config_default_mode(RDPG_o *p_rdp)
{
  set_read_mode(p_rdp, ID_MCOMPONENT_MODE);   /* Se configura por defecto el modo de lectura de componente matriz. */
  set_read_smode(p_rdp, ID_MCOMP_SMODE);      /* Se configura por defecto el sub-modo de lectura de informacion de componente matriz. */
  set_select_comp(p_rdp, (int)mII);           /* Se muestra por defecto la matriz de incidencia. */
  p_rdp->posVP = 0;
  p_rdp->posVT = 0;
  p_rdp->vdim = MAX_VDIM;
  p_rdp->shot_result = (int)SHOT_INIT;
  p_rdp->error_code = EC_NULL;
}


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION/IMPLEMENTACION DE FUNCIONES SMP-secure (Multiprocesamiento simetrico seguro)
 * En esta seccion se define el conjunto de funciones SMPs. Se trata de las mismas funciones que maneja una RDPG desde su vtable pero estas funciones
 * tienen soporte al multiprocesamiento simetrico seguro, esto quiere decir que cuando el codigo se somete a multiples hilos paralelamente los
 * recursos estan protegidos.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

extern int SMPs_disparar_rdpg_s(RDPG_o *p_rdp, char *p_entrada, SHOT_MODE p_mode)
{
  int transicion; /* variable que almacena el valor entero del numero de transicion a disparar. */

  if( extraer_entero(p_entrada, &transicion) == ERROR_EXTRAER_ENTERO)
  {
    p_rdp->shot_result = (int)SHOT_FAILED;
    return SHOT_FAILED;
  }
  else /* Disparar transicion */
  {
    return p_rdp->methods->SMPs_shoot_rdpg(p_rdp, transicion, SHOT_MODE_E);
  }
}



int SMPs_disparar_rdpg(RDPG_o *p_rdp, int p_idT, SHOT_MODE p_mode)
{
  int rt_fshoot; /* Retorno de funcion shoot_rdpg() */

  //spin_lock(&mtx_RDP1);
  write_lock(&p_rdp->lock_RDPG);

  rt_fshoot= p_rdp->methods->shoot_rdpg(p_rdp, p_idT, p_mode);

  //spin_unlock(&mtx_RDP1);
  write_unlock(&p_rdp->lock_RDPG);

  return rt_fshoot;
}


int SMPs_add_value_vG(RDPG_o *p_rdp, char *p_entrada)
{
  int rt_fadd; /* Retorno de funcion add_value_vG() */

  //spin_lock(&mtx_RDP1);
  write_lock(&p_rdp->lock_RDPG);

  rt_fadd = add_value_vG(p_rdp, p_entrada);

  //spin_unlock(&mtx_RDP1);
  write_unlock(&p_rdp->lock_RDPG);

  return rt_fadd;
}


void SMPs_update_work_components(RDPG_o *p_rdp)
{
  write_lock(&p_rdp->lock_RDPG);

  p_rdp->methods->update_work_components(p_rdp);

  write_unlock(&p_rdp->lock_RDPG);
}


void SMPs_load_vcomp_with_vcomp(RDPG_o *p_rdp, int p_vcomp_dst, int p_vcomp_src)
{
  write_lock(&p_rdp->lock_RDPG);

  p_rdp->methods->load_vcomp_with_vcomp(p_rdp, p_vcomp_dst, p_vcomp_src);

  write_unlock(&p_rdp->lock_RDPG);
}


int SMPs_rdpg_empty(RDPG_o *p_rdp)
{
  int rt_fempty;

  write_lock(&p_rdp->lock_RDPG);

  rt_fempty = rdpg_empty(p_rdp);

  write_unlock(&p_rdp->lock_RDPG);

  return rt_fempty;

}


int SMPs_get_TokensPlace(RDPG_o *p_rdp, char *p_entrada)
{
  int rt_fget; /* Retorno de funcion get_TokensPlace */

  write_lock(&p_rdp->lock_RDPG);

  rt_fget = get_TokensPlace(p_rdp, p_entrada);

  write_unlock(&p_rdp->lock_RDPG);

  return rt_fget;
}


int SMPs_get_vHDelement(RDPG_o *p_rdp, char *p_entrada)
{
  int rt_fget; /* Retorno de funcion get_vHDelement */

  write_lock(&p_rdp->lock_RDPG);

  rt_fget = get_vHDelement(p_rdp, p_entrada);

  write_unlock(&p_rdp->lock_RDPG);

  return rt_fget;
}



void SMPs_set_MemAllocMode(RDPG_o *p_rdp, char *c_mode)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->set_MemAllocMode(p_rdp, c_mode);

  write_unlock(&p_rdp->lock_RDPG);
}


void SMPs_set_select_comp(RDPG_o *p_rdp, int p_component)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->set_select_comp(p_rdp, p_component);

  write_unlock(&p_rdp->lock_RDPG);
}

void SMPs_set_read_mode(RDPG_o *p_rdp, ID_READ_MODE p_mode)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->set_read_mode(p_rdp, p_mode);

  write_unlock(&p_rdp->lock_RDPG);
}

void SMPs_set_read_smode(RDPG_o *p_rdp, ID_READ_SUBMODE p_smode)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->set_read_smode(p_rdp, p_smode);

  write_unlock(&p_rdp->lock_RDPG);
}

void SMPs_set_posVP(RDPG_o *p_rdp, char *p_entrada)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->set_posVP(p_rdp, p_entrada);

  write_unlock(&p_rdp->lock_RDPG);
}

void SMPs_set_posVT(RDPG_o *p_rdp, char *p_entrada)
{
  write_lock(&p_rdp->lock_RDPG);

  p_rdp->methods->set_posVT(p_rdp, p_entrada);
  
  write_unlock(&p_rdp->lock_RDPG);
}

void SMPs_set_vdim(RDPG_o *p_rdp, char *p_entrada)
{
  write_lock(&p_rdp->lock_RDPG);

  p_rdp->methods->set_vdim(p_rdp, p_entrada);
  
  write_unlock(&p_rdp->lock_RDPG);
}


void SMPs_set_catComp(RDPG_o *p_rdp, int p_comp)
{
  write_lock(&p_rdp->lock_RDPG);

  p_rdp->methods->set_catComp(p_rdp, p_comp);
  
  write_unlock(&p_rdp->lock_RDPG);
}


void SMPs_inc_vHQCV(RDPG_o *p_rdp, char *p_entrada)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->inc_vHQCV(p_rdp, p_entrada);

  write_unlock(&p_rdp->lock_RDPG);
}


void SMPs_dec_vHQCV(RDPG_o *p_rdp, char *p_entrada)
{
  write_lock(&p_rdp->lock_RDPG);
  
  p_rdp->methods->dec_vHQCV(p_rdp, p_entrada);

  write_unlock(&p_rdp->lock_RDPG);
}


int SMPs_read_rdpg_component(RDPG_o *p_rdp, char *p_kbuf, int p_comp, int p_read_mode)
{
  int rt_function; /* Retorno de funcion read_rdpg_component() */

  read_lock(&p_rdp->lock_RDPG);

  rt_function= p_rdp->methods->read_rdpg_component(p_rdp, p_kbuf, p_comp, p_read_mode);

  read_unlock(&p_rdp->lock_RDPG);

  return rt_function;
}


int SMPs_read_rdpg_info(RDPG_o *p_rdp, char *p_kbuf)
{
  int rt_function; /* Retorno de funcion read_rdpg_info() */

  read_lock(&p_rdp->lock_RDPG);

  rt_function= p_rdp->methods->read_rdpg_info(p_rdp, p_kbuf);

  read_unlock(&p_rdp->lock_RDPG);

  return rt_function;
}
