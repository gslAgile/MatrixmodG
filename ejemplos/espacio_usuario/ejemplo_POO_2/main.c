#include <stdio.h>
#include <string.h>
#include "Persona_c.h"

int main()
{
	Persona_c persona_1;
	new_Persona(&persona_1);

	printf("Edad de persona: %d \n", persona_1.metodos->getEdad(&persona_1));
	printf("Altura de persona: %.2f \n", persona_1.metodos->getAltura(&persona_1));

	return 0;
}