CC = gcc
CCFLAGS=-Wall -g

BINS=client server

all: $(BINS)

server: server.o utility.o
	$(CC) $(CCFLAGS) $^ -o $@

client: client.c
	$(CC) $(CCFLAGS) -o client client.c

server.o: utility.h

utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o
