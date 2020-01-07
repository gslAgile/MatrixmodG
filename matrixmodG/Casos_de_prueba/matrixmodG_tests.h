/*------------------------------------------------------------------------------------------------------------------------------------------------------* 
 * Libreria matrixmodG_tests.h/.c (archivo cabecera - header file).
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

/**
 * MT_ID_TRANSICION: Enumerador del identificador de una transicion de la RDPG.
 * @note Se definen el identificador de 5 transiciones, pero se puede seguir definiendo identificadores en la medida que se necesiten.
 */
enum MT_ID_TRANSICION{
  id_T0 = 0, /**< 0: Indica la matriz de incidencia I de una RdPG. */
  id_T1,
  id_T2,
  id_T3,
  id_T4,
  id_T5
};