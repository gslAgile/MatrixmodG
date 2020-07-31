#ifndef CLASE_PERSONA_H                   /* protección de inclusión */
#define CLASE_PERSONA_H

typedef struct persona_struct Persona_c;  /* estructura opaca de clase persona*/
 
/* Vtable de clase persona*/
struct p_metodos
{
  int (*getEdad)(Persona_c *);
  float (*getAltura)(Persona_c *);
};
 
/* Estructura de la clase persona*/
struct persona_struct
{
   /* Atributos */
   int edad;
   float altura;
 
   /* Metodos */
   const struct p_metodos *metodos;
};
 
/* Declaración de métodos de clase persona*/
void new_Persona(Persona_c *p);
Persona_c* new_Persona_inHeap();
void delete_Persona_inHeap(Persona_c* p);

int edadPersona(Persona_c *p);
float alturaPersona(Persona_c *p);


#endif /* CLASE_PERSONA_H */