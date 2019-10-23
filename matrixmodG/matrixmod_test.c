/* Esto hace que este codigo sea un driver igual que matrixmod.c pero con las implementaciones de los test
 * definidos de manera separada para poder testear el modulo desde esta implementacion y dejar el modulo libre
 * de los test cundo que funcional con todas las pruebas verificadas. Es decir se evita tener que contar con el
 * codigo de test cuando no es necesario.
 */
#include "../matrixmod.c"


/* Variablesglobales para Pruebas con KTF. 
 * gt: Todas las variables globales de test inician con gt para no confundir con el resto de variables manipuladas por el modulo.
 */
int gt_f, gt_c; /* variables para asignar filas y columnas a los test que lo requieran.*/
char gt_vaux[15] = "valor ";
char gt_faux[15] = "fila ";
char gt_caux[15] = "columna ";
char *gt_entradaMI[] = {
  "0_0_-1", "0_1_0", "0_2_0","0_3_1","1_0_1","1_1_0","1_2_0","1_3_1","2_0_0","2_1_-1","2_2_1", "2_3_0",
  "3_0_0","3_1_1","3_2_-1","3_3_0","4_0_1","4_1_-1","4_2_0","4_3_0", "5_0_-1", "5_1_1","5_2_0", "5_3_0"
};

/* Implementacion de casos de prueba organizando por bancos de pruebas.*/
/*Pruebas unitarias: */
TEST(MatrixmodTestRF, CP_crear_I)
{
  gt_f = 6;
  gt_c = 4;
  crear_rdp(&gt_f, &gt_c, &I, mIncidenciaI); /* 0: hace referencia a mc[0] para detectar que se creo
                                    una matriz en referencia a esa posicion, en este caso I */

  /* Verificamos si la creacion de I fue exitosa (mc[0]=1). */
  EXPECT_INT_EQ(mc[mIncidenciaI], 1);

  /* Validamos si todos los valores estan inicializados en cero.*/


  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "CP: Crear I. Se ejecuto correctamente.");
}

TEST(MatrixmodTestRF, CP_crear_rdpg)
{
  gt_f = 6;
  gt_c = 4;
  //start_rdpg(6,4); /* Iniciar la creacion de una Matriz de incidencia I con 6 filas y 4 columnas. */
  start_rdpg(gt_f,gt_c); /* Iniciar la creacion de una Matriz de incidencia I con 6 filas y 4 columnas. */

  /* Verificamos si la creacion de I fue exitosa (mc[0]=1). */
  ASSERT_INT_EQ(mc[mIncidenciaI], 1);

  /* Verificamos si la creacion del resto de matrices fue exitoso. */
  EXPECT_INT_EQ(mc[vmActual], 1);
  EXPECT_INT_EQ(mc[mDisparos], 1);
  EXPECT_INT_EQ(mc[vmNuevo], 1);
  EXPECT_INT_EQ(mc[mIncidenciaH], 1);
  EXPECT_INT_EQ(mc[mIncidenciaR], 1);
  EXPECT_INT_EQ(mc[vectorE], 1);
  EXPECT_INT_EQ(mc[vectorQ], 1);
  EXPECT_INT_EQ(mc[vectorW], 1);
  EXPECT_INT_EQ(mc[vectorL], 1);
  EXPECT_INT_EQ(mc[vectorB], 1);
  EXPECT_INT_EQ(mc[vectorEx], 1);

  /* Validamos si todos los valores estan inicializados en cero.*/


  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "CP: Crear rdpg. Se ejecuto correctamente.");
}

TEST(MatrixmodTestRF, CP_agregarValor_MI)
{
  /* Verificamos si la creacion de I fue exitosa (mc[0]=1). */
  ASSERT_INT_EQ(mc[mIncidenciaI], 1);

  /* Agregacion de valores en matriz I. */
  // int i;
  // int t_tam= I.filas*I.columnas;
  // for(i=0; i<t_tam; i++)
  // {
  //   EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[i], gt_vaux, gt_faux, gt_caux, &I), -1); // analizar posibilidad de meter un for.  
  // }
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[0], gt_vaux, gt_faux, gt_caux, &I), -1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[1], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[2], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[3], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[4], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[5], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[6], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[7], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[8], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[9], gt_vaux, gt_faux, gt_caux, &I), -1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[10], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[11], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[12], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[13], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[14], gt_vaux, gt_faux, gt_caux, &I), -1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[15], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[16], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[17], gt_vaux, gt_faux, gt_caux, &I), -1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[16], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[19], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[20], gt_vaux, gt_faux, gt_caux, &I), -1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[21], gt_vaux, gt_faux, gt_caux, &I), 1);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[22], gt_vaux, gt_faux, gt_caux, &I), 0);
    EXPECT_INT_EQ(agregar_valor((int)gt_entradaMI[23], gt_vaux, gt_faux, gt_caux, &I), 0);

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "CP: Agregar valor en MI. Se ejecuto correctamente.");
}

TEST(MatrixmodTestRF, CP_RDPG_dispararT0)
{
  /* Verificamos si existen las matrices de incidencia I, H y R desde el vector de matrices creadas (mc[]=1). */
  ASSERT_INT_EQ(mc[mIncidenciaI], 1);
  ASSERT_INT_EQ(mc[mIncidenciaH], 1);
  ASSERT_INT_EQ(mc[mIncidenciaR], 1);

  /* Efectuamos disparo sobre la RDPG. */
  (void)disparar_rdpg(0, 'E'); /* Disparo en modo explicito. Y se castea el return entero a void para ignorarlo.*/

  /* Mensaje de finalizacion de caso de prueba. */
  tlog(T_INFO, "CP: Disparo T0 en RDPG. Se ejecuto correctamente.");
}

/*Pruebas de integracion: */

/*Pruebas de sistema: */

/* Funcion que adiciona los casos de prueba a cada banco de prueba para ser utilizados con KTF en nuestro modulo.*/
static void add_tests(void)
{
  ADD_TEST(CP_crear_rdpg);
  ADD_TEST(CP_crear_I);
  ADD_TEST(CP_agregarValor_MI);
  /*ADD_TEST(hello_fail);*/
}
