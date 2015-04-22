CC = gcc
CCFLAGS=-Wall -g

BINS=client server

all: $(BINS)

server: server.o utility.o
	$(CC) $(CCFLAGS) -o server $^

client: client.o utility.o
	$(CC) $(CCFLAGS) -o client $^

client.o: utility.h
server.o: utility.h
utility.o: utility.h

.PHONY:
clean:
	rm -f *.o $(BINS) 
