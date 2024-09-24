ccflags-y := -std=gnu99

CONFIG_MODULE_SIG=n

obj-m += maze.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
