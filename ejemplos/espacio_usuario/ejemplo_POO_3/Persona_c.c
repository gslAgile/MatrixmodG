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

Persona_c *new_Persona_inHeap(){

	/* Creacion de puntero a objeto persona.*/
	Persona_c *punt;

	/* Asignacion de memoria*/
	punt = (Persona_c*) malloc(sizeof(Persona_c));

	if(punt == NULL){ /* Captura de error.*/

		/* Gestion de error.*/
		printf("No fue posible crear objeto! \n");
		return NULL;
	}

	/* Asignación de métodos de objeto persona con la vtable de la clase persona. */
	punt->metodos = &vtable_Persona;
 
	/* Inicialización por defecto de atributos del objeto. */
	punt->edad= 1;
	punt->altura= 1;

	return punt;
}

/* Implementacion de destructor de clase persona. */
void delete_Persona_inHeap(Persona_c* p){

	if(p != NULL) 	/* Si puntero no es null es posible liberar la memoria.*/
	{
		free(p);	/* liberación de memoria previamente reservada.*/
		p = NULL;
	}
	else {			/* Captura de error, posible Unallocated.*/
		
		/* Gestion de error*/
	}
}
 
/* Implementación de métodos de clase Persona*/
int edadPersona(Persona_c *p){ return p->edad; }
float alturaPersona(Persona_c *p){ return p->altura; }