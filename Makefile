CC = gcc
CCFLAGS=-Wall -g

BINS=client server

all: $(BINS)

server: server.o utility.o
	$(CC) $(CCFLAGS) -o server server.c

client: client.c
	$(CC) $(CCFLAGS) -o client client.c

.PHONY:
clean:
	rm -f *.o $(BINS) 
