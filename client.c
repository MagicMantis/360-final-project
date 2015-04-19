#include <stdio.h>
#include <stdlib.h>
#include "udcp.h"
#include "utility.h"

int main(int argc, char *argv[]) {
	
	int sock; //socket id
	struct addrinfo addrCriteria; //criteria for connections
	struct addrinfo *serverAddr; //list of addresses found by getaddrinfo()
	//struct sockaddr_storage fromAddr; //address of response from server
	socklen_t fromAddrLen = sizeof(fromAddr); //length of fromAddr

	//usage error check
	if (argc != 6) failProgram("Usage error: ./robotClient <ip/host> <port> <robot_id> <length> <number_of_sides>\n");

	//set up criteria for possible addresses
	memset(&addrCriteria, 0, sizeof(addrCriteria));
	addrCriteria.ai_family = AF_UNSPEC;
	addrCriteria.ai_socktype = SOCK_DGRAM;
	addrCriteria.ai_protocol = IPPROTO_UDP;

	//obtain address information and place into serverAddr array
	if (getaddrinfo(argv[1], argv[2], &addrCriteria, &serverAddr) != 0) 
		failProgram("Failed to get address info.\n");

	//create the socket used for communication
	sock = socket(serverAddr->ai_family, serverAddr->ai_socktype, serverAddr->ai_protocol);
	if (sock < 0) failProgram("Failed to create socket.\n");
	
}
