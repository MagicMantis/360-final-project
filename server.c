/* Written by Joseph Savold, Marshall Clyburn, and Jackson Wood
 *
 * server.c: contains the procedures to recieve UDCP requests, and
 * forward them to the robot server as HTTP requests, and to return
 * the results to the client via UDCP. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#define SERVER_ADDR "130.127.192.62"

int main(int argc, char *argv[]) {

	struct sockaddr_in robotAddr;	/* address of the server managing the robot */
	struct hostent *thehost; 		/* hostent used for gethostbyname() */

	int sock;	/* TCP sockets for communicating with the robot server */
	int outPort, inPort; /* outPort is port for http server, in port is for UDCP server bind */
	int bytes = 0;
	char *robotAddrName, *robotAddrPath, *robotID, *imageID;

	//check for usage errors
	if (argc != 4) failProgram("Usage error: ./server <robot_id> <hostname> <port>\n");

	//get parameters from command line arguments
	inPort = atoi(argv[1]);
	robotID = argv[2];
	imageID = argv[3];

	return 0;
}

/* Form and send the http request coorespondign to rqNum (request number) */
void sendRobotRequest(int rqNum) {

	//allocate space for robot path string
	robotAddrPath = (char *) malloc(100);

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
			sprintf(robotAddrPath, "/twist?id=%s&lx=%d", robotID, /*TODO speed var*/port);
			break;
		case 5: //turn
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&az=%d", robotID, /*TODO degrees var*/port);
			break;
		case 6: //stop
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&lx=0", robotID);
			break;
		default: failProgram("Recieved bad request number. Exiting program...\n");
	}

	//set up accepted addresses
	memset(&robotAddr, 0, sizeof(robotAddr)); /* Zero out structure */
	servAddr.sin_family = AF_INET; /* Internet address family */
	servAddr.sin_addr.s_addr = inet_addr(robotAddrName); /* Address of robot server */
	servAddr.sin_port = htons(port); /* Port of desired service */

	//get the host name
	if (servAddr.sin_addr.s_addr == (in_addr_t) -1) {
		thehost = gethostbyname(servername);
		servAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);
	}

	//create socket
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
		failProgram("Failed to create socket. \n");

	//establish connection
	if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		failProgram("Failed to connect to server. \n");

	//form http request
	char *request = (char *) malloc(1000);
	sprintf(request, "GET https://%s:%d HTTP/1.1\r\nHost: %s\r\n\r\n", 
		robotAddrName, outPort, robotAddrName);

	//send http request
	int totalBytes = 0;
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
