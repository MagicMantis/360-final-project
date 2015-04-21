CC = gcc
CCFLAGS=-Wall -g

BINS=client server

all: $(BINS)

client: client.o utility.o udcp.o

server: server.o utility.o udcp.o
	$(CC) $(CCFLAGS) $^ -o $@

client: client.c
	$(CC) $(CCFLAGS) -o client client.c

server.o: utility.h
client.o: utility.h udcp.h
udcp.o: utility.h
utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o $(BINS) 
