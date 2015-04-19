CC = gcc
CCFLAGS = -g

server.o: utility.h
utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o
