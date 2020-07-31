#ifndef CLASE_PERSONA_H                   	/* protección de inclusión */
#define CLASE_PERSONA_H

#include <linux/module.h>
#include <linux/kernel.h>

/* Informacion del DDL (Device Driver Linux) */
MODULE_LICENSE("GPL");						/* Licencia del módulo */


typedef struct persona_struct Persona_c;  	/* estructura opaca de clase persona*/
 
/* Vtable de clase persona*/
struct p_metodos
{
  int (*getEdad)(Persona_c *);
  int (*getAltura)(Persona_c *);
};
 
/* Estructura de la clase persona*/
struct persona_struct
{
   /* Atributos */
   int edad;
   int altura;
 
   /* Metodos */
   const struct p_metodos *metodos;
};
 
/* Declaración de métodos de clase persona*/
void new_Persona(Persona_c *p);

int edadPersona(Persona_c *p);
int alturaPersona(Persona_c *p);


#endif /* CLASE_PERSONA_H */