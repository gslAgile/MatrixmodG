/*------------------------------------------------------------------------------------------------------------------------------------------------------* 
 * Libreria matrixmodG_tests.h/.c (archivo fuente - source file).
 * 
 * Esta libreria contiene todas las pruebas unitarias e integrales del DDL MatrixmodG generadas con uso del framework KTF. Este framework permite la 
 * automatizacion de las pruebas haciendo el trabajo mas sencillo y eficiente.
 * 
 * El include a matrixmodG.c hace que este codigo sea un driver igual que matrixmodG.c pero con las declaraciones y definiciones de los test, separados
 * del codigo fuente funcional, de esta forma se testea modulo desde esta implementacion y se deja el modulo libre de los test cunado quede funcional con
 * todas las pruebas verificadas. Es decir que de esta forma se evita tener que contar con el codigo de test cuando no es necesario, pero se incorpora 
 * durante el periodo de pruebas para validar su funcionamiento.
 * 
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define MT_TEST_ON              /* Esta macro habilita a que el modulo entre en proceso de prueba con el framework KTF. */

#include "../matrixmodG.c"

#include "matrixmodG_tests.h"

/* Definicion de macros en MT (matrixmod_test) */
#define MT_MAX_TRANSICIONES 50 /* Macro que define el maximo de tranciones en un vector de prueba por MT (matrimod_test). */
#define MT_MAX_PLAZAS 50       /* Macro que define el maximo de plazas en un vector de prueba por MT (matrimod_test). */

/* Prototipo de funciones utilizadas en los casos de prueba */
void mt_verificar_mincidencia(void);
void mt_iniciar_rdpg_de_prueba(RDPG_o *p_rdp);
void mt_add_values_rdpg_components(RDPG_o *p_rdp);
void mt_add_values_MI(RDPG_o *p_rdp);
void mt_add_values_MH(RDPG_o *p_rdp);
void mt_add_values_MR(RDPG_o *p_rdp);
void mt_add_values_VMI(RDPG_o *p_rdp);
void mt_update_work_components(RDPG_o *p_rdp);
int mt_allComponents_created(RDPG_o *p_rdp);
int mt_allComponents_deleted(RDPG_o *p_rdp);
int mt_allComponents_inValue(RDPG_o *p_rdp, int p_value);
int mt_start_matrix_validV(matrix_o *, int p_value);
int mt_start_vector_validV(vector_o *, int p_value);
int mt_start_matrix_validZ(matrix_o *);
int mt_start_vector_validZ(vector_o *);
int mt_start_matrix_validI(matrix_o *);



/* Variablesglobales para Pruebas con KTF. 
 * gt: Todas las variables globales de test inician con gt para no confundir con el resto de variables manipuladas por el modulo.
 * lt: Son variables locales de test utilizadas dentro de cada funcion TEST asociada a un caso de prueba.
 */
const int gt_f=6, gt_c=4; /* variables para asignar filas y columnas a los test que lo requieran.*/
int gt_CP3_i = 0;
int gt_CP3_tamMax =1;
const char *gt_input1= "6_4 "; /* */

/* Comandos de entrada simulados, de valores a cargar en matriz de incidencia I (). */
char *gt_entradaMI[] = {
  "0_0_-1  ", "0_1_0  ", "0_2_0  ","0_3_1  ","1_0_1  ","1_1_0  ","1_2_0  ","1_3_-1  ","2_0_0  ","2_1_-1  ","2_2_1  ", "2_3_0  ",
  "3_0_0  ","3_1_1  ","3_2_-1  ","3_3_0  ","4_0_1  ","4_1_-1  ","4_2_0  ","4_3_0  ", "5_0_-1  ", "5_1_1  ","5_2_0  ", "5_3_0  "
};

/* Valores correctos que se validan con los ingresos de gt_entradaMI para la matriz de incidencia I (). */
int gt_valor_validoI[] = { -1, 0, 0, 1,
  1, 0, 0, -1,
  0, -1, 1, 0,
  0, 1, -1, 0,
  1, -1, 0, 0,
  -1, 1, 0, 0 };


/* Comandos de entrada simulados, de valores a cargar en matriz de incidencia H (). */
// char *gt_entradaMH[] = {
//   "0_0_0  ", "0_1_1  ", "0_2_0  ","0_3_0  ","1_0_0  ","1_1_0  ","1_2_0  ","1_3_0  ","2_0_0  ","2_1_0  ","2_2_0  ", "2_3_0  ",
//   "3_0_0  ","3_1_0  ","3_2_0  ","3_3_0  ","4_0_0  ","4_1_0  ","4_2_0  ","4_3_0  ", "5_0_0  ", "5_1_0  ","5_2_0  ", "5_3_0  "
// };
char *gt_entradaMH[] = {
  "0_0_0  ", "0_1_0  ", "0_2_0  ","0_3_0  ","1_0_0  ","1_1_0  ","1_2_0  ","1_3_0  ","2_0_0  ","2_1_0  ","2_2_0  ", "2_3_0  ",
  "3_0_0  ","3_1_0  ","3_2_0  ","3_3_0  ","4_0_0  ","4_1_0  ","4_2_0  ","4_3_0  ", "5_0_0  ", "5_1_0  ","5_2_0  ", "5_3_0  "
};

/* Valores correctos que se validan con los ingresos de gt_entradaMH para la matriz de incidencia H (). */
int gt_valor_validoH[] = { 0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0 };


/* Comandos de entrada simulados, de valores a cargar en matriz de incidencia R (CPIb, ). */
// char *gt_entradaMR[] = {
//   "0_0_0  ", "0_1_0  ", "0_2_0  ","0_3_0  ","1_0_0  ","1_1_0  ","1_2_0  ","1_3_0  ","2_0_0  ","2_1_0  ","2_2_0  ", "2_3_0  ",
//   "3_0_0  ","3_1_0  ","3_2_0  ","3_3_0  ","4_0_0  ","4_1_0  ","4_2_0  ","4_3_1  ", "5_0_0  ", "5_1_0  ","5_2_0  ", "5_3_0  "
// };
char *gt_entradaMR[] = {
  "0_0_0  ", "0_1_0  ", "0_2_0  ","0_3_0  ","1_0_0  ","1_1_0  ","1_2_0  ","1_3_0  ","2_0_0  ","2_1_0  ","2_2_0  ", "2_3_0  ",
  "3_0_0  ","3_1_0  ","3_2_0  ","3_3_0  ","4_0_0  ","4_1_0  ","4_2_0  ","4_3_0  ", "5_0_0  ", "5_1_0  ","5_2_0  ", "5_3_0  "
};

/* Valores correctos que se validan con los ingresos de gt_entradaMR para la matriz de incidencia R (). */
int gt_valor_validoR[] = { 0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0 };


/* Comandos de entrada simulados, de valores a cargar en vector de marcado inicial MI (). */
char *gt_entradaVMI[] = {
  "0_1  ", "1_0  ", "2_1  ","3_0  ","4_0  ","5_3  "
};

/* Valores corectos que se validan con los ingresos de gt_entradaVMI para el vector de marcado inicial MI (). */
int gt_valor_validoVMI[] = { 1, 0, 1, 0, 0, 3 };


/* Implementacion de casos de prueba organizando por bancos de pruebas.*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * PRUEBAS UNITARIAS
 *------------------------------------------------------------------------------------------------------------------------------------------------------*
 * Bancos de pruebas:
 * - BPU1_OperacionesAuxiliares
 * - BPU2_FuncionesEstructuraRDPG
 * - BPU3_FuncionesDinamicaRDP
 * - BPU4_FuncionesControlErroresEntrada
 * - BPU5_FuncionesExtraerDatos
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU1_OperacionesAuxiliares
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
// TEST(BPU1_OperacionesAuxiliares, CPU1_1_agregarValor_MI)
// {
//   /* Verificamos si la creacion de I fue exitosa (mc[0]=1). */
//   ASSERT_INT_EQ(RDP1.mII.mc, 1);

//   /* Agregacion de valores en matriz I. */
//   gt_CP3_tamMax= RDP1.mII.filas*RDP1.mII.columnas; /* Cantidad maxima de valores a agregar en matriz I.*/
  
//   if(gt_CP3_i < gt_CP3_tamMax){
//     EXPECT_INT_EQ(add_value_in_mcomponent(gt_entradaMI[gt_CP3_i], &(RDP1.mII)), gt_valor_validoI[gt_CP3_i]);
//     gt_CP3_i++;

//     /* Mensaje de finalizacion de caso de prueba. */
//     tlog(T_INFO, "Agregar valor %i en I. Se ejecuto correctamente.", gt_valor_validoI[gt_CP3_i-1]);
//   }
//   else{
//     /* Mensaje de finalizacion de caso de prueba. */
//     tlog(T_INFO, "Todos los valores estan agregados en matriz de incidencia I.");
//   }
// }


/**/
TEST(BPU1_OperacionesAuxiliares, CPU1_2_add_value_in_mcomponent)
{
  char kbuf[COMMANDSIZE]="RDP add P 1_2_3 \n"; /* simulacion de comando. */
  int lt_int1 = 0, lt_int2 = 0, lt_int3 = 0;
  char lt_entrada1[COMMANDSIZE], lt_entrada2[COMMANDSIZE], lt_entrada3[COMMANDSIZE];
  memset(lt_entrada1, '\0', COMMANDSIZE); // limpiamos entrada simulada 1
  memset(lt_entrada2, '\0', COMMANDSIZE); // limpiamos entrada simulada 2
  memset(lt_entrada3, '\0', COMMANDSIZE); // limpiamos entrada simulada 3
  strcpy(lt_entrada2, "100_200_300");
  strcpy(lt_entrada3, "1000_2000_3000");

  if( sscanf(kbuf,"add P %s", lt_entrada1) == 1)
  {
    tlog(T_INFO, "La entrada capturada es: \n- %s", lt_entrada1);
    //tlog(T_INFO, "Las entradas capturadas son: \n- %s \n- %s \n- %s", lt_entrada1, lt_entrada2, lt_entrada3);
    
    /* Captura de numeros enteros desde entrada*/
    //result_ent1 = sscanf(lt_entrada1,"%d_%*d_%*d", &lt_int1);
    //result_ent2 = sscanf(lt_entrada1,"%*d_%d_%*d", &lt_int2);
    //result_ent3 = sscanf(lt_entrada1,"%*d_%*d_%d", &lt_int3);
    //result_ent2 = sscanf(lt_entrada1,"%*s_%s_%*s", &lt_entrada2);

    if(extraer_fcv(&lt_int1, &lt_int2, &lt_int3, lt_entrada1) == 1)
    {
      tlog(T_INFO, "Los enteros capturados son: \n- %d \n- %d \n- %d", lt_int1, lt_int2, lt_int3);
      //tlog(T_INFO, "Los enteros capturados son: \n- %s \n- %s \n- %s", lt_entrada1, lt_entrada2, lt_entrada3);
    }

    if(extraer_fcv(&lt_int1, &lt_int2, &lt_int3, lt_entrada2) == 1)
    {
      tlog(T_INFO, "Los enteros capturados son: \n- %d \n- %d \n- %d", lt_int1, lt_int2, lt_int3);
      //tlog(T_INFO, "Los enteros capturados son: \n- %s \n- %s \n- %s", lt_entrada1, lt_entrada2, lt_entrada3);
    }

    if(extraer_fcv(&lt_int1, &lt_int2, &lt_int3, lt_entrada3) == 1)
    {
      tlog(T_INFO, "Los enteros capturados son: \n- %d \n- %d \n- %d", lt_int1, lt_int2, lt_int3);
      //tlog(T_INFO, "Los enteros capturados son: \n- %s \n- %s \n- %s", lt_entrada1, lt_entrada2, lt_entrada3);
    }

    else
    {
      tlog(T_INFO, "No se pudo capturar enteros.");
    }
  }
  else
  {
    tlog(T_INFO, "No se pudo capturar entrada.");
  }
}


/*
 * @brief El proposito de esta prueba es verificar si al agregar un valor en cada posición de la matriz de incidencia I, se valida que cada 
 * valor agregado es el valor correcto en su respectiva posición.
 */
