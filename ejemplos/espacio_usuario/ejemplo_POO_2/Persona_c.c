#include <stdio.h>
#include <stdlib.h>
#include "Persona_c.h"

/* Asignación de métodos a vtable de clase persona*/
struct p_metodos vtable_Persona = {
	.getEdad = edadPersona,
	.getAltura = alturaPersona
};
 
/* Implementación de constructores de clase Persona*/
void new_Persona(Persona_c *p){

	/* Asignación de métodos de objeto persona con la vtable de la clase persona. */
	p->metodos = &vtable_Persona;
 
	/* Inicialización por defecto de atributos del objeto. */
	p->edad= 0;
	p->altura= 0;
}
 

/* Implementación de métodos de clase Persona*/
int edadPersona(Persona_c *p){ return p->edad; }
float alturaPersona(Persona_c *p){ return p->altura; }