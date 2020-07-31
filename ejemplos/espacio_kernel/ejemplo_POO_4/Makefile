# Si se llama el makefile desde espacio usuario con comando make la variable KERNELRELEASE no esta configurada, rebota el if y se ejecuta el else.
# En el else se configura la variable KERNELRELEASE con la construccion del directorio usando las variables KDIR y PWD, enviandolo por parametro
# desde las reglas del makefile (module: o clean:) utilizando la opcion -C en conjunto para la construccion del modulo.
# Variable KERNELRELEASE: En los makefile es una cadena unica adecuada para construir nombres de directorios en la carga de un modulo.
#
# Este makefile se ejecuta dos veces, inicialmente desde el espacio usuario cuando se llama por el comando "make" y una segunda vez desde cualquiera
# de las reglas definidas en el propio makefile. En este momento la cariable KERNELRELEASE ya esta configurada.

ifneq ($(KERNELRELEASE),)
	obj-m := mi_DriverSV.o # mi_DriverSV.o, se forma con los archivos de "mi_Driver-objs"
	mi_DriverSV-objs = Persona_c.o mi_Driver.o

else 
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD  := $(shell pwd)

module:
		#$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
		$(MAKE) -C $(KDIR) M=$(PWD) modules	

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean

endif