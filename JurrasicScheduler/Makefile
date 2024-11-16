CC = gcc
CFLAGS = -g -o
RM = rm -f

jurassic_scheduler: scheduler.c
	$(CC) $(CFLAGS) jurassic_scheduler scheduler.c

install: jurassic_shell
	sudo cp jurassic_scheduler /bin/

uninstall:
	sudo $(RM) /bin/jurassic_scheduler

clean:
	$(RM) jurassic_scheduler