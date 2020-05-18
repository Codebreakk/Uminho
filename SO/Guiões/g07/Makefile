CC = gcc
CFLAGS = -Wall -g -I ../../mysolib/

all: clean main

main: main.o g07.o ../../mysolib/my_printf.o

run:
	./main

clean:
	rm -f main.o main g07.o
