CC = gcc
CFLAGS = -Wall -g -I ../../mysolib/

all: main

main: main.o g99.o ../../mysolib/my_printf.o

run:
	./main

clean:
	rm -f main.o main g99.o
