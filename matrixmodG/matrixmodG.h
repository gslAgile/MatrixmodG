/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Device Driver Linux MatrixmodG (archivo cabecera - header file).
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

#ifndef MATRIXMOD_H
#define MATRIXMOD_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/list.h>

#include "Librerias_kernel/RDPG_object.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * INFORMCION DE DRIVER MATRIXMODG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
MODULE_LICENSE("GPL");                                                      /* Licencia del módulo */
MODULE_AUTHOR("Doctor Ing. Micolini Orlando, Ing. Sosa Ludueña Gabriel.");  /* Autores */
MODULE_DESCRIPTION("MatrixmodG permite la gestion de una RDPG en el kernel."\
          " Comunicacion disponible desde entrada /proc/matrixmodG.");      /* Descripcion del modulo */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE MACROS
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SUCCESS 0;


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE FUNCIONES DE LIBRERIA
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int matrixmodG_open(struct inode *inode, struct file *file);
static int matrixmodG_release(struct inode *inode, struct file *file);
static ssize_t matrixmodG_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static ssize_t matrixmodG_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static int init_matrixmodG(void);
static void exit_matrixmodG(void);

/* Enumeraciones */
/**
 * MAXIMOS_MINIMOS: Enumerador de los valores maximos y minimos que se utilizan en la implementacion del modulo matrixmod.
 */
enum MAXIMOS_MINIMOS{
  MAX_COMPONENTS_RDP = 20 /**< Maximo de componentes que utiliza una rdp. Equivale a la cantidad de matrices y vectores que se asocian a una rdp. */
};


#ifdef MT_TEST_ON /* Si modulo esta en proceso de prueba, se inicia las pruebas con framework de prueba KTF. */
  
  /* Se llama esta funcion para realizar las pruebas con KTF en nuestro modulo */
  KTF_INIT();

  /* Prototipo de funciones de matrixmod_tests que se utilizan en matrixmod */
  void add_tests(void);

#endif


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE VARIABLES GLOBALES DE DRIVER
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct file_operations proc_entry_fops = {		/* Estructura de operaciones brindadas por driver. */
    .owner = THIS_MODULE,
    .open = matrixmodG_open,
    .release = matrixmodG_release,
    .read = matrixmodG_read,
    .write = matrixmodG_write,    
};

static int Device_Open = 0; /* Contador de accesos a driver. Es un device open? */
			    			/* Uso para prevenir multiples accesos en el dispositivo */

//int count_read = 0; 		/* Contador de lecturas en driver. */

static struct proc_dir_entry *proc_entry; /* Variable para gestion de archivo de entrada de /proc. */

RDPG_o RDP1; 				/* Declaracion de Objeto RDP 1. */


#endif /* MATRIXMOD_H */

