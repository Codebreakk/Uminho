CC = gcc
CFLAGS = -Wall -g

all: clean main

main: main.o g03.o

main.o: main.c main.h

g01: g03.c main.h

run:
	./main

clean:
	rm -f main.o main g03.o
