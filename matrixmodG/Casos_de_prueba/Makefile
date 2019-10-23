# Makefile de matrixmod.c con soporte a KTF 
ccflags-y += -Wno-declaration-after-statement

# Version de kernel sobre la que corre KTF
KVER = 3.16.0-38-generic

# Directorio fuente donde se encuentra KTF
KTF_DIR = /home/gabriel/src/ktf/ktf/kernel

# Directorio donde fue compilado KTF y donde se creo ktf.ko
KTF_BDIR = /home/gabriel/build/3.16.0-38-generic/ktf/kernel

-include ktf_gen.mk

ccflags-y += -I$(KTF_DIR)

obj-m := matrixmodG_db.o # matrixmod_db.c no existe, se forma con los archivos de "matrixmod_db-objs"
matrixmodG_db-objs = ../Librerias_kernel/MV_object.o ../Librerias_kernel/mascii.o ../Librerias_kernel/sformat_control.o ../Librerias_kernel/RDPG_object.o matrixmodG_tests.o
# matrixmod_db-objs = ../Librerias_kernel/mascii.o matrixmod_tests.o
#matrixmod_db-objs = matrixmod_tests.o

# Directorio a la fuente del kernel
KDIR   := /lib/modules/$(KVER)/build
PWD    := $(shell pwd)

EXTRASYMS := KBUILD_EXTRA_SYMBOLS="$(KTF_BDIR)/Module.symvers"

module:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) $(EXTRASYMS) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

check: all
install: all
	