CC = gcc
CFLAGS = -Wall

all: clean main

main: main.o g01.o

main.o: main.c main.h

g01: g01.c main.h

run:
	./main

clean:
	rm -f main.o main g01.o
