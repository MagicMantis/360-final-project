CC = gcc
CCFLAGS=-Wall -g

all: client server

client: client.o utility.o udcp.o
	$(CC) $(CCFLAGS) $^ -o $@

server: server.o utility.o udcp.o
	$(CC) $(CCFLAGS) $^ -o $@

client.o: utility.h
server.o: utility.h
udcp.o: udcp.h
utility.o: utility.h

.PHONY:
clean:
	rm -f *.o client server
