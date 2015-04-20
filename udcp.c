#include "udcp.h"

int message_id = 0;

//reliably send a message to the server
int udcpSend() {



}

//reliably receive a message from the server
int udcpRecv(int sock, void *buffer, size_t size, int id) {



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