TEST(BPU1_OperacionesAuxiliares, CPU1_3_agregarValores_MI)
{
  /* Verificamos si la creacion de I fue exitosa (mc[0]=1). */
  ASSERT_INT_EQ(RDP1.mII.mc, 1);

  /* Agregacion de valores en matriz I. */
  gt_CP3_tamMax= RDP1.mII.filas*RDP1.mII.columnas; /* Cantidad maxima de valores a agregar en matriz I.*/

  int li;
  for(li=0; li<gt_CP3_tamMax; li++)
  { 
    EXPECT_INT_EQ(RDP1.methods->add_value_in_mcomponent(&(RDP1.mII), gt_entradaMI[li]), gt_valor_validoI[li]);

    /* Mensaje de informacion de caso de prueba. */
    tlog(T_INFO, "Agregar valor %i con funcion add_value_in_mcomponent() en matriz I. Se ejecuto correctamente.", gt_valor_validoI[li]);
  }

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion add_value_in_mcomponent() al cargar matriz de incidencia I. Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU2_FuncionesEstructuraRDPG
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion create_rdpg_mcomponent().
 * Se verifica si la creacion de una matriz de incidencia I es correcta. Se verifica que todos sus valores sean inicializados a cero.
 * Al finalizar la verificacion se elimina la matriz creada para ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_1a_crear_mcomp)
{
  int lt_filas = gt_f, lt_col = gt_c;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);  

  /* Verifico si la creacion de I fue exitosa (mc=1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa (mc[0]=0). */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent(). Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion create_rdpg_vcomponent().
 * Se verifica si la creacion de un componente vector es correcta. Se verifica que todos sus valores sean inicializados a cero.
 * Al finalizar la verificacion se elimina el vector creado para ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_1b_crear_vcomp)
{
  int lt_elementos = gt_f;
  create_rdpg_vcomponent(&(RDP1.vMI), lt_elementos);

  /* Verifico si en la estructura del vector, se asigno el numero de elementos correctamente.*/
  ASSERT_INT_EQ(RDP1.vMI.elements, lt_elementos);

  /* Verifico si la creacion de vMI fue exitosa (mc=1). */
  ASSERT_INT_EQ(RDP1.vMI.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_vector_validZ(&(RDP1.vMI)),0);

  /* Elimino el vector vMI. */
  delete_rdpg_mcomponent(&(RDP1.vMI));

  /* Verifico si la eliminacion del vector vMI fue exitosa (mc[0]=0). */
  EXPECT_INT_EQ(RDP1.vMI.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_vcomponent(). Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief El proposito de esta prueba es verificar el correcto funcionamiento de la funcion create_rdpg().
 * Se verifica si al crear todos los componentes matrices y vectores asociados a una RDPG se realiza correctamente y se valida
 * que todos los valores de las matrices y vectores esten inicializados en cero.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_2a_crear_rdpg)
{
  //int lt_filas = gt_f, lt_col = gt_c;
  //create_rdpg(6,4); /* Iniciar la creacion de una Matriz de incidencia I con 6 filas y 4 columnas. */
  char lt_entrada[COMMANDSIZE];
  memset(lt_entrada, '\0', COMMANDSIZE); // limpiamos entrada de comando con filas y columnas
  strcpy(lt_entrada, gt_input1);

  create_rdpg(&RDP1,lt_entrada); /* Inicia la creacion de una nueva RDPG. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Valido si todos los valores de los componentes de la RDPG estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_allComponents_inValue(&RDP1, 0), 0);

  /* Valido si matriz disparos es la matriz identidad. */
  EXPECT_INT_EQ(mt_start_matrix_validI(&(RDP1.mD)), 0);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg(). Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief El proposito de esta prueba es verificar el correcto funcionamiento de la funcion create_rdpg() al crear dos veces una misma RDPG.
 * Se verifica crear una RDPG y luego de la corecta creacion intentar crear otra vez la RDPG. Lo correcto es que no se permita esta operacion y se
 * notifique adecuadamente del error ocacionado.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_2b_dobleCreacion_rdpg)
{
  char lt_entrada[COMMANDSIZE];
  memset(lt_entrada, '\0', COMMANDSIZE); // limpiamos entrada de comando con filas y columnas
  strcpy(lt_entrada, gt_input1);

  create_rdpg(&RDP1,lt_entrada); /* Inicia la creacion de una nueva RDPG. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Valido si todos los valores de los componentes de la RDPG estan inicializados en cero.*/
  create_rdpg(&RDP1,lt_entrada); /* Inicia la doble creacion de la RDPG. */  

  /* Analizo variable de ultimo error se asocia a una doble creacion de RDPG.*/
  EXPECT_INT_EQ(RDP1.error_code, -EC_dobleCreacionDeRDPG);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg(). Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief El proposito de esta prueba es verificar si al agregar un valor en cada posición de la matriz de incidencia I, se valida que cada 
 * valor agregado es el valor correcto en su respectiva posición.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_3_agregarValores_mII)
{
  /* Verificamos si la creacion de I fue exitosa (mc=1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Agregacion de valores en componente mII. */
  gt_CP3_tamMax= RDP1.mII.filas*RDP1.mII.columnas; /* Cantidad maxima de valores a agregar en componente matriz I.*/

  int li;
  for(li=0; li<gt_CP3_tamMax; li++)
  {
    EXPECT_INT_EQ(RDP1.methods->add_value_in_mcomponent(&(RDP1.mII), gt_entradaMI[li]), gt_valor_validoI[li]);

    /* Mensaje de informacion de caso de prueba. */
    tlog(T_INFO, "Agregar valor %i en componente mII. Se ejecuto correctamente.", gt_valor_validoI[li]);
  }

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion add_value_in_mcomponent() al cargar matriz de incidencia I. Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief El proposito de esta prueba es verificar si al agregar un valor en cada posición del vector de marcado inicial vMI, se valida que cada
 * valor agregado es el valor correcto en su respectiva posición.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_4_agregarValores_vMI)
{
  /* Verificamos si existe la matriz de incidencia I (mc[0]=1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Agregacion de valores en componente vector vMI. */
  gt_CP3_tamMax= RDP1.vMI.elements; /* Cantidad maxima de valores a agregar en vector vMI, equivale a la cantidad de plazas de la RdP. */

  int li;
  for(li=0; li<gt_CP3_tamMax; li++)
  {
    EXPECT_INT_EQ(RDP1.methods->add_value_in_vcomponent(&(RDP1.vMI), gt_entradaVMI[li]), gt_valor_validoVMI[li]);

    /* Mensaje de informacion de caso de prueba. */
    tlog(T_INFO, "Agregar valor %i en componente vMI. Se ejecuto correctamente.", gt_valor_validoVMI[li]);
  }

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion add_value_in_vcomponent() al cargar vector vMI. Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de la funcion de eliminar la RDPG que se encuentra cargada en el modulo. 
 * Se comprueba la eliminacion de cada una de los componentes matrices y vectores que se crearon para iniciar una RDPG.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_5a_eliminar_rdpg)
{
  delete_rdpg(&RDP1); /* Borramos la RDP (ya sea ordinaria o generalizada). */

  /* Verificamos si dejo de existir a nivel logico la RDPG. Comprobando si las marcas de creacion de matrices y vectores es cero*/
  ASSERT_INT_EQ(mt_allComponents_deleted(&RDP1) ,OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion delete_rdpg() de RDPG. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de la funcion de eliminar la RDPG que se encuentra cargada en el driver. 
 * Se comprueba si al eliminar dos veces la misma RDPG responde correctamente.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_5b_dobleEliminacion)
{
  char lt_entrada[COMMANDSIZE];
  memset(lt_entrada, '\0', COMMANDSIZE); // limpiamos entrada de comando con filas y columnas
  strcpy(lt_entrada, gt_input1);

  create_rdpg(&RDP1,lt_entrada); /* Inicia la creacion de una nueva RDPG. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Elimino la RDPG . */
  delete_rdpg(&RDP1); 

  /* Verificamos si dejo de existir a nivel logico la RDPG. Comprobando si las marcas de creacion de matrices y vectores es cero*/
  ASSERT_INT_EQ(mt_allComponents_deleted(&RDP1) ,OBJ_NO_CREATED);

  /* Realizo la doble eleiminacion de la RDPG.*/
  delete_rdpg(&RDP1); 

  /* Verifico si el codigo de error es el adecuado al error de deoble eliminacion de RDPG.*/
  EXPECT_INT_EQ(RDP1.error_code, -EC_dobleEliminacionDeRDPG);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion delete_rdpg() de RDPG. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica si al crear la matriz de incidencia I con la función create_rdpg_mcomponent() de una RDP con 10 plazas y 10 transiciones,
 * se asigna adecuadamente la memoria en una matriz de 10 filas y 10 columnas y si los valores de todos los elementos de la matriz están inicializados
 * en cero.
 * Se elimina la matriz creada para poder ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_6_crear_mII_10x10)
{
  int lt_filas = 10, lt_col = 10;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);

  /* Verifico si la creacion de I fue exitosa (mc==1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa. */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent() con matriz %dx%d. Se ejecuto correctamente.", lt_filas, lt_col);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba Verifica si al crear la matriz de incidencia I con la función create_rdpg_mcomponent() de una RDP con 100 plazas y 100 transiciones,
 * se asigna adecuadamente la memoria en una matriz de 100 filas y 100 columnas y si los valores de todos los elementos de la matriz están inicializados
 * en cero.
 * Se elimina la matriz creada para poder ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_7_crear_mII_100x100)
{
  int lt_filas = 100, lt_col = 100;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);

  /* Verifico si la creacion de I fue exitosa (mc==1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa (mc==0). */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent() con matriz %dx%d. Se ejecuto correctamente.", lt_filas, lt_col);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba Verifica si al crear la matriz de incidencia I con la función create_rdpg_mcomponent() de una RDP con 1000 plazas y 1000 transiciones,
 * se asigna adecuadamente la memoria en una matriz de 1000 filas y 1000 columnas y si los valores de todos los elementos de la matriz están inicializados
 * en cero.
 * Se elimina la matriz creada para poder ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_8_crear_mII_1000x1000)
{
  int lt_filas = 1000, lt_col = 1000;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);

  /* Verifico si la creacion de I fue exitosa (mc==1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa (mc==0). */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent() con matriz %dx%d. Se ejecuto correctamente.", lt_filas, lt_col);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica si al crear la matriz de incidencia I con la función create_rdpg_mcomponent() de una RDP con 5000 plazas y 5000 transiciones,
 * se asigna adecuadamente la memoria en una matriz de 4000 filas y 4000 columnas y si los valores de todos los elementos de la matriz están inicializados
 * en cero.
 * Se elimina la matriz creada para poder ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_9_crear_mII_4000x4000)
{
  int lt_filas = 4000, lt_col = 4000;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);

  /* Verifico si la creacion de I fue exitosa (mc==1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa (mc[0]=0). */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent() con matriz %dx%d. Se ejecuto correctamente.", lt_filas, lt_col);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica si al crear la matriz de incidencia I con la función create_rdpg_mcomponent() de una RDP con 5000 plazas y 5000 transiciones,
 * se asigna adecuadamente la memoria en una matriz de 5000 filas y 5000 columnas y si los valores de todos los elementos de la matriz están inicializados
 * en cero.
 * Se elimina la matriz creada para poder ejecutar el resto de las pruebas unitarias.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_10_crear_mII_5000x5000)
{
  int lt_filas = 5000, lt_col = 5000;
  create_rdpg_mcomponent(&(RDP1.mII), lt_filas, lt_col);

  /* Verifico si en la estructura de la matriz I, se asigno el numero de filas y columnas correctamente.*/
  ASSERT_INT_EQ(RDP1.mII.filas, lt_filas);
  ASSERT_INT_EQ(RDP1.mII.columnas, lt_col);

  /* Verifico si la creacion de I fue exitosa (mc==1). */
  ASSERT_INT_EQ(RDP1.mII.mc, OBJ_CREATED);

  /* Verifico si todos los valores estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_start_matrix_validZ(&(RDP1.mII)),0);

  /* Elimino la matriz de incidencia I. */
  delete_rdpg_mcomponent(&(RDP1.mII));

  /* Verifico si la eliminacion de I fue exitosa (mc[0]=0). */
  EXPECT_INT_EQ(RDP1.mII.mc, OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg_mcomponent() con matriz %dx%d. Se ejecuto correctamente.", lt_filas, lt_col);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief El proposito de esta prueba es verificar el correcto funcionamiento de la funcion create_rdpg().
 * Se verifica si al crear todas las matrices y vectores asociados a una RDP se realiza correctamente y se valida
 * que todos los valores de las matrices y vectores se inicien en cero.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU2_FuncionesEstructuraRDPG, CPU2_11_crear_rdpg_1000x1000)
{
  //int lt_filas = 1000, lt_col = 1000;
  //create_rdpg(6,4); /* Iniciar la creacion de una Matriz de incidencia I con 6 filas y 4 columnas. */
  
  char *lt_entrada = "1000_1000 ";

  create_rdpg(&RDP1,lt_entrada); /* Inicia la creacion de una Matriz de incidencia I con 6 filas y 4 columnas, con todos los valores por defecto en cero. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Validamos si todos los valores de los componentes de la RDPG estan inicializados en cero.*/
  EXPECT_INT_EQ(mt_allComponents_inValue(&RDP1, 0), 0);

  /* Valido si matriz disparos es la matriz identidad. */
  EXPECT_INT_EQ(mt_start_matrix_validI(&(RDP1.mD)), 0);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion create_rdpg() 1000x1000. Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU3_FuncionesDinamicaRDP
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de la funcion shoot_rdpg() en una RDP (generalizada u ordinaria).
 * Se verifica la creación de la RDP en el módulo antes de disparar.
 * Se verifica si se efectúa correctamente el disparo de la transición T0 de acuerdo a si se encuentra sensibilizada o no-sensibilizada.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU3_FuncionesDinamicaRDP, CPU3_1_RDPG_dispararT0)
{
  /* Se verifica existencia de RDP o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);


  if(RDP1.vEx.vector[id_T0]) /* Si T0 esta sensibilizada. Se realiza disparo de transicion considerandola como sensibilizada. */
  {
    /* Efectuamos disparo sobre la RDPG. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), 1); /* Disparo de transicion sensibilizada T0 en modo explicito.*/

    /* Verifico resultado exitoso de ultimo disparo T0. */
    EXPECT_INT_EQ(RDP1.vUDT.vector[id_T0], SHOT_OK);
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
  else /* Si T0 no esta sensibilizado. Se realiza disparo considerandolo como no-sensibilizado. */
  {
    /* Efectuamos disparo sobre la RDPO. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), -1); /* Disparo de transicion no-sensibilizada T0 en modo explicito.*/

    /* Verifico resultado fallido de ultimo disparo T0. */
    EXPECT_INT_EQ(RDP1.vUDT.vector[id_T0], SHOT_FAILED);
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion no-sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de la funcion shoot_rdpg() en una RDP (generalizada u ordinaria).
 * Se verifica la creación de la RDP en el módulo antes de disparar.
 * Se verifica si se efectúa correctamente el disparo de la transición T1 de acuerdo a si se encuentra sensibilizada o no-sensibilizada.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU3_FuncionesDinamicaRDP, CPU3_2_RDPG_dispararT1)
{
  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);


  if(RDP1.vEx.vector[id_T1]) /* Si T1 esta sensibilizada. Se realiza disparo de transicion considerandola como sensibilizada. */
  {
    /* Efectuamos disparo sobre la RDPO. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 1, SHOT_MODE_E), 1); /* Disparo de transicion sensibilizada T1 en modo explicito.*/

     /* Verifico resultado exitoso de ultimo disparo T1. */
    EXPECT_INT_EQ(RDP1.vUDT.vector[id_T1], SHOT_OK);
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion sensibilizada T%i en RDPG. Se ejecuto correctamente.", id_T1);
  }
  else /* Si T1 no esta sensibilizado. Se realiza disparo considerandolo como no-sensibilizado. */
  {
    /* Efectuamos disparo sobre la RDPG. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 1, SHOT_MODE_E), -1); /* Disparo de transicion no-sensibilizada T1 en modo explicito.*/

    /* Verifico resultado fallido de ultimo disparo T1. */
    EXPECT_INT_EQ(RDP1.vUDT.vector[id_T1], SHOT_FAILED);
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion no-sensibilizada T%i en RDPG. Se ejecuto correctamente.", id_T1);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de la funcion shoot_rdpg() en una RDP (generalizada u ordinaria).
 * Se verifica la creación de la RDP en el módulo antes de disparar.
 * Se verifica si se efectúa correctamente el disparo de la transición T1 de acuerdo a si se encuentra sensibilizada o no-sensibilizada.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU3_FuncionesDinamicaRDP, CPU3_3_RDPG_dispararTnoExistente)
{
  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Efectuamos disparo sobre la RDPO. */
  EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 1000, SHOT_MODE_E), -1); /* Disparo de transicion inexistente.*/

  /* Verifico si codigo de error es de transicion inexistente.*/
  EXPECT_INT_EQ(RDP1.error_code, -EC_transicionInexistente); 
  
  /* Mensaje de finalizacion de caso de prueba. */
  if(RDP1.error_code == -EC_transicionInexistente)
    tlog(T_INFO, "Se detecto error de Transicion inexistente exitosamente. La prueba ejecuto correctamente.");
  else
    tlog(T_INFO, "No se detecto error de Transicion inexistente. La prueba fallo.");
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica si la funcion de eliminar la RDP que se encuentra cargada en el modulo se realiza correctamente. Se comprueba la eliminacion de cada
 * una de las matrices y vectores que se crearon para iniciar una RDP.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU3_FuncionesDinamicaRDP, CPU3_4_RDPG_eliminar)
{
  delete_rdpg(&RDP1); /* Borramos la RDP (ya sea ordinaria o generalizada). */

  /* Verificamos si dejo de existir a nivel logico la RDPG. Comprobando si las marcas de creacion de matrices y vectores es cero*/
  ASSERT_INT_EQ(mt_allComponents_deleted(&RDP1) ,OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion delete_rdpg() de RDP. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU4_FuncionesControlErroresEntrada
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/

TEST(BPU4_FuncionesControlErroresEntrada, CPU4_1_control_errores_entradas_validas)
{
  /* Comandos de entradas simuladas, formato de cadena con numero de transicion. */
  char *lt_entrada_ntransicion[] = {
    "1  ", "10  ", "100  ","1000  ","1","10","100","1000"," 1  "," 10  "," 100  ", " 1000  ", "  1  ","  10  ","  100  ", "  1000  ",
    " 1"," 10"," 100", " 1000",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Comandos de entradas simuladas, formato de cadena de numeros de filas y columnas. */
  char *lt_entrada_fc[] = {
    "1_1  ", "1_10  ", "1_100  ","1_1000  ","10_1  ","10_10  ","10_100  ","10_1000  ","100_1  ","100_10  ","100_100  ", "100_1000  ",
    "1000_1  ","1000_10  ","1000_100  ","1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Comandos de entradas simuladas, formato de cadena de numeros de pos filas, pos columnas y valor. */
  char *lt_entrada_fcv[] = {
    "1_1_1  ", "1_1_10  ", "1_1_100  ", "1_1_1000  ", "1_10_1  ", "1_10_10  ", "1_10_100  ", "1_10_1000  ", "1_100_1  ", "1_100_10  ", "1_100_100  ",
    "1_100_1000  ", "1_1000_1  ", "1_1000_10  ", "1_1000_100  ", "1_1000_1000  ",
    "10_1_1  ", "10_1_10  ", "10_1_100  ", "10_1_1000  ", "10_10_1  ", "10_10_10  ", "10_10_100  ", "10_10_1000  ", "10_100_1  ", "10_100_10  ", "10_100_100  ",
    "10_100_1000  ", "10_1000_1  ", "10_1000_10  ", "10_1000_100  ", "10_1000_1000  ",
    "100_1_1  ", "100_1_10  ", "100_1_100  ", "100_1_1000  ", "100_10_1  ", "100_10_10  ", "100_10_100  ", "100_10_1000  ", "100_100_1  ", "100_100_10  ", "100_100_100  ",
    "100_100_1000  ", "100_1000_1  ", "100_1000_10  ", "100_1000_100  ", "100_1000_1000  ",
    "1000_1_1  ", "1000_1_10  ", "1000_1_100  ", "1000_1_1000  ", "1000_10_1  ", "1000_10_10  ", "1000_10_100  ", "1000_10_1000  ", "1000_100_1  ", "1000_100_10  ", "1000_100_100  ",
    "1000_100_1000  ", "1000_1000_1  ", "1000_1000_10  ", "1000_1000_100  ", "1000_1000_1000  ",
    "1_1_-1  ", "1_1_-10  ", "1_1_-100  ", "1_1_-1000  ", "1_10_-1  ", "1_10_-10  ", "1_10_-100  ", "1_10_-1000  ", "1_100_-1  ", "1_100_-10  ", "1_100_-100  ",
    "1_100_-1000  ", "1_1000_-1  ", "1_1000_-10  ", "1_1000_-100  ", "1_1000_-1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Verificacion de funcion controlador_errores_entrada_numeros() con diferentes entradas incrementando las cifras de numero 1 y numero 2 con diferentes
  entradas posibles lt_entrada_fc, e incrementando cifras de numero 1, numero 2 y numero 3 con diferentes combinaciones posibles en lt_entradafcv.*/
  int i;
  for(i=0; strcmp(lt_entrada_ntransicion[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_entrada_ntransicion[i],1), 1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter validas con formato de un solo numero' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_entrada_fc[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_entrada_fc[i], 2), 1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter validas con formato de dos numeros' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_entrada_fcv[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_entrada_fcv[i],3), 1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter validas con formato de tres numeros' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion controlador_errores_entrada_numeros() sobre diferentes entradas validas. Ejecuto exitosamente.\n");
}


TEST(BPU4_FuncionesControlErroresEntrada, CPU4_2_control_errores_entradas_no_validas)
{
  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_vacia[] = {
    " ", "  ", "   ", "    ", " 10_1  ", "10_ 1  ", " 10_100_1000  ", "10_ 100_ 1000", "1000 _10_100", "100_1000_ 10", "10_100 _1000", "1000 _100 _1",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_otro_formato[] = {
    "10#10  ", "10 10  ", "10/10   ", "10.10  ", "10,10  ", "10A10  ", "10|10  ", "10-10  ", "10+10  ", "10*10  ", "10<10  ", "10>10  ", "10=10  ",
    "10#10#1000  ", "10 10 1000 ", "10/10/1   ", "10.10.1000  ", "10,10,10  ", "10A10B100  ", "10|10|1  ", "10-10-100  ", "10+10+1  ", "10*10*11  ", "10<10<10  ", "10>10>100  ", "10=10=10  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_letras[] = {
    "A_B  ", "1_B  ", "10_C   ", "100_D  ", "1000_E  ", "A_1  ", "B_10  ", "C_100  ", "D_1000  ", "AA_BB  ",
    "a_b  ", "1_b  ", "10_c   ", "100_d  ", "1000_e  ", "a_1  ", "b_10  ", "c_100  ", "d_1000  ", "aa_bb  ",
    "A_B_C  ", "1_B_C  ", "10_C_D   ", "100_D_F  ", "1000_E_F  ", "A_1_1000  ", "B_10_1  ", "C_100_100  ", "D_1000_1  ", "AA_BB_CC  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_incompleta[] = { // pasa primera
    "1_  ", "10_  ", "100_   ", "1000_  ", "10__  ", "_1  ", "_10  ", "_100  ", "_1000 ", 
    "__  ", "___  ", "_", "_ ", "_   ", "_    ", "10_100_  ", "1_2_", "1_2_  ", "1_100_  ", "1_1000_  ", "10_1_  ", "10_10_  ", "10_100_ ", "10_1000_ ",
    "100_1_  ", "100_10_  ", "100_100_  ", "100_1000_  ", "1000_1_  ", "1000_10_  ", "1000_100_  ", "1000_1000_  ", "_1_1  ", "_1_10  ", "_1_100  ", "_1_1000",
    "_10_1  ", "_10_10  ", "_10_100  ", "_10_1000  ", "1__1  ", "1__10  ", "1__1000  ", "10__1  ", "10__1000",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_negativa[] = {
    "-1_10  ", "-10_10  ", "-100_10  ", "-1000_10  ", "10_-1  ", "10_-10  ", "10_-100  ", "10_-1000  "
    "-1_10_1  ", "-10_10_10  ", "-100_10_1000  ", "-1000_10_1  ", "10_-1_1  ", "10_-10_0  ", "10_-100_0  ", "10_-1000_1  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };


  /* Verificacion de funcion controlador_errores_entrada_numeros() con diferentes entradas incrementando las cifras de numero 1 y numero 2 con diferentes
  entradas posibles lt_entrada_fc, e incrementando cifras de numero 1, numero 2 y numero 3 con diferentes combinaciones posibles en lt_entradafcv.*/
  int i;
  for(i=0; strcmp(lt_cadena_vacia[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_cadena_vacia[i], 3), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter vacias' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_otro_formato[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_cadena_otro_formato[i], 3), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con otros formatos' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_letras[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_cadena_letras[i], 3), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con letras' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_incompleta[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_cadena_incompleta[i], 3), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter incompletas' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_negativa[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(controlador_errores_entrada_numeros(lt_cadena_negativa[i], 3), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con numeros negativos' sobre funcion controlador_errores_entrada_numeros(). Respondio exitosamente.\n", i);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion controlador_errores_entrada_numeros() sobre diferentes entradas no-validas. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU5_FuncionesExtraerDatos
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Verifica el correcto funcionamiento de la función extraer_enterox2() cuando los datos de las entradas de cadenas de carácter son correctos, de acuerdo
 * a diferentes entradas de cadenas de caracteres con el formato de entrada definido en la especificación de requerimientos. Se pone a prueba dos puntos:
 * - Que la extraccion del número de filas y el número de columnas sea el que se envía desde la cadena de caracteres. 
 * Por ejemplo: si la cadena es 10_500 la variable entera de filas deberá contener 10 mientras que la variable entera de columnas deberá contener 500.
 * - Que la respuesta de la función siga siendo válida para las diferentes combinaciones que pueden existir entre las cifras del número de filas y el
 * número de columnas.
 * Por ejemplo: si una cadena es 10_500 la respuesta de la función es como el ejemplo anterior, pero si la cadena cambia a 10_50, 10_5, 10_5000, 1_5,
 * 1_50. 1_500, 1_5000, 100_5, 100_50, etc. La función siempre debe responder correctamente para la extraccion del número de filas y el número de columnas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU5_FuncionesExtraerDatos, CPU5_1_extraer_enterox2_datos_validos)
{
  /* Comandos de entrada simulados, de numeros de filas y columnas para crear matrices y vectores. */
  char *lt_entrada_fc[] = {
    "1_1  ", "1_10  ", "1_100  ","1_1000  ","10_1  ","10_10  ","10_100  ","10_1000  ","100_1  ","100_10  ","100_100  ", "100_1000  ",
    "1000_1  ","1000_10  ","1000_100  ","1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Valores correctos del numero de filas de acuerdo a las entradas de caracteres dadas por lt_entrada_fc.  */
  int lt_num_filas_valido[] = { 1, 1, 1, 1,
    10, 10, 10, 10,
    100, 100, 100, 100,
    1000, 1000, 1000, 1000};

  /* Valores correctos del numero de columnas de acuerdo a las entradas de caracteres dadas por lt_entrada_fc.  */
  int lt_num_columnas_valido[] = { 1, 10, 100, 1000,
    1, 10, 100, 1000,
    1, 10, 100, 1000,
    1, 10, 100, 1000};

  /* Verificacion de funcion extraer_enterox2 con diferentes entradas incrementando las cifras de filas y columnas. */
  int i, lt_filas, lt_columnas;
  for(i=0; strcmp(lt_entrada_fc[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_entrada_fc[i]), 1);
    EXPECT_INT_EQ(lt_filas, lt_num_filas_valido[i]); /* Verifico si el numero de filas extraido es el que realmente deberia extraer. */
    EXPECT_INT_EQ(lt_columnas, lt_num_columnas_valido[i]); /* Verifico si el numero de columnas extraido es el que realmente deberia extraer. */
  }

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_enterox2() de entradas con datos validos. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Verifica el correcto funcionamiento de la función extraer_enterox2() cuando los datos de las entradas de cadenas de carácter son incorrectos,
 * de acuerdo a diferentes entradas de cadenas de caracteres que no respeten el formato de entrada definido en la especificación de requerimientos. 
 * Se pone a prueba que la función extraer_enterox2() responda correctamente ante el envío de cadenas de caracteres que no respeten el formato definido. Estas
 * cadenas de caracteres pueden ser:
 * - Cadenas de caracteres vacías.
 * - Cadenas de caracteres con otros formatos. Por ejemplo: 10-33.
 * - Cadenas de caracteres con letras respetando formatos. Por ejemplo A_B, 10_B, C_100.
 * - Cadenas incompletas. Por ejemplo 19_, _1, _, __, etc.
 * - Cadenas de caracteres con números negativos o ceros. Por ejemplo: 0_0, -10_10, 11_-40, -1_-2, etc.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU5_FuncionesExtraerDatos, CPU5_2_extraer_enterox2_datos_no_validos)
{
  /**/
  int lt_filas, lt_columnas;

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_vacia[] = {
    " ", "  ", "   ", "    ", " 10_1  ", "10__ 1  ", " 10___ 100  ", "10__100  ", "1000___10  ",
    " 10_1", "10__ 1", "10___ 100", "10__100", "1000___10",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_otro_formato[] = {
    "10#10  ", "10 10  ", "10/10   ", "10.10  ", "10,10  ", "10A10  ", "10|10  ", "10-10  ", "10+10  ", "10*10  ", "10<10  ", "10>10  ", "10=10  ",
    "1  ", "10  ", "100  ", "1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_letras[] = {
    "A_B  ", "1_B  ", "10_C   ", "100_D  ", "1000_E  ", "A_1  ", "B_10  ", "C_100  ", "D_1000  ", "AA_BB  ",
    "a_b  ", "1_b  ", "10_c   ", "100_d  ", "1000_e  ", "a_1  ", "b_10  ", "c_100  ", "d_1000  ", "aa_bb  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_incompleta[] = { // pasa primera
    "1_  ", "10_  ", "100_   ", "1000_  ", "1", "1 ", "1  ", "1   ", "1     ", "10  ", "100  ", "1000  ", "10__  ", "_1  ", "_10  ", "_100  ", "_1000 ", 
    "__  ", "___  ", "_", "_ ", "_   ", "_    ",
    "_10_1  ", "_10_10  ", "_10_100  ", "_10_1000  ", "1__1  ", "1__10  ", "1__1000  ", "10__1  ", "10__1000  ", "1___10  ",
    "1_", "10_", "100_", "1000_", "1", "1", "10", "100", "1000", "10__", "_1", "_10", "_100", "_1000", 
    "__", "___", "_",
    "_10_1", "_10_10", "_10_100", "_10_1000", "1__1", "1__10", "1__1000", "10__1", "10__1000", "1___10",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_negativa[] = {
    "-1_10  ", "-10_10  ", "-100_10  ", "-1000_10  ", "10_-1  ", "10_-10  ", "10_-100  ", "10_-1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_datos_extra[] = {
    "1_1_1  ", "1_10_10  ", "1_100_100  ","1_1000_1000  ", "10_1_1  ","10_10_10  ","10_100_100  ","10_1000_1000  ","100_1_1  ",
    "100_10_10  ","100_100_100  ", "100_1000_1000  ", "1000_1_1  ","1000_10_10  ","1000_100_100  ","1000_1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };


  /* Verificacion de funcion extraer_enterox2 con diferentes entradas no-validas. */
  int i;
  for(i=0; strcmp(lt_cadena_vacia[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_vacia[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter vacias' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_otro_formato[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_otro_formato[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con otros formatos' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);
  
  for(i=0; strcmp(lt_cadena_letras[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_letras[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con letras' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_incompleta[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_incompleta[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter incompletas' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_negativa[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_negativa[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con numeros negativos' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_datos_extra[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_enterox2(&lt_filas, &lt_columnas, lt_cadena_datos_extra[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de datos extra' sobre funcion extraer_enterox2(). Respondio exitosamente.\n", i);
  

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_enterox2() de entradas con datos no-validos. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Verifica el correcto funcionamiento de la función extraer_fcv() cuando los datos de las entradas de cadenas de carácter son correctos, de
 * acuerdo a diferentes entradas de cadenas de caracteres con el formato de entrada definido en la especificación de requerimientos. Se pone a prueba dos
 * puntos:
 * - Que la extracción del número de posición de fila, el número de posición de columna y que el número de valor (positivo o negativo) se corresponda con
 * el que se envía desde la cadena de caracteres. Tener en cuenta que valor puede ser un número negativo mientras que el número de filas o columnas no 
 * podrán ser negativos.
 * Por ejemplo: si la cadena es 10_500_1 la variable entera de posición de fila deberá contener 10 mientras que la variable entera de posición de columna
 * deberá contener 500 y por último la variable entera de valor, deberá contener 1.
 * - Que la respuesta de la función siga siendo válida para las diferentes combinaciones que pueden existir entre las cifras del número de posición de
 * fila, el número de posición de columna y el número de valor.
 * Por ejemplo: si una cadena es 10_500_1 la respuesta de la función es como el ejemplo anterior, pero si la cadena cambia a 10_50_1, 10_5_11, 10_5000_111,
 * 1_5_11, 1_50_1. 1_500_1, 1_5000_1, 100_5_11, 100_50_111, etc. La función siempre debe responder correctamente para la extraccion del número de filas y el
 * número de columnas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU5_FuncionesExtraerDatos, CPU5_3_extraer_fcv_datos_validos)
{
  /* Variables entereas donde se almacenan los datos de la cadena de caracteres. */
  int lt_posf, lt_posc, lt_valor;

  /* Comandos de entradas simulados, formato de cadena de numeros de pos filas, pos columnas y valor positivo. */
  char *lt_entrada_fcvPositivo[] = {
    "1_1_1  ", "1_1_10  ", "1_1_100  ", "1_1_1000  ", "1_10_1  ", "1_10_10  ", "1_10_100  ", "1_10_1000  ", "1_100_1  ", "1_100_10  ", "1_100_100  ",
    "1_100_1000  ", "1_1000_1  ", "1_1000_10  ", "1_1000_100  ", "1_1000_1000  ",
    "10_1_1  ", "10_1_10  ", "10_1_100  ", "10_1_1000  ", "10_10_1  ", "10_10_10  ", "10_10_100  ", "10_10_1000  ", "10_100_1  ", "10_100_10  ", "10_100_100  ",
    "10_100_1000  ", "10_1000_1  ", "10_1000_10  ", "10_1000_100  ", "10_1000_1000  ",
    "100_1_1  ", "100_1_10  ", "100_1_100  ", "100_1_1000  ", "100_10_1  ", "100_10_10  ", "100_10_100  ", "100_10_1000  ", "100_100_1  ", "100_100_10  ", "100_100_100  ",
    "100_100_1000  ", "100_1000_1  ", "100_1000_10  ", "100_1000_100  ", "100_1000_1000  ",
    "1000_1_1  ", "1000_1_10  ", "1000_1_100  ", "1000_1_1000  ", "1000_10_1  ", "1000_10_10  ", "1000_10_100  ", "1000_10_1000  ", "1000_100_1  ", "1000_100_10  ", "1000_100_100  ",
    "1000_100_1000  ", "1000_1000_1  ", "1000_1000_10  ", "1000_1000_100  ", "1000_1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Comandos de entradas simulados, formato de cadena de numeros de pos filas, pos columnas y valor negativo. */
  char *lt_entrada_fcvNegativo[] = {
    "1_1_-1  ", "1_1_-10  ", "1_1_-100  ", "1_1_-1000  ", "1_10_-1  ", "1_10_-10  ", "1_10_-100  ", "1_10_-1000  ", "1_100_-1  ", "1_100_-10  ", "1_100_-100  ",
    "1_100_-1000  ", "1_1000_-1  ", "1_1000_-10  ", "1_1000_-100  ", "1_1000_-1000  ",
    "10_1_-1  ", "10_1_-10  ", "10_1_-100  ", "10_1_-1000  ", "10_10_-1  ", "10_10_-10  ", "10_10_-100  ", "10_10_-1000  ", "10_100_-1  ", "10_100_-10  ", "10_100_-100  ",
    "10_100_-1000  ", "10_1000_-1  ", "10_1000_-10  ", "10_1000_-100  ", "10_1000_-1000  ",
    "100_1_-1  ", "100_1_-10  ", "100_1_-100  ", "100_1_-1000  ", "100_10_-1  ", "100_10_-10  ", "100_10_-100  ", "100_10_-1000  ", "100_100_-1  ", "100_100_-10  ", "100_100_-100  ",
    "100_100_-1000  ", "100_1000_-1  ", "100_1000_-10  ", "100_1000_-100  ", "100_1000_-1000  ",
    "1000_1_-1  ", "1000_1_-10  ", "1000_1_-100  ", "1000_1_-1000  ", "1000_10_-1  ", "1000_10_-10  ", "1000_10_-100  ", "1000_10_-1000  ", "1000_100_-1  ", "1000_100_-10  ", "1000_100_-100  ",
    "1000_100_-1000  ", "1000_1000_-1  ", "1000_1000_-10  ", "1000_1000_-100  ", "1000_1000_-1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  
  /* Valores correctos del numero de posicion de fila de acuerdo a las entradas de caracteres dadas por lt_entrada_fcvPositivo|Negativo.  */
  int lt_num_posf_valido[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };

  /* Valores correctos del numero de posicion de columna de acuerdo a las entradas de caracteres dadas por lt_entrada_fcvPositivo|Negativo.  */
  int lt_num_posc_valido[] = { 1, 1, 1, 1, 10, 10, 10, 10, 100, 100, 100, 100, 1000, 1000, 1000, 1000, 
    1, 1, 1, 1, 10, 10, 10, 10, 100, 100, 100, 100, 1000, 1000, 1000, 1000, 
    1, 1, 1, 1, 10, 10, 10, 10, 100, 100, 100, 100, 1000, 1000, 1000, 1000, 
    1, 1, 1, 1, 10, 10, 10, 10, 100, 100, 100, 100, 1000, 1000, 1000, 1000 };

  /* Valores correctos del numero de valor de acuerdo a las entradas de caracteres dadas por lt_entrada_fcvPositivo.  */
  int lt_num_vPositivo_valido[] = { 1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000,
    1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000,
    1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000, 
    1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000, 1, 10, 100, 1000 };

  /* Valores correctos del numero de valor de acuerdo a las entradas de caracteres dadas por lt_entrada_fcvNegativo.  */
  int lt_num_vNegativo_valido[] = { -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, 
    -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, 
    -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, 
    -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000, -1, -10, -100, -1000 };


  /* Verificacion de funcion extraer_fcv par entradas lt_entrada_fcvPositivo. */
  int i;
  for(i=0; strcmp(lt_entrada_fcvPositivo[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_entrada_fcvPositivo[i]), 1);
    ASSERT_INT_EQ(lt_posf, lt_num_posf_valido[i]);
    ASSERT_INT_EQ(lt_posc, lt_num_posc_valido[i]);
    ASSERT_INT_EQ(lt_valor, lt_num_vPositivo_valido[i]);
  }
  tlog(T_INFO, "Prueba de %d 'entradas validas con valor positivo' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);

  /* Verificacion de funcion extraer_fcv par entradas lt_entrada_fcvNegativo. */
  for(i=0; strcmp(lt_entrada_fcvNegativo[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_entrada_fcvNegativo[i]), 1);
    ASSERT_INT_EQ(lt_posf, lt_num_posf_valido[i]);
    ASSERT_INT_EQ(lt_posc, lt_num_posc_valido[i]);
    ASSERT_INT_EQ(lt_valor, lt_num_vNegativo_valido[i]);
  }
  tlog(T_INFO, "Prueba de %d 'entradas validas con valor negativo' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_fcv() de RDP. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Verifica el correcto funcionamiento de la función extraer_fcv() cuando los datos de las entradas de cadenas de carácter son incorrectos,
 * de acuerdo a diferentes entradas de cadenas de caracteres que no respeten el formato de entrada definido en la especificación de requerimientos.
 * Se pone a prueba que la función extraer_fcv() responda correctamente ante el envío de cadenas de caracteres que no respeten el formato definido.
 * Estas cadenas de caracteres pueden ser:
 * - Cadenas de caracteres vacías.
 * - Cadenas de caracteres con otros formatos. Por ejemplo: 10-33-10.
 * - Cadenas de caracteres con letras respetando formatos. Por ejemplo A_B_C, 10_B_D, C_100_V, Q_Z_10.
 * - Cadenas incompletas. Por ejemplo 19__10, _1_2, _, __,1_2_, etc.
 * - Cadenas de caracteres con números negativos. Por ejemplo: -10_10_100, 11_-40_10, -1_-2_-11, etc.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU5_FuncionesExtraerDatos, CPU5_4_extraer_fcv_datos_no_validos)
{
  /* Variables entereas donde se almacenan los datos de la cadena de caracteres. */
  int lt_posf, lt_posc, lt_valor;

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_vacia[] = {
    " ", "  ", "   ", "    ", " 10_100_1000  ", "10_ 100_ 1000", "1000 _10_100", "100_1000_ 10", "10_100 _1000", "1000 _100 _1",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_otro_formato[] = {
    "10#10#1000  ", "10 10 1000 ", "10/10/1   ", "10.10.1000  ", "10,10,10  ", "10A10B100  ", "10|10|1  ", "10-10-100  ", "10+10+1  ", "10*10*11  ", "10<10<10  ", "10>10>100  ", "10=10=10  ",
    "1_1  ", "10_10  ", "100_100  ", "1000_1000  ", "1  ", "10  ", "100  ", "1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_letras[] = {
    "A_B_C  ", "1_B_C  ", "10_C_D   ", "100_D_F  ", "1000_E_F  ", "A_1_1000  ", "B_10_1  ", "C_100_100  ", "D_1000_1  ", "AA_BB_CC  ",
    "10_10_C  ", "1_1_C  ", "100_100_D   ", "1000_1000_F  ", "1000_E_F  ", "A_1_1000  ", "B_10_1  ", "C_100_100  ", "D_1000_1  ", "AA_BB_CC  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_incompleta[] = { // pasa primera
    "10_100_  ", "1_2_", "1_2_  ", "1_100_  ", "1_1000_  ", "10_1_  ", "10_10_  ", "10_100_ ", "10_1000_ ",
    "100_1_  ", "100_10_  ", "100_100_  ", "100_1000_  ", "1000_1_  ", "1000_10_  ", "1000_100_  ", "1000_1000_  ", "_1_1  ", "_1_10  ", "_1_100  ", "_1_1000",
    "_10_1  ", "_10_10  ", "_10_100  ", "_10_1000  ", "1__1_  ", "1__10_  ", "1__1000  ", "10__1_  ", "10__1000_",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_negativa[] = {
    "-1_10_1  ", "-10_10_10  ", "-100_10_1000  ", "-1000_10_1  ", "10_-1_1  ", "10_-10_0  ", "10_-100_0  ", "10_-1000_1  ",
    "-1_-10_1  ", "-10_-10_10  ", "-100_-10_1000  ", "-1000_-10_1  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_datos_extra[] = {
    "1_1_1_1  ", "1_1_10_1  ", "1_1_100_1  ", "1_1_1000_1  ", "1_10_1_1  ", "1_10_10_1  ", "1_10_100_1  ", "1_10_1000_1  ", "1_100_1_1  ",
    "1_100_10_1  ", "1_100_100_1  ", "1_100_1000_1  ", "1_1000_1_1  ", "1_1000_10_1  ", "1_1000_100_1  ", "1_1000_1000_1  ",
    "10_1_1_10  ", "10_1_10_10  ", "10_1_100_10  ", "10_1_1000_10  ", "10_10_1_10  ", "10_10_10_10  ", "10_10_100_10  ", "10_10_1000_10  ",
    "10_100_1_10  ", "10_100_10_10  ", "10_100_100_10  ", "10_100_1000_10  ", "10_1000_1_10  ", "10_1000_10_10  ", "10_1000_100_10  ",
    "10_1000_1000_10  ",
    "100_1_1_100  ", "100_1_10_100  ", "100_1_100_100  ", "100_1_1000_100  ", "100_10_1_100  ", "100_10_10_100  ", "100_10_100_100  ",
    "100_10_1000_100  ", "100_100_1_100  ", "100_100_10_100  ", "100_100_100_100  ", "100_100_1000_100  ", "100_1000_1_100  ", "100_1000_10_100  ",
    "100_1000_100_100  ", "100_1000_1000_100  ",
    "1000_1_1_1000  ", "1000_1_10_1000  ", "1000_1_100_1000  ", "1000_1_1000_1000  ", "1000_10_1_1000  ", "1000_10_10_1000  ", "1000_10_100_1000  ",
    "1000_10_1000_1000  ", "1000_100_1_1000  ", "1000_100_10_1000  ", "1000_100_100_1000  ", "1000_100_1000_1000  ", "1000_1000_1_1000  ",
    "1000_1000_10_1000  ", "1000_1000_100_1000  ", "1000_1000_1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };
  

  /* Verificacion de funcion extraer_fcv para entradas no-validas. */
  int i;
  for(i=0; strcmp(lt_cadena_vacia[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_vacia[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas vacias' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);


  for(i=0; strcmp(lt_cadena_otro_formato[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_otro_formato[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas con otros formatos' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);


  for(i=0; strcmp(lt_cadena_letras[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_letras[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas con letras' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);


  for(i=0; strcmp(lt_cadena_incompleta[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_incompleta[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas incompletas' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);


  for(i=0; strcmp(lt_cadena_negativa[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_negativa[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas con posiciones negativas' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);  


  for(i=0; strcmp(lt_cadena_datos_extra[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_fcv(&lt_posf, &lt_posc, &lt_valor, lt_cadena_datos_extra[i]), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de cadenas con datos extra' sobre funcion extraer_fcv(). Respondio exitosamente.\n", i);  


  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_fcv() de RDP. Ejecuto exitosamente.\n");
}


/**/
TEST(BPU5_FuncionesExtraerDatos, CPU5_5_extraer_entero_input_valid)
{
  /* Variables entereas donde se almacenan el numero de trnasicion a extraer de la cadena de caracteres. */
  int lt_ntransicion;

  /* Comandos de entradas simulados, formato de cadena de numeros de transiciones. */
  char *lt_entrada_ntransicion[] = {
    "1  ", "10  ", "100  ", "1000  ", "1", "10", "100", "1000", 
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Valores correctos del numero de  transiciones de acuerdo a las entradas de caracteres dadas por lt_entrada_ntransicion.  */
  int lt_ntransicion_valido[] = { 1, 10, 100, 1000, 1, 10, 100, 1000 };

  /* Verificacion de funcion extraer_entero par entradas lt_entrada_ntransicion. */
  int i;
  for(i=0; strcmp(lt_entrada_ntransicion[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_entero(lt_entrada_ntransicion[i], &lt_ntransicion), 1);
    ASSERT_INT_EQ(lt_ntransicion, lt_ntransicion_valido[i]);
  }
  tlog(T_INFO, "Prueba de %d 'entradas validas con valores positivo' sobre funcion extraer_entero(). Respondio exitosamente.\n", i);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_entero() de RDP. Ejecuto exitosamente.\n");
}


/**/
TEST(BPU5_FuncionesExtraerDatos, CPU5_6_extraer_entero_input_invalid)
{
  /**/
  int lt_ntransicion;

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_vacia[] = {
    " ", "  ", "   ", "    ", " _1  ", " __100  ", "10_  ", "1000__ ", " 10_",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_letras[] = {
    "A  ", "AA  ", "AAA   ", "ABCD  ", " E  ", "  F  ", "A_  ", "_A  ", "D__  ",
    "a  ", "aa  ", "aaa   ", "abcd  ", " e  ", "  f  ", "a_  ", "_a  ", "d__  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_negativa[] = {
    "-1  ", "-10  ", "-100  ", "-1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };

  /* Entradas simuladas, de datos no validos. */
  char *lt_cadena_datos_extra[] = {
    "1_1  ", "1_10  ", "1_100  ","1_1000  ", "10_1  ","10_10  ","10_100  ","10_1000  ","100_1  ", "100_10  ","100_100  ", 
    "100_1000  ", "1000_1  ","1000_10  ","1000_100  ","1000_1000  ",
    "1_1_1  ", "1_10_10  ", "1_100_100  ","1_1000_1000  ", "10_1_1  ","10_10_10  ","10_100_100  ","10_1000_1000  ","100_1_1  ",
    "100_10_10  ","100_100_100  ", "100_1000_1000  ", "1000_1_1  ","1000_10_10  ","1000_100_100  ","1000_1000_1000  ",
    "" /* Esta ultima cadena nula, marca el fin del array. */
  };


  /* Verificacion de funcion extraer_enterox2 con diferentes entradas no-validas. */
  int i;
  for(i=0; strcmp(lt_cadena_vacia[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_entero(lt_cadena_vacia[i], &lt_ntransicion), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter vacias' sobre funcion extraer_entero(). Respondio exitosamente.\n", i);

  
  for(i=0; strcmp(lt_cadena_letras[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_entero(lt_cadena_letras[i], &lt_ntransicion), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con letras' sobre funcion extraer_entero(). Respondio exitosamente.\n", i);


  for(i=0; strcmp(lt_cadena_negativa[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_entero(lt_cadena_negativa[i], &lt_ntransicion), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de caracter con numeros negativos' sobre funcion extraer_entero(). Respondio exitosamente.\n", i);

  for(i=0; strcmp(lt_cadena_datos_extra[i], "") != 0; i++)
  {
    ASSERT_INT_EQ(extraer_entero(lt_cadena_datos_extra[i], &lt_ntransicion), -1);
  }
  tlog(T_INFO, "Prueba de %d 'entradas de datos extra' sobre funcion extraer_entero(). Respondio exitosamente.\n", i);
  

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion extraer_entero() de entradas con datos no-validos. Ejecuto exitosamente.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPU6_ConfiguracionDeParametros
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion set_MemAllocMode() cuando no existe cargada una RDPG en el kernel de Linux.
 * Se verifica que se realice correctamente el cambio del modo de asignacion de memoria mientras la RDPG del kernel no exista cargada en el kernel.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU6_ConfiguracionDeParametros, CPU6_1_setAllocMode_ok)
{
  if(RDP1.mc == OBJ_NO_CREATED)
  {
    /* Configuracion de M_KMALLOC.*/
    set_MemAllocMode(&RDP1, "1");
    EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");
  
    /*Configuracion de M_KZALLOC.*/
    set_MemAllocMode(&RDP1, "2");
    EXPECT_STREQ(RDP1.s_allocMode, "M_KZALLOC");
  
    /* Configuracion de M_KCALLOC.*/
    set_MemAllocMode(&RDP1, "3");
    EXPECT_STREQ(RDP1.s_allocMode, "M_KCALLOC");
  
    /* Configuracion de M_VMALLOC.*/
    set_MemAllocMode(&RDP1, "4");
    EXPECT_STREQ(RDP1.s_allocMode, "M_VMALLOC");
  
    /* Configuracion de M_VZALLOC.*/
    set_MemAllocMode(&RDP1, "5");
    EXPECT_STREQ(RDP1.s_allocMode, "M_VZALLOC");
  
    /* Configuracion de modo no reconocido.*/
    set_MemAllocMode(&RDP1, "6");
    EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");
  
    /* Configuracion de modo no reconocido.*/
    set_MemAllocMode(&RDP1, "102");
    EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");
  
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion set_MemAllocMode(). Ejecuto correctamente.");
  }
  else
  {
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion set_MemAllocMode(), FALLO. Se debe eliminar la RDPG cargada en el kernel.");
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion set_MemAllocMode() cuando ya existe cargada una RDPG en el kernel de Linux.
 * Se verifica que no se realice un cambio del modo de asignacion de memoria mientras exista una RDPG cargada en el kernel.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU6_ConfiguracionDeParametros, CPU6_2_setAllocMode_fail)
{
  /* Se verifica existencia de RDP o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);  

  /*Configuracion de M_KZALLOC. Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "2");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Configuracion de M_KCALLOC.Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "3");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Configuracion de M_VMALLOC.Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "4");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Configuracion de M_VZALLOC.Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "5");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Configuracion de modo no reconocido.Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "6");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Configuracion de modo no reconocido.Se mantiene M_KMALLOC. */
  set_MemAllocMode(&RDP1, "102");
  EXPECT_STREQ(RDP1.s_allocMode, "M_KMALLOC");

  /* Elimino RDPG. */
  delete_rdpg(&RDP1);

  /* Verificamos si dejo de existir a nivel logico la RDPG. */
  ASSERT_INT_EQ(RDP1.mc ,OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion set_MemAllocMode() con RDPG creada. Ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion set_posVP().
 * Se verifica que se realice correctamente el cambio del atributo posVP de la RDPG gestionada por el driver.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU6_ConfiguracionDeParametros, CPU6_3_setPosVP)
{
  /* Se verifica existencia de RDPG o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG de prueba por defecto con 6 plazas y 4 transiciones.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);
  
  /* Se verifica valor por defecto. */
  EXPECT_INT_EQ(RDP1.posVP, 0);

  /* Verifico si se realiza cambio de valor posVP, cuando el cambio es correcto. */
  set_posVP(&RDP1, "1");
  EXPECT_INT_EQ(RDP1.posVP, 1);
  set_posVP(&RDP1, "2");
  EXPECT_INT_EQ(RDP1.posVP, 2);
  set_posVP(&RDP1, "3");
  EXPECT_INT_EQ(RDP1.posVP, 3);
  set_posVP(&RDP1, "4");
  EXPECT_INT_EQ(RDP1.posVP, 4);
  set_posVP(&RDP1, "5");
  EXPECT_INT_EQ(RDP1.posVP, 5);

  /* Verifico si se realiza cambio de valor posVP, cuando el cambio no es correcto. Caso de ser negativo o mayor que el numero de plazas de la RDPG */
  set_posVP(&RDP1, "6");
  EXPECT_INT_EQ(RDP1.posVP, 5);
  set_posVP(&RDP1, "10");
  EXPECT_INT_EQ(RDP1.posVP, 5);
  set_posVP(&RDP1, "108");
  EXPECT_INT_EQ(RDP1.posVP, 5);
  set_posVP(&RDP1, "-1");
  EXPECT_INT_EQ(RDP1.posVP, 5);
  set_posVP(&RDP1, "-155");
  EXPECT_INT_EQ(RDP1.posVP, 5);

  /* Verifico codigo de error asociados a cambio de posVP. */
  EXPECT_INT_EQ(RDP1.error_code, -EC_falloPosVP);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion set_posVP(). Ejecuto correctamente.");

}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion set_posVT().
 * Se verifica que se realice correctamente el cambio del atributo posVT de la RDPG gestionada por el driver.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU6_ConfiguracionDeParametros, CPU6_4_setPosVT)
{
  /* Se verifica existencia de RDPG o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG de prueba por defecto con 6 plazas y 4 transiciones.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);
  
  /* Se verifica valor por defecto. */
  EXPECT_INT_EQ(RDP1.posVT, 0);

  /* Verifico si se realiza cambio de valor posVT, cuando el cambio es correcto. */
  set_posVT(&RDP1, "1");
  EXPECT_INT_EQ(RDP1.posVT, 1);
  set_posVT(&RDP1, "2");
  EXPECT_INT_EQ(RDP1.posVT, 2);
  set_posVT(&RDP1, "3");
  EXPECT_INT_EQ(RDP1.posVT, 3);

  /* Verifico si se realiza cambio de valor posVT, cuando el cambio no es correcto. Caso de ser negativo o mayor que el numero de plazas de la RDPG */
  set_posVT(&RDP1, "4");
  EXPECT_INT_EQ(RDP1.posVT, 3);
  set_posVT(&RDP1, "10");
  EXPECT_INT_EQ(RDP1.posVT, 3);
  set_posVT(&RDP1, "108");
  EXPECT_INT_EQ(RDP1.posVT, 3);
  set_posVT(&RDP1, "-1");
  EXPECT_INT_EQ(RDP1.posVT, 3);
  set_posVT(&RDP1, "-155");
  EXPECT_INT_EQ(RDP1.posVT, 3);

  /* Verifico codigo de error asociados a cambio de posVT. */
  EXPECT_INT_EQ(RDP1.error_code, -EC_falloPosVT);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion set_posVT(). Ejecuto correctamente.");

}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba verifica el correcto funcionamiento de las funcion set_posVT().
 * Se verifica que se realice correctamente el cambio del atributo posVT de la RDPG gestionada por el driver.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPU6_ConfiguracionDeParametros, CPU6_5_setVdim)
{
  /* Se verifica existencia de RDPG o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG de prueba por defecto con 6 plazas y 4 transiciones.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);
  
  /* Se verifica valor por defecto. */
  EXPECT_INT_EQ(RDP1.vdim, MIN_VDIM);

  /* Verifico si se realiza cambio de valor vdim, cuando el cambio es correcto. */
  set_vdim(&RDP1, "11");
  EXPECT_INT_EQ(RDP1.vdim, 11);
  set_vdim(&RDP1, "20");
  EXPECT_INT_EQ(RDP1.vdim, 20);
  set_vdim(&RDP1, "30");
  EXPECT_INT_EQ(RDP1.vdim, 30);

  /* Verifico si se realiza cambio de valor vdim, cuando el cambio no es correcto. Caso de ser negativo o mayor que el numero de plazas de la RDPG */
  set_vdim(&RDP1, "4");
  EXPECT_INT_EQ(RDP1.vdim, 30);
  set_vdim(&RDP1, "31");
  EXPECT_INT_EQ(RDP1.vdim, 30);
  set_vdim(&RDP1, "108");
  EXPECT_INT_EQ(RDP1.vdim, 30);
  set_vdim(&RDP1, "-1");
  EXPECT_INT_EQ((int)RDP1.vdim, 30);
  set_vdim(&RDP1, "-155");
  EXPECT_INT_EQ((int)RDP1.vdim, 30);

  /* Verifico codigo de error asociados a cambio de vdim. */
  EXPECT_INT_EQ(RDP1.error_code, -EC_falloVdim);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion set_posVT(). Ejecuto correctamente.");

}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * PRUEBAS DE INTEGRACION
 *------------------------------------------------------------------------------------------------------------------------------------------------------
 * Bancos de pruebas:
 * - BPI1_RDPG_dispararS0
 * - BPI2_RDPG_DispararT0ConHilos
 * - BPI3_FuncionesGetsSets
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPI1_RDPG_dispararS0
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief Esta prueba dispara una secuencia de disparos sobre la red de petri ordinaria cargada por la prueba CPI1a_RDPG_crear, durante la ejecucion de cada
 * disparo se verifica si cada marcado nuevo es el correcto a traves de un conjunto de validaciones que lo comprueban.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
// TEST(BPI1_RDPG_dispararS0, CPI1_1_RDPG_dispararS0)
// {
//   int li, lj;
//   /* Creamos secuencia de disparos. */
//   int l_S0[4] = { 0, 1, 3, 2};
//   int lt_n_disparos = 4; /* numero de disparos. */

//   /* Se verifica existencia de RDPG o sino se crea. */
//   mt_iniciar_rdpg_de_prueba(); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

//   /* Creamos los vectores de marcado nuevo correctos asociados a cada disparo. Cuando el marcado inicial es     {1, 0, 1, 0, 0, 3}*/
//   // int *l_VMN_valido[] = {
//   int l_VMN_valido[4][6] = {
//     {0, 1, 1, 0, 1, 2},
//     {0, 1, 0, 1, 0, 3},
//     {1, 0, 0, 1, 0, 3},
//     {1, 0, 1, 0, 0, 3}};


//   /* Verificamos si existen las matrices y vectores utlizados al disparar en la RDP. */
//   ASSERT_INT_EQ(I.mc, 1);
//   ASSERT_INT_EQ(MI.mc, 1);
//   ASSERT_INT_EQ(MA.mc, 1);
//   ASSERT_INT_EQ(MN.mc, 1);
//   ASSERT_INT_EQ(disparos.mc, 1);

//   for(li=0; li<lt_n_disparos; li++)
//   {
//     /* La transicion a disparar esta sensibilizada ? */
//     ASSERT_INT_EQ((int)(E.vector[l_S0[li]]), 1);

//     /*Realizamos disparos sobre RdP y validamos si se realizo exitosamente. */
//     ASSERT_INT_EQ(shoot_rdpg(l_S0[li], SHOT_MODE_E), 1);
    
//     /*Verificamos si el marcado nuevo es el esperado. */
//     for(lj=0; lj<MN.elements; lj++)
//     {
//       EXPECT_INT_EQ((int)(vMN.vector[lj]), l_VMN_valido[li][lj]);
//     }
    
//     tlog(T_INFO, "Disparo t%i de secuencia S0 en RDPO. Se ejecuto correctamente.", l_S0[li]);
//   }

//   /* Mensaje de finalizacion de caso de prueba. */
//   tlog(T_INFO, "Todos los disparos de secuencia S0 en RDPO han tenido exito. Se validaron todos los estados nuevos asociados a cada disparo correctamente.");
// }


/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPI1_RDPG_dispararS0
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*
*/
TEST(BPI1_RDPG_dispararS0, CPI1_1_RDPG_dispararS0)
{
  int li, lj;
  /* Creamos secuencia de disparos. */
  //char *lt_S0[] = { " 0 ", " 3 ", " 0 ", " 1 ", " 2 ", " 1 "}; /* Este vector indica el numero de transicion a disparar. */
  int lt_S0[6] = {0, 3, 0, 1, 2, 1};
  int lt_n_disparos = 6; /* numero de disparos. */
  //int lt_ntransicion;

  /* Creamos los vectores de marcado nuevo correctos asociados a cada disparo. Cuando el marcado inicial es     {1, 0, 1, 0, 0, 3}*/
  // int *l_VMN_valido[] = {
  int lt_VMN_valido[6][6] = {
    {0, 1, 1, 0, 1, 2},
    {1, 0, 1, 0, 1, 2},
    {0, 1, 1, 0, 2, 1},
    {0, 1, 0, 1, 1, 2},
    {0, 1, 1, 0, 1, 2},
    {0, 1, 0, 1, 0, 3}
  };

  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Se trata de una RDPG? Todos los componentes de la RDPG existen? */
  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /* Realizamos la secuencia de disparos. Equivalente a ejecutar la ecuacion de estado extendida. */
  for(li=0; li<lt_n_disparos; li++)
  {
    /* La transicion a disparar esta sensibilizada ? */
    //(void)extraer_entero(lt_S0[li], &lt_ntransicion); /* casteamos a void para ignorar el return entero (int). */
    //ASSERT_INT_EQ((int)(RDP1.vE.vector[lt_ntransicion]), 1);

    /*Realizamos disparos sobre RdP y validamos si se realizo exitosamente. */
    ASSERT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, lt_S0[li], SHOT_MODE_E), 1);
    
    /*Verificamos si el marcado nuevo es el esperado. */
    for(lj=0; lj<RDP1.vMN.elements; lj++)
    {
      EXPECT_INT_EQ((int)(RDP1.vMN.vector[lj]), lt_VMN_valido[li][lj]);
    }
    
    tlog(T_INFO, "Disparo t%d de secuencia S0 en RDP. Se ejecuto correctamente.", lt_S0[li]);
  }

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Todos los disparos de secuencia S0 en RDPG han tenido exito. Se validaron todos los estados nuevos asociados a cada disparo correctamente.");
}


/**/
TEST(BPI1_RDPG_dispararS0, CPI1_2_RDPG_eliminar)
{
  delete_rdpg(&RDP1); /* Elimino la RDPG. */

  /* Verificamos si dejo de existir a nivel logico la RDPG. Comprobando si las marcas de creacion de matrices y vectores es cero*/
  ASSERT_INT_EQ(mt_allComponents_deleted(&RDP1) ,OBJ_NO_CREATED);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Ejecuto exitosamente. Se eliminaron exitosamente todos los componentes de la RDPG.\n");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPI2_RDPG_DispararT0ConHilos
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define NUM_TEST_THREADS 4

struct ktf_thread test_threads[NUM_TEST_THREADS]; /* Vector de hilos de prueba. */

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba de hilo verifica si al disparar la transicion T0 (sensibilizada o no-sensibilizada) con la funcion shoot_rdpg()
 * tiene un retorno correcto. Como la prueba puede ejecutarse por diferentes hilos que prueban el funcionamiente de la funcion
 * shoot_rdpg() exponiendo la funcion a la concuerrencia de hilos/subprocesos, en este caso como la funcion no tiene soporte de
 * SMP-secure lo mas probable es que la prueba no tenga exito algunas veces.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
KTF_THREAD(test_hilo_disparaT0)
{
  /* Efectuamos disparo sobre la RDP. */
  RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E); /* Disparo de transicion sensibilizada T0 en modo explicito.*/

  /* Mensaje de finalizacion de caso de prueba. */
   tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion T%i en RDP. Se ejecuto correctamente.", id_T0);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba de hilo verifica si al disparar la transicion T0 (sensibilizada o no-sensibilizada) con la funcion SMPs_shoot_rdpg()
 * tiene un retorno correcto. Como la prueba puede ejecutarse por diferentes hilos que prueban el funcionamiente de la funcion
 * SMPs_shoot_rdpg() exponiendo la funcion a la concuerrencia de hilos/subprocesos, en este caso como la funcion tiene soporte de
 * SMP-secure lo mas probable es que la prueba siempre tenga exito.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
KTF_THREAD(test_hilo_SMPs_disparaT0)
{
  /* Efectuamos disparo sobre la RDP con soporte SMPs. */
  RDP1.methods->SMPs_shoot_rdpg(&RDP1, 0, SHOT_MODE_E); /* Disparo de transicion sensibilizada T0 en modo explicito.*/

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion SMPs_shoot_rdpg() para disparo de transicion T%i en RDP. Se ejecuto correctamente.", id_T0);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba de hilo verifica antes de disparar si la transicion T0 esta sensibilizada, cosa que hace la funcion
 * shoot_rdpg(), pero se repite dicha condicion con el fin de probar si los hilos ingresan incorrectamente a una misma sentencia
 * al manejar mal el uso del recurso compartido RDPG_o.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
KTF_THREAD(test_hilo_disparaT0_sensandoEX)
{
  if(RDP1.vEx.vector[id_T0]) /* Si T0 esta sensibilizada. Se realiza disparo de transicion considerandola como sensibilizada. */
  {
    /* Efectuamos disparo sobre la RDP. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), SHOT_OK); /* Disparo de transicion sensibilizada T0 en modo explicito.*/
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
  else /* Si T0 no esta sensibilizado. Se realiza disparo considerandolo como no-sensibilizado. */
  {
    /* Efectuamos disparo sobre la RDPO. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), SHOT_FAILED); /* Disparo de transicion no-sensibilizada T0 en modo explicito.*/
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion no-sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba de hilo verifica antes de disparar si la transicion T0 esta sensibilizada, cosa que hace la funcion
 * shoot_rdpg(), pero se repite dicha condicion con el fin de probar si los hilos ingresan incorrectamente a una misma sentencia
 * al manejar mal el uso del recurso compartido RDPG_o. En este caso como ai proteccion de recurso con spinlocks siempre se gestiona
 * correctamente el recurso compartido.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
KTF_THREAD(test_hilo_SMPs_disparaT0_sensandoEX)
{
  //spin_lock(&mtx_RDP1);
  write_lock(&RDP1.lock_RDPG);
  if(RDP1.vEx.vector[id_T0]) /* Si T0 esta sensibilizada. Se realiza disparo de transicion considerandola como sensibilizada. */
  {
    /* Efectuamos disparo sobre la RDP. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), SHOT_OK); /* Disparo de transicion sensibilizada T0 en modo explicito.*/
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
  else /* Si T0 no esta sensibilizado. Se realiza disparo considerandolo como no-sensibilizado. */
  {
    /* Efectuamos disparo sobre la RDPO. */
    EXPECT_INT_EQ(RDP1.methods->shoot_rdpg(&RDP1, 0, SHOT_MODE_E), SHOT_FAILED); /* Disparo de transicion no-sensibilizada T0 en modo explicito.*/
    
    /* Mensaje de finalizacion de caso de prueba. */
    tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() para disparo de transicion no-sensibilizada T%i en RDP. Se ejecuto correctamente.", id_T0);
  }
  //spin_unlock(&mtx_RDP1);
  write_unlock(&RDP1.lock_RDPG);
}



TEST(BPI2_RDPG_DispararT0ConHilos, CPI2_0_RDPG_crearRDP)
{
  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  tlog(T_INFO, "Creacion de RDP. Se ejecuto correctamente.");
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba inicia la creacion y asignacion de la RDP de prueba y utiliza dos hilos/subprocesos para disparar paralelamente
 * la transicion T0 mediante la ejecucion de la prueba por hilos KTF_THREAD(test_hilo_disparaT0).
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPI2_RDPG_DispararT0ConHilos, CPI2_1_RDPG_DispararT0xHilos)
{
  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /**/
  KTF_THREAD_INIT(test_hilo_disparaT0, &test_threads[0]);
  KTF_THREAD_INIT(test_hilo_disparaT0, &test_threads[1]);

  /**/
  KTF_THREAD_RUN(&test_threads[1]);
  KTF_THREAD_RUN(&test_threads[0]);

  /**/
  KTF_THREAD_WAIT_COMPLETED(&test_threads[0]);
  KTF_THREAD_WAIT_COMPLETED(&test_threads[1]);

  tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() de transicion T0 en RDP con %d hilos paralelamente. Se ejecuto correctamente.", NUM_TEST_THREADS);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba inicia la creacion y asignacion de la RDP de prueba y utiliza dos hilos/subprocesos para disparar paralelamente
 * la transicion T0 con proteccion SMPs mediante la ejecucion de la prueba por hilos KTF_THREAD(test_hilo_SMPs_disparaT0).
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPI2_RDPG_DispararT0ConHilos, CPI2_2_RDPG_SMPs_DispararT0xHilos)
{
  //int i;

  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);
  
  /**/
  KTF_THREAD_INIT(test_hilo_SMPs_disparaT0, &test_threads[0]);
  KTF_THREAD_INIT(test_hilo_SMPs_disparaT0, &test_threads[1]);

  /**/
  KTF_THREAD_RUN(&test_threads[1]);
  KTF_THREAD_RUN(&test_threads[0]);

  /**/
  KTF_THREAD_WAIT_COMPLETED(&test_threads[0]);
  KTF_THREAD_WAIT_COMPLETED(&test_threads[1]);

  /*for (i = 0; i < NUM_TEST_THREADS; i++) {
    KTF_THREAD_INIT(test_hilo_disparaT0_sensandoEX, &test_threads[i]);
    KTF_THREAD_RUN(&test_threads[i]);
  }
  for (i = 0; i < NUM_TEST_THREADS; i++)
    KTF_THREAD_WAIT_COMPLETED(&test_threads[i]);*/


  tlog(T_INFO, "Prueba sobre funcion SMPs_shoot_rdpg() de transicion T0 en RDP con %d hilos paralelamente. Se ejecuto correctamente.", NUM_TEST_THREADS);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba inicia la creacion y asignacion de la RDP de prueba y utiliza dos hilos/subprocesos para disparar paralelamente
 * la transicion T0 mediante la ejecucion de la prueba por hilos KTF_THREAD(test_hilo_disparaT0_sensandoEX).
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPI2_RDPG_DispararT0ConHilos, CPI2_3_RDPG_DispararT0xHilos_sensandoEX)
{
  //int i;

  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /**/
  KTF_THREAD_INIT(test_hilo_disparaT0_sensandoEX, &test_threads[0]);
  KTF_THREAD_INIT(test_hilo_disparaT0_sensandoEX, &test_threads[1]);

  /**/
  KTF_THREAD_RUN(&test_threads[1]);
  KTF_THREAD_RUN(&test_threads[0]);

  /**/
  KTF_THREAD_WAIT_COMPLETED(&test_threads[0]);
  KTF_THREAD_WAIT_COMPLETED(&test_threads[1]);

  /*for (i = 0; i < NUM_TEST_THREADS; i++) {
    KTF_THREAD_INIT(test_hilo_disparaT0_sensandoEX, &test_threads[i]);
    KTF_THREAD_RUN(&test_threads[i]);
  }
  for (i = 0; i < NUM_TEST_THREADS; i++)
    KTF_THREAD_WAIT_COMPLETED(&test_threads[i]);*/


  tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() de transicion T0 en RDP con %d hilos paralelamente. Se ejecuto correctamente.", NUM_TEST_THREADS);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief    Esta prueba inicia la creacion y asignacion de la RDP de prueba y utiliza dos hilos/subprocesos para disparar paralelamente
 * la transicion T0 mediante la ejecucion de la prueba por hilos KTF_THREAD(test_hilo_SMPs_disparaT0_sensandoEX).
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
TEST(BPI2_RDPG_DispararT0ConHilos, CPI2_4_RDPG_SMPs_DispararT0xHilos_sensandoEX)
{
  //int i;

  /* Se verifica existencia de RDPG o sino se crea. */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de todos los componentes de la RDPG.*/
  ASSERT_INT_EQ(mt_allComponents_created(&RDP1), OBJ_CREATED);

  /**/
  KTF_THREAD_INIT(test_hilo_SMPs_disparaT0_sensandoEX, &test_threads[0]);
  KTF_THREAD_INIT(test_hilo_SMPs_disparaT0_sensandoEX, &test_threads[1]);

  /**/
  KTF_THREAD_RUN(&test_threads[1]);
  KTF_THREAD_RUN(&test_threads[0]);

  /**/
  KTF_THREAD_WAIT_COMPLETED(&test_threads[0]);
  KTF_THREAD_WAIT_COMPLETED(&test_threads[1]);

  /*for (i = 0; i < NUM_TEST_THREADS; i++) {
    KTF_THREAD_INIT(test_hilo_disparaT0_sensandoEX, &test_threads[i]);
    KTF_THREAD_RUN(&test_threads[i]);
  }
  for (i = 0; i < NUM_TEST_THREADS; i++)
    KTF_THREAD_WAIT_COMPLETED(&test_threads[i]);*/


  tlog(T_INFO, "Prueba sobre funcion shoot_rdpg() de transicion T0 en RDP con %d hilos paralelamente. Se ejecuto correctamente.", NUM_TEST_THREADS);
}



/*---------------------------------------------------------------------------------------------------------------------------------------------------------*
 * BANCO DE PRUEBA: BPI3_FuncionesGetsSets
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Esta prueba verifica el correcto funcionamiento de la funcion get_TokensPlace() cuando existe creada una RDPG en el kernel testeando los tokens de
 * las plazas de una RDPG previamente cargada y conocida. */
TEST(BPI3_FuncionesGetsSets, CPI3_1_getTokensPlace)
{
  /* Variables enterea donde se almacena el numero de la posicion de plaza a extraer de la cadena de caracteres. */
  int lt_pos_e;

  /* Se verifica existencia de RDPG o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG de prueba por defecto con 6 plazas y 4 transiciones.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);
  
  /* Se verifica valor por defecto. */
  EXPECT_INT_EQ(RDP1.posVP, 0);


  /* Verifica retorno de tokens de plaza 0. Siendo gt_valor_validoVMI[] = { 1, 0, 1, 0, 0, 3 }; */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "0"), gt_valor_validoVMI[0]);

  /* Verifica retorno de tokens de plaza 1. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "1"), gt_valor_validoVMI[1]);

  /* Verifica retorno de tokens de plaza 2. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "2"), gt_valor_validoVMI[2]);

  /* Verifica retorno de tokens de plaza 3. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "3"), gt_valor_validoVMI[3]);

  /* Verifica retorno de tokens de plaza 4. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "4"), gt_valor_validoVMI[4]);

  /* Verifica retorno de tokens de plaza 5. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "5"), gt_valor_validoVMI[5]);


  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba sobre funcion get_TokensPlace() de RDPG. Ejecuto exitosamente.\n");
}


/* Esta prueba verifica el correcto funcionamiento de la funcion get_TokensPlace() ante su respuesta a errores. Se verifican los siguientes errores: 
  - Dato incorrecto.
  - Error en extraccion de datos. 
  - Error por componente no creado. 
*/
TEST(BPI3_FuncionesGetsSets, CPI3_1b_getTokensPlace_fail)
{
  /* Variables enterea donde se almacena el numero de la posicion de plaza a extraer de la cadena de caracteres. */
  int lt_pos_e;

  /* Se verifica existencia de RDPG o sino se crea RDPG con modo de reserva por defecto (KMALLOC). */
  mt_iniciar_rdpg_de_prueba(&RDP1); /* se inicia RDPG de prueba, si no esta creada. No se realizan comprobaciones en esta funcion ya que no es el proposito de la prueba. */

  /* Verifica la creacion de RDPG de prueba por defecto con 6 plazas y 4 transiciones.*/
  ASSERT_INT_EQ(RDP1.mc, OBJ_CREATED);
  
  /* Se verifica valor por defecto. */
  EXPECT_INT_EQ(RDP1.posVP, 0);


  /* Verifica retorno de tokens de plaza 0. Siendo gt_valor_validoVMI[] = { 1, 0, 1, 0, 0, 3 }; */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "6"), -EC_datoIncorrecto);

  /* Verifica retorno de tokens de plaza inexistente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "7"), -EC_datoIncorrecto);

  /* Verifica retorno de tokens de plaza inexistente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "22"), -EC_datoIncorrecto);

  /* Verifica retorno de tokens de plaza inexistente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "100"), -EC_datoIncorrecto);

  /* Verifica retorno de tokens de plaza inexistente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "1000"), -EC_datoIncorrecto);

  /* Verifica retorno de tokens de plaza inexistente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "-1"), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, ""), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "a"), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, " "), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, " _1"), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "_"), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "1_"), -EC_extraccionDato);

  /* Verifica retorno de get_TokensPlace() cuando falla extraccion. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "1_1"), -EC_extraccionDato);

  /* Elimino la RDPG . */
  delete_rdpg(&RDP1); 
  
  /* Verifica retorno de get_TokensPlace() cuando no existe componente. Siendo gt_valor_validoVMI[] = { 1, 0, 1, 0, 0, 3 }; */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "0"), -EC_componenteNoCreado);

  /* Verifica retorno de get_TokensPlace() cuando no existe componente. */
  ASSERT_INT_EQ(RDP1.methods->get_TokensPlace(&RDP1, "1"), -EC_componenteNoCreado);


  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "Prueba de errores sobre funcion get_TokensPlace() de RDPG. Ejecuto exitosamente.\n");
}



/*------------------------------------------------------------------------------------------------------------------------------------------------------* 
 * Implementacion de funciones para proceso de prueba
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Funcion que adiciona los casos de prueba a cada banco de prueba para ser utilizados con KTF en nuestro modulo.*/
extern void add_tests(void)
{
  /* Banco de prueba: BPU1_OperacionesAuxiliares */
  // ADD_TEST(CPU1_1_agregarValor_MI);
  // ADD_TEST(CPU1_2_add_value_in_mcomponent);
  // ADD_TEST(CPU1_3_agregarValores_MI);

  /* Banco de prueba: BPU2_FuncionesEstructuraRDPG */
  ADD_TEST(CPU2_1a_crear_mcomp);
  ADD_TEST(CPU2_1b_crear_vcomp);
  ADD_TEST(CPU2_2a_crear_rdpg);
  ADD_TEST(CPU2_2b_dobleCreacion_rdpg);
  ADD_TEST(CPU2_3_agregarValores_mII);
  ADD_TEST(CPU2_4_agregarValores_vMI);
  ADD_TEST(CPU2_5a_eliminar_rdpg);
  ADD_TEST(CPU2_5b_dobleEliminacion);
  ADD_TEST(CPU2_6_crear_mII_10x10);
  ADD_TEST(CPU2_7_crear_mII_100x100);
  ADD_TEST(CPU2_8_crear_mII_1000x1000);
  ADD_TEST(CPU2_9_crear_mII_4000x4000);
  ADD_TEST(CPU2_10_crear_mII_5000x5000);
  ADD_TEST(CPU2_11_crear_rdpg_1000x1000);

  /* Banco de prueba: BPU3_FuncionesDinamicaRDP */
  ADD_TEST(CPU3_1_RDPG_dispararT0);
  ADD_TEST(CPU3_2_RDPG_dispararT1);
  ADD_TEST(CPU3_3_RDPG_dispararTnoExistente);
  ADD_TEST(CPU3_4_RDPG_eliminar);

  /* Banco de prueba: BPU4_FuncionesControlErroresEntrada */
  ADD_TEST(CPU4_1_control_errores_entradas_validas);
  ADD_TEST(CPU4_2_control_errores_entradas_no_validas);

  /* Banco de prueba: BPU5_FuncionesExtraerDatos */
  ADD_TEST(CPU5_1_extraer_enterox2_datos_validos);
  ADD_TEST(CPU5_2_extraer_enterox2_datos_no_validos);
  ADD_TEST(CPU5_3_extraer_fcv_datos_validos);
  ADD_TEST(CPU5_4_extraer_fcv_datos_no_validos);
  ADD_TEST(CPU5_5_extraer_entero_input_valid);
  ADD_TEST(CPU5_6_extraer_entero_input_invalid);

  /* Banco de prueba: BPU6_ConfiguracionDeParametros*/
  ADD_TEST(CPU6_1_setAllocMode_ok);
  ADD_TEST(CPU6_2_setAllocMode_fail);
  ADD_TEST(CPU6_3_setPosVP);
  ADD_TEST(CPU6_4_setPosVT);
  ADD_TEST(CPU6_5_setVdim);

  /* Banco de prueba: BPI1_RDPG_dispararS0 */
  ADD_TEST(CPI1_1_RDPG_dispararS0);
  ADD_TEST(CPI1_2_RDPG_eliminar);

  /* Banco de prueba: BPI2_RDPG_DispararT0ConHilos*/
  ADD_TEST(CPI2_0_RDPG_crearRDP);
  ADD_TEST(CPI2_1_RDPG_DispararT0xHilos);
  ADD_TEST(CPI2_2_RDPG_SMPs_DispararT0xHilos);
  ADD_TEST(CPI2_3_RDPG_DispararT0xHilos_sensandoEX);
  ADD_TEST(CPI2_4_RDPG_SMPs_DispararT0xHilos_sensandoEX);

  /* Banco de prueba: BPI3_FuncionesGets. */
  ADD_TEST(CPI3_1_getTokensPlace);
  ADD_TEST(CPI3_1b_getTokensPlace_fail);

}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de iniciar una RDPG por defecto que se utiliza para realizar diferentes pruebas, si no fue iniciada previamente.
 * Iniciar una RDPG equivale a crear los diferentes componentes de una RDPG (matrices y vectores asociados a la red), y de cargar los diferentes
 * valores que se corresponde a cada uno de los componentes.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_iniciar_rdpg_de_prueba(RDPG_o *p_rdp)
{
  if(p_rdp->mII.mc==0)
  {
    //int lt_filas = gt_f, lt_col = gt_c;
    char lt_entrada[COMMANDSIZE];
    memset(lt_entrada, '\0', COMMANDSIZE); // limpiamos entrada de comando con filas y columnas
    strcpy(lt_entrada, gt_input1);

    create_rdpg(p_rdp,lt_entrada); /* Inicia la creacion de una Matriz de incidencia I con n (lt_filas) filas y m (lt_col) columnas, con todos los valores por defecto en cero. */

    /* Agregar valores de componentes de la RDPG. */
    mt_add_values_rdpg_components(p_rdp);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de agregar los valores a cada uno de los componentes de una RDPG.
 * Agrupa las funciones de carga de las matrices de incidencia I, H y R y el vector de marcado inicial VMI, que son todos los componentes
 * necesarios cargar para representar la estructura de una RDPG y dar su estado inicial.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_add_values_rdpg_components(RDPG_o *p_rdp)
{
  /* Cargo valores de matriz de incidencia I. */
  mt_add_values_MI(p_rdp);
  
  /* Cargo valores de matriz de incidencia H. */
  mt_add_values_MH(p_rdp);

  /* Cargo valores de matriz de incidencia R. */
  mt_add_values_MR(p_rdp);

  /* Cargo valores de vector de marcado inicial VMI. */
  mt_add_values_VMI(p_rdp);

  /* Actualizo valores de componentes de trbajo asociados a RDPG. */
  mt_update_work_components(p_rdp);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga actualizar los vectores de trabajo asociados a una RDPG (ya sea una RDPO o RDPG aplica para ambos)
 * Se actualizan todos los componentes de la RDPG que son utilizados para determinar un nuevo estado de la red como lo son:
 * - Vector E de transiciones sensibilizadas E.
 * - Vector W de la funcion uno sobre los tokens de cada plaza.
 * - Vector Q de la funcion cero sobre los tokens de cada plaza.
 * - Vector B de transiciones des-sensibilizadas por arco ihibidor.
 * - Vector L de transiciones des-sensibilizadas por arcor lector.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_update_work_components(RDPG_o *p_rdp)
{
  update_vE(p_rdp);
  update_vW(p_rdp);
  update_vQ(p_rdp);
  update_vB(p_rdp);
  update_vL(p_rdp);
  update_vEx(p_rdp);
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de agregar todos los valores de la matriz de incidencia I de acuerdo al caso de prueba que 
 * se encuentre cargado en la variable global gt_entradaMI, que es uno de los componentes de la RDP sobre la cual se realizan las pruebas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_add_values_MI(RDPG_o *p_rdp)
{
  /* Agregacion de valores en matriz I. */
  int lt_tamMax= (p_rdp->mII.filas)*(p_rdp->mII.columnas); /* Cantidad maxima de valores a agregar en matriz I.*/

  int li;
  for(li=0; li<lt_tamMax; li++)
  {
    /* Agrego valor a traves de comandos simulados en gt_entradaMI. */
    (void)RDP1.methods->add_value_in_mcomponent(&(p_rdp->mII), gt_entradaMI[li]);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de agregar todos los valores de la matriz de incidencia H de acuerdo al caso de prueba que 
 * se encuentre cargado en la variable global gt_entradaMH, que es uno de los componentes de la RDP sobre la cual se realizan las pruebas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_add_values_MH(RDPG_o *p_rdp)
{
  /* Agregacion de valores en matriz H. */
  int lt_tamMax= p_rdp->mII.filas*p_rdp->mII.columnas; /* Cantidad maxima de valores a agregar en matriz H.*/

  int li;
  for(li=0; li<lt_tamMax; li++)
  {
    /* Agrego valor a traves de comandos simulados en gt_entradaMH. */
    //add_value_in_mcomponent(, &(p_rdp->mIH)); gt_entradaMH[li]
    (void)RDP1.methods->add_value_in_mcomponent(&(p_rdp->mIH), gt_entradaMH[li]);

  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de agregar todos los valores de la matriz de incidencia R de acuerdo al caso de prueba que 
 * se encuentre cargado en la variable global gt_entradaMR, que es uno de los componentes de la RDP sobre la cual se realizan las pruebas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_add_values_MR(RDPG_o *p_rdp)
{
  /* Agregacion de valores en matriz R. */
  int lt_tamMax= p_rdp->mII.filas*p_rdp->mII.columnas; /* Cantidad maxima de valores a agregar en matriz R.*/

  int li;
  for(li=0; li<lt_tamMax; li++)
  {
    /* Agrego valor a traves de comandos simulados en gt_entradaMR. */
    //add_value_in_mcomponent(gt_entradaMR[li], &(p_rdp->mIR));
    (void)RDP1.methods->add_value_in_mcomponent(&(p_rdp->mIR), gt_entradaMR[li]);
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion se encarga de agregar todos los valores en el vector de marcado inicial VMI de acuerdo al caso de prueba que
 * se encuentre cargado en la variable global gt_entradaVMI, que es uno de los componentes de la RDP sobre la cual se realizan las pruebas.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_add_values_VMI(RDPG_o *p_rdp)
{
  /* Agregacion de valores en vector de marcado inicial VMI. */
  int lt_tamMax= p_rdp->mII.filas; /* Cantidad maxima de valores a agregar en vector VMI, equivale a la cantidad de plazas de la RdP. */

  int li;
  for(li=0; li<lt_tamMax; li++)
  {
    /* Agrego valor a traves de comandos simulados en gt_entradaVMI. */
    //add_value_in_mcomponent(gt_entradaVMI[li], &RDP1.vMI);
    (void)RDP1.methods->add_value_in_vcomponent(&(p_rdp->vMI), gt_entradaVMI[li]);
  }

  /* Se actualiza vector de marcado actual de acuerdo al vector de marcado inicial, como primer estador actual de partida.*/
  p_rdp->methods->load_vcomp_with_vcomp(p_rdp, vMA, vMI);
}



/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si todos los componentes matrices y vectores de una RDPG estan creados chequeando su bandera mc (marca de creacion).
 *
 * @param      p_rdp  Puntero a la direccion de memoria de la RDPG sobre la que se desea verificar si sus componentes existen.
 *
 * @return     El retorno es:
 *              - OBJ_CREATED: Si todos los componentes de la RDPG existen.
 *              - OBJ_NO_CREATED: Si alguno de los componentes de la RDPG no existe.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_allComponents_created(RDPG_o *p_rdp)
{
  size_t i;

  /* Verifico bandera mc en componentes matrix_o. */
  for(i=0; i< N_MCOMPONENTS; i++)
  {
    if(p_rdp->mcomponent[i]->mc != OBJ_CREATED)
      return OBJ_NO_CREATED;
  }

  /* Verifico bandera mc en componentes vector_o. */
  for(i=0; i< N_VCOMPONENTS; i++)
  {
    if(p_rdp->vcomponent[i]->mc != OBJ_CREATED)
      return OBJ_NO_CREATED;
  }

  return OBJ_CREATED;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si todos los componentes matrices y vectores de una RDPG estan eliminados chequeando su bandera mc (marca de creacion).
 *
 * @param      p_rdp  Puntero a la direccion de memoria de la RDPG sobre la que se desea verificar si sus componentes no existen.
 *
 * @return     El retorno es:
 *              - OBJ_CREATED: Si todos los componentes de la RDPG existen.
 *              - OBJ_NO_CREATED: Si alguno de los componentes de la RDPG no existe.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_allComponents_deleted(RDPG_o *p_rdp)
{
  size_t i;

  /* Verifico bandera mc en componentes matrix_o. */
  for(i=0; i< N_MCOMPONENTS; i++)
  {
    if(p_rdp->mcomponent[i]->mc != OBJ_NO_CREATED)
      return OBJ_CREATED;
  }

  /* Verifico bandera mc en componentes vector_o. */
  for(i=0; i< N_VCOMPONENTS; i++)
  {
    if(p_rdp->vcomponent[i]->mc != OBJ_NO_CREATED)
      return OBJ_CREATED;
  }

  return OBJ_NO_CREATED;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si todos los componentes de una RDPG tienen inicializados sus valores al valor enviado por parametro.
 *
 * @param      p_rdp    Puntero a la direccion de memoria sobre la que se desea realizar la verificacion.
 * @param[in]  p_value  Valor que se desea verificar en todos los elementos de todos los componentes de una RDPG.
 *
 * @return     El retorno es:
 *              - 0: Si todos los elementos de los componentes de una RDPG estan inicializados en p_value.
 *              - -1: Si alguno de los elementos de cualquier componente de una RDPG no esta inicializado en p_value.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_allComponents_inValue(RDPG_o *p_rdp, int p_value)
{
  size_t i;

  /* Verifico elementos de componentes matrix_o. */
  for(i=0; i< N_MCOMPONENTS; i++)
  {
    if(p_rdp->mcomponent[i]->obj_id != mD) /* Mientras id componente matrix_o sea distinto de id matriz disparos (mD). */
    {
      if(mt_start_matrix_validV(p_rdp->mcomponent[i], p_value) != 0)
        return -1;
    }
  }

  /* Verifico elementos de componentes vector_o. */
  for(i=0; i< N_VCOMPONENTS; i++)
  {
    if(p_rdp->vcomponent[i]->obj_id != vG)
    {
        if(mt_start_vector_validV(p_rdp->vcomponent[i], p_value) != 0)
          return -1;
    }
  }

  return 0;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si todos los elementos de un componente matriz estan inicializados en el valor p_value enviado por parametro.
 *
 * @param      <p_mo>  Puntero a la direccion de memoria del objeto matrix_o sobre el que se desea realizar la verificacion.
 *
 * @return     El retorno es:
 *              - 0: Si todos los elementos, de la matriz enviada por parametros, estan inicializados en p_value.
 *              - -1: Si alguno de los elementos de la matriz enviada por parametros no esta inicializado en p_value.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_start_matrix_validV(matrix_o *p_mo, int p_value)
{
  size_t li, lj; /* local i y j*/

  for(li=0; li<(p_mo->filas); li++){
    for(lj=0; lj<(p_mo->columnas); lj++){
      if(p_mo->matriz[li][lj] != p_value)
        return -1;
    }
  }
  return 0;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si todos los elementos, de un componente vector, estan inicializados en el valor p_value enviado por parametro.
 *
 * @param      <p_vo>  Puntero a la direccion de memoria del objeto vector_o sobre el que se desea realizar la verificacion.
 *
 * @return     El retorno es:
 *              - 0: Si todos los elementos, del vector enviado por parametro, estan inicializados en p_value.
 *              - -1: Si alguno de los elementos del vector enviado por parametros no esta inicializado en p_value.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_start_vector_validV(vector_o *p_vo, int p_value)
{
  size_t li; /* local i. */

  for(li=0; li<(p_vo->elements); li++){
      if(p_vo->vector[li] != p_value)
        return -1;
  }
  return 0;
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion verifica si estan creadas las matrices de incidencia I, H y R de una RDPG.
 * @note       mt: son las siglas de matrixmod_test, estas se utiliza en todas las funciones propias del presente
 * modulo matrixmod_test.c que testea a matrixmod.c. Se utiliza este formato de nombre para evitar problemas de
 * nombres con las funciones implementadas en el modulo matrixmod.c
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mt_verificar_mincidencia(void)
{

}


/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si la inicializacion de una matriz enviada por parametro fue exitosa, verificando que
 * todos sus valores sean 0, al validar la inicializacion en cero es por eso su nombre Z al final de la funcion.
 *
 * @param      p_m   Es un puntero de una estructura de matriz definida en matrices.h, es el parametro que nos indica la
 * matriz a validar sus valores.
 *
 * @return     El retorno es:
 *              + 0: Si todos los valores se encuentran inicializados correctamente en cero.
 *              + -1: Si alguno de los valores de la matriz indicada por parametro no se encuentra con todos sus
 *              valores inicializados en cero.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_start_matrix_validZ(matrix_o * p_m)
{
  int li, lj; /* local i y j*/
  for(li=0; li<(p_m->filas); li++){
    for(lj=0; lj<(p_m->columnas); lj++){
      if(p_m->matriz[li][lj]!=0)
        return -1;
    }
  }
  return 0;
}


int mt_start_vector_validZ(vector_o * p_vo)
{
  int li; /* local i. */
  for(li=0; li<(p_vo->elements); li++){
      if(p_vo->vector[li]!=0)
        return -1;
  }
  return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*
 * @brief      Esta funcion valida si la inicializacion de una matriz identidad enviada por parametro fue exitosa, verificando que
 * todos sus valores i=j sean 1 y el resto 0. Al validar la inicializacion en una matriz identidad es por eso su nombre I al final de la funcion.
 *
 * @param      p_m   Es un puntero de una estructura de matriz definida en matrices.h, es el parametro que nos indica la
 * matriz a validar sus valores.
 *
 * @return     El retorno es:
 *              + 0: Si todos los valores se encuentran inicializados correctamente de acuerdo a una matriz identidad.
 *              + -1: Si alguno de los valores de la matriz indicada por parametro no se encuentra con todos sus
 *              valores inicializados de acuerdo a una matriz identidad.
 *------------------------------------------------------------------------------------------------------------------------------------------------------*/
int mt_start_matrix_validI(matrix_o * p_m)
{
  int li, lj; /* local i y j*/
  for(li=0; li<(p_m->filas); li++){
    for(lj=0; lj<(p_m->columnas); lj++){
      if(li==lj)
      {
        if(p_m->matriz[li][lj]!=1){
          return -1;}
      }
      else
      {
        if(p_m->matriz[li][lj]!=0){
          return -1;}
      }
    }
  }
  return 0;
}