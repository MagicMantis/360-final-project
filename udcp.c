#include "udcp.h"

int message_id = 0;

//reliably send a message to the server
int udcpSend(int sock, struct addrinfo info, void *buffer, size_t size, unsigned int id) {



}

//reliably receive a message from the server
int udcpRecv(int sock, void *buffer, size_t size, unsigned int id) {

	Response *temp = malloc(sizeof(Response));

	int bytes = -1;
	while (bytes < 0) {
		struct sockaddr fromAddr;
		struct socklen_t fromAddrLen;
		bytes = recvfrom(sock, temp, size, 0, &fromAddr, &fromAddrLen);
	}

	Response *msgs = (Response *) buffer;
	unsigned int id = temp->request_id;
	unsigned int num_msgs = temp->num_of_msgs;
	unsigned int seq = temp->sequence_num;
	memcpy(&msgs[seq], temp, sizeof(Response));
	int i = 1;
	while (i < num_msgs) {
		bytes = recvfrom(sock, temp, size, 0, &fromAddr, &fromAddrLen);
		if (bytes > 0) {
			if (temp->request_id != id) continue;
			temp->data_size = bytes - (sizeof(unsigned int) * 3);
			memcpy(&msgs[temp->sequence_num], temp, sizeof(Response));
			i++;
		}
	}

	uint8_t *unpack = malloc(num_msgs * 1000);
	int loc = 0;
	for (i = 0; i < num_msgs; i++) {
		memcpy(&unpack[loc], &(msgs[i].data), msgs[i].data_size);
		loc += msgs[i].data_size;
	}
	memcpy(buffer, unpack, (size_t) loc);
}


/*	   ssize_t bytes = sendto(sock, (void *) &guess, 4, 0,
	      serverAddr->ai_addr, serverAddr->ai_addrlen);
	   if (bytes < 0) die("send failed.\n"); 
	   else if (bytes != 4) die("sent wrong number of bytes.\n");

	   //recieve response from server	   
	   bytes = recvfrom(sock, (int *) &code, 4, 0, 
	      (struct sockaddr *) &fromAddr, &fromAddrLen);
	   if (bytes < 0) die("recieve failed");
	   else if (bytes != 4) die("recieved wrong # of bytes.\n");*/
