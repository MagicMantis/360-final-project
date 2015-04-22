/* Written by Joseph Savold
 *
 * udcp.h: This header contains function prototypes for
 * functions used by the client to establish a connection
 * and to control the robot */

#include "utility.h"

typedef struct Response_t {
	unsigned int request_id;
	unsigned int num_of_msgs;
	unsigned int sequence_num;
	char data[1000];
	size_t data_size;
} Response;

typedef struct Request_t {
	unsigned int robot_id;
	unsigned int request_id;
	char request[16];
} Request;

extern int message_id;

//reliably send a message to the server
int udcpSend(int sock, struct addrinfo info, void *buffer, size_t size, unsigned int id);

//reliably receive a message from the server
int udcpRecv(int sock, void *buffer, size_t size, unsigned int id);
