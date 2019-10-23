# Si se llama el makefile desde espacio usuario con comando make la variable KERNELRELEASE no esta configurada, rebota el if y se ejecuta el else.
# En el else se configura la variable KERNELRELEASE con la construccion del directorio usando las variables KDIR y PWD, enviandolo por parametro
# desde las reglas del makefile (module: o clean:) utilizando la opcion -C en conjunto para la construccion del modulo.
# Variable KERNELRELEASE: En los makefile es una cadena unica adecuada para construir nombres de directorios en la carga de un modulo.
#
# Este makefile se ejecuta dos veces, inicialmente desde el espacio usuario cuando se llama por el comando "make" y una segunda vez desde cualquiera
# de las reglas definidas en el propio makefile. En este momento la cariable KERNELRELEASE ya esta configurada.

ifneq ($(KERNELRELEASE),)
	obj-m := matrixmodG_SV1.o # matrixmodG.o, se forma con los archivos de "matrixmodG-objs"
	matrixmodG_SV1-objs = /Librerias_kernel/MV_object.o /Librerias_kernel/mascii.o /Librerias_kernel/sformat_control.o /Librerias_kernel/RDPG_object.o matrixmodG.o

else 
	KDIR := /lib/modules/$(shell uname -r)/build
	PWD  := $(shell pwd)

module:
		#$(MAKE) -C $(KDIR) M=$(PWD) modules	
		$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean

endif


# Makefile de matrixmod.c con soporte a KTF (migrado a driver /Casos_de_prueba/matrizmod_tests.c)
# ccflags-y += -Wno-declaration-after-statement

# # Version de kernel sobre la que corre KTF
# KVER = 3.16.0-38-generic

# # Directorio fuente donde se encuentra KTF
# KTF_DIR = /home/gabriel/src/ktf/ktf/kernel

# # Directorio donde fue compilado KTF y donde se creo ktf.ko
# KTF_BDIR = /home/gabriel/build/3.16.0-38-generic/ktf/kernel

# -include ktf_gen.mk

# ccflags-y += -I$(KTF_DIR)

# #obj-m := matrixmod.o
# obj-m := matrixmodG.o # matrixmodG.c no existe, se forma con los archivos de "matrixmodG-objs"
# matrixmodG-objs = /Librerias_kernel/mascii.o matrixmod.o


# # Directorio a la fuente del kernel
# KDIR   := /lib/modules/$(KVER)/build
# PWD    := $(shell pwd)

# EXTRASYMS := KBUILD_EXTRA_SYMBOLS="$(KTF_BDIR)/Module.symvers"

# module:
# 	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) $(EXTRASYMS) modules
# clean:
# 	$(MAKE) -C $(KDIR) M=$(PWD) clean

# check: all
# install: all


