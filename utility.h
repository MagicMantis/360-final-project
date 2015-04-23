#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

// Print a message to stdout and exit the program.
void failProgram(char* msg);

#endif
