CC = gcc
CFLAGS = -Wall -g

all: clean main

main: main.o g04.o

main.o: main.c main.h

g01: g04.c main.h

run:
	./main

clean:
	rm -f main.o main g04.o
