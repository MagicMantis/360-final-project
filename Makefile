CC = gcc
CCFLAGS = -g

utility.o: utility.c
	$(CC) $(CCFLAGS) -c $^

.PHONY:
clean:
	rm -f *.o
