CONFIG_MODULE_SIG=n
ccflags-y := -std=gnu99

CONFIG_FRAME_WARN=0

obj-m += maze.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
