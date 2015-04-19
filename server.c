/* Written by Joseph Savold, Marshall Clyburn, and Jackson Wood
 *
 * server.c: contains the procedures to recieve UDCP requests, and
 * forward them to the robot server as HTTP requests, and to return
 * the results to the client via UDCP. */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#include "utility.h"

#define SERVER_ADDR "130.127.192.62"
char *robotAddrName, *robotID, *imageID;

int main(int argc, char *argv[]) {

	//check for usage errors
	if (argc != 4) failProgram("Usage error: ./server <robot_id> <hostname> <port>\n");

	//get parameters from command line arguments

	int inPort; /* in port is for UDCP server bind */
	inPort = atoi(argv[1]);
	robotID = argv[2];
	imageID = argv[3];

	return 0;
}

/* Form and send the http request coorespondign to rqNum (request number) */
void sendRobotRequest(char* robotID, int rqNum) {

	//allocate space for robot path string
	char *robotAddrPath = (char *) malloc(100);

	unsigned int outPort;
	
	//format variables based on request type

	switch (rqNum) {
		case 0: //get image
			outPort = 8081;
			sprintf(robotAddrPath, "/snapshot?topic=/%s/image?width=600?height=500", robotID);
			break;
		case 1: //get GPS
			outPort = 8082;
			sprintf(robotAddrPath, "/state?id=%s", robotID);
			break;
		case 2: //get dGPS
			outPort = 8084;
			sprintf(robotAddrPath, "/state?id=%s", robotID);
			break;
		case 3: //get lazers
			outPort = 8083;
			sprintf(robotAddrPath, "/state?id=%s", robotID);
			break;
		case 4: //move
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&lx=%d", robotID, /*TODO speed var*/outPort);
			break;
		case 5: //turn
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&az=%d", robotID, /*TODO degrees var*/outPort);
			break;
		case 6: //stop
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&lx=0", robotID);
			break;
		default: failProgram("Recieved bad request number. Exiting program...\n");
	}

	//set up accepted addresses
	struct sockaddr_in robotAddr;
	memset(&robotAddr, 0, sizeof(struct sockaddr)); /* Zero out structure */
	robotAddr.sin_family = AF_INET; /* Internet address family */
	robotAddr.sin_addr.s_addr = inet_addr(robotAddrName); /* Address of robot server */
	robotAddr.sin_port = htons(outPort); /* Port of desired service */

	//get the host name
	struct addrinfo* results;
	char port_str[6];
	sprintf(port_str, "%d", outPort);
	if (robotAddr.sin_addr.s_addr == (in_addr_t) -1) {
	    struct addrinfo criteria;
	    memset(&criteria, 0, sizeof(struct addrinfo));
	    criteria.ai_family = AF_INET;
	    criteria.ai_socktype = SOCK_STREAM;
	    criteria.ai_protocol = IPPROTO_TCP;
	    getaddrinfo(robotAddrName, port_str, &criteria, &results);
	}

	//create socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
		failProgram("Failed to create socket. \n");

	//establish connection
	if (connect(sock, (struct sockaddr*) &(results->ai_addr), sizeof(struct sockaddr)) < 0)
		failProgram("Failed to connect to server. \n");

	//form http request
	char *request = (char *) malloc(1000);
	sprintf(request, "GET https://%s:%d HTTP/1.1\r\nHost: %s\r\n\r\n", 
		robotAddrName, outPort, robotAddrName);

	//send http request
	int totalBytes = 0;
	int bytes;
	do {
		if ((bytes = send(sock, &request[totalBytes], strlen(request)-totalBytes, 0)) == -1) 
			failProgram("Send failed. \n");
		totalBytes += bytes;
	} while (totalBytes < strlen(request));

	//TODO process response


	//cleanups
	free(request);
	free(robotAddrPath);
}
