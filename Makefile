CC = gcc
CFLAGS=-Wall -g -lm

all: client server

client: client.o utility.o robot.o udcp.o
	$(CC) $(CFLAGS) $^ -o $@

server: server.o utility.o udcp.o
	$(CC) $(CFLAGS) $^ -o $@

client.o: utility.h
server.o: utility.h
robot.o: robot.h
udcp.o: udcp.h
utility.o: utility.h

.PHONY:
clean:
	rm -f *.o client server *.txt *.png
