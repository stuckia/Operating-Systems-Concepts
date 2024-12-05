CC = gcc
CFLAGS = -Wall -g -pthread -o
RM = rm -f

workload_balancer: balancer.c
	$(CC) $(CFLAGS) workload_balancer balancer.c -lpthread -lrt