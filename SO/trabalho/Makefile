CC = gcc
CFLAGS = -Wall -g -I ../mysolib/

all: main

main: main.o trabalho.o ../mysolib/my_printf.o
			gcc -o argus main.o trabalho.o ../mysolib/my_printf.o

run:
	./argus

clean:
	rm -f *.o argus log.idx log
