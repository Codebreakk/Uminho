CC = gcc
CFLAGS = -Wall -g

all: clean main

main: main.o g02.o

main.o: main.c main.h

g01: g02.c main.h

run:
	./main

clean:
	rm -f main.o main g02.o
