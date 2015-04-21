CC = gcc
CCFLAGS = -g

all: server client

client: client.o utility.o udcp.o

server: server.o utility.o udcp.o
	$(CC) $(CCFLAGS) $^ -o $@

server.o: utility.h
client.o: utility.h udcp.h
udcp.o: utility.h
utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o
