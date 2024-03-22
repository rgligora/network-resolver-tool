CC=gcc
CFLAGS=-I.

default: prog

prog: prog.o
	$(CC) -o prog prog.o

prog.o: prog.c prog.h
	$(CC) -c prog.c $(CFLAGS)

clean:
	rm -f *.o prog