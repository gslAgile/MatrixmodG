
#include "Persona_c.h"     /* inclusión librería de clase persona*/
 
Persona_c persona_1;
 
static int __init init_miDriver(void)
{
    new_Persona(&persona_1);
 
    printk(KERN_INFO "Edad de persona: %d \n", persona_1.metodos->getEdad(&persona_1));
    printk(KERN_INFO "Altura de persona: %d \n", persona_1.metodos->getAltura(&persona_1));  

    return 0;
}

static void __exit exit_miDriver(void)
{
  printk(KERN_INFO "DDL desinstalado del kernel exitosamente!\n");
}
 
module_init(init_miDriver);
module_exit(exit_miDriver);