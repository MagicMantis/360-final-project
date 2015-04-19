CC = gcc
CCFLAGS = -g

all: server

server: server.o utility.o
	$(CC) $(CCFLAGS) $^ -o $@

server.o: utility.h
utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o
