CC = gcc
CFLAGS = -Wall -g -std=gnu99 -m64

all: main_cliente main_servidor

main_cliente: main_cliente.o cliente.o mySOlib.o
							$(CC) $(CFLAGS) -o argus main_cliente.o cliente.o mySOlib.o

main_servidor: main_servidor.o servidor.o mySOlib.o
					$(CC) $(CFLAGS) -o argus_server main_servidor.o servidor.o mySOlib.o

run:
	./argus

clean:
	rm -f *.o argus log.idx log argus_server fifo* historico
