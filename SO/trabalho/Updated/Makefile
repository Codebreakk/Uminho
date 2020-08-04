CC = gcc
CFLAGS = -Wall -g -std=gnu99 -m64

all: argus argusd

argus: argus.o cliente.o mySOlib.o
							$(CC) $(CFLAGS) -o argus argus.o cliente.o mySOlib.o

argusd: argusd.o servidor.o mySOlib.o
					$(CC) $(CFLAGS) -o argusd argusd.o servidor.o mySOlib.o

run:
	./argus

clean:
	rm -f *.o argus log.idx log argusd fifo* historico
