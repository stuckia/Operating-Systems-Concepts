CC = gcc
CFLAGS = -g -pthread -o
RM = rm -f

workload_balancer: balancer.c
	$(CC) $(CFLAGS) workload_balancer balancer.c

install: jurassic_shell
	sudo cp workload_balancer /bin/

uninstall:
	sudo $(RM) /bin/workload_balancer

clean:
	$(RM) workload_balancer