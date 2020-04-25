CC = gcc
CFLAGS = -Wall -g -I ../../mysolib/

all: clean main

main: main.o g05.o ../../mysolib/my_printf.o

run:
	./main

clean:
	rm -f main.o main g05.o
