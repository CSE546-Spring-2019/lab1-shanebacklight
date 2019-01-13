#makefile for count.c

CC=gcc
CFLAGS = -g -Wall

all: count

count: count.c
	$(CC) $(CFLAGS) -o count count.c

clean:
	rm count
