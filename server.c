/* Written by Joseph Savold, Marshall Clyburn, and Jackson Wood
 *
 * server.c: contains the procedures to recieve UDCP requests, and
 * forward them to the robot server as HTTP requests, and to return
 * the results to the client via UDCP. */

#include <arpa/inet.h>
#include <assert.h>
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

char *robotName;

void sendRobotRequest(char *robotID, int agNum, int speed, char *ImageID);

struct sockaddr_in servAddr;
struct sockaddr_in clntAddr;
int sockUDP;
unsigned int messageLength;
unsigned int cliAddrLen;
unsigned int ID;

int main(int argc, char *argv[]) {

	//check for usage errors
	if (argc != 5) printf("Usage error: ./server <server port> <hostname> <robot ID> <image ID>\n");

	int port = atoi(argv[1]);
	robotName = argv[2];
	char *robotID = argv[3];
	char *imageID = argv[4];
	char buff[500];

	if((sockUDP = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		printf("Error with setting up socket\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if(bind(sockUDP, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		printf("Couldn't bind to socket\n");

	cliAddrLen = sizeof(clntAddr);

	for(;;){	
		messageLength = recvfrom(sockUDP, buff, sizeof(buff), 0, (struct sockaddr *) &clntAddr, &cliAddrLen);
		if(messageLength == -1){
			printf("recvfrom failed\n");
			exit(0);
		}
		printf("Message Length = %d\n", messageLength);
		printf("Got a message\n");
		memset(&ID, 0, 4);
		memcpy(&ID, buff, 4);
		char roboID[strlen(robotID)];
		memcpy(roboID, buff+4, strlen(robotID));
		char command[11];
		memset(command, 0, 11);
		memcpy(command, buff+4+strlen(roboID)+1, 11);
		printf("ID = %d\nrobotID = %s\ncommand = %s\n", ID, robotID, command);
		
		char *order = strtok(command, " ");
		if(strcmp(order,"GET") == 0){
			order = strtok(NULL," ");
			if(strcmp(order,"IMAGE") == 0){
				sendRobotRequest(roboID, 0, 0, imageID);
			}
			else if(strcmp(order,"GPS") == 0){
				sendRobotRequest(roboID, 1, 0, imageID);
			}
			else if(strcmp(order,"DGPS") == 0){
				sendRobotRequest(roboID, 2, 0, imageID);
			}
			else if(strcmp(order,"LASERS") == 0){
				sendRobotRequest(roboID, 3, 0, imageID);
			}
		}
		else if(strcmp(order,"MOVE") == 0){
			order = strtok(NULL," ");
			int speed = atoi(order);
			sendRobotRequest(roboID, 4, speed, imageID);
		}
		else if(strcmp(order,"TURN") == 0){
			order = strtok(NULL," ");
			int speed = atoi(order);
			sendRobotRequest(roboID, 5, speed, imageID);
		}
		else{
			sendRobotRequest(roboID, 6, 0, imageID);
		}
//		printf("resetting buff\n");
		memset(buff, 0, 500);		
	}
//	printf("About to exit\n");
	exit(0);
}

/* Form and send the http request coorespondign to rqNum (request number) */
void sendRobotRequest(char* robotID, int rqNum, int speed, char *imageID) {
	//allocate space for robot path string
	char *robotAddrPath = (char *) malloc(100);

	unsigned int outPort;

	//format variables based on request type

	switch (rqNum) {
		case 0: //get image
			outPort = 8081;
			sprintf(robotAddrPath, "/snapshot?topic=/robot_%s/image?width=600?height=500", imageID);
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
			sprintf(robotAddrPath, "/twist?id=%s&lx=%d", robotID, speed);
			break;
		case 5: //turn
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&az=%d", robotID, speed);
			break;
		case 6: //stop
			outPort = 8082;
			sprintf(robotAddrPath, "/twist?id=%s&lx=0", robotID);
			break;
		default: printf("Recieved bad request number. Exiting program...\n");
	}

	struct hostent *server;

	server = gethostbyname(robotName);

	//set up accepted addresses
	struct sockaddr_in robotAddr;
	memset(&robotAddr, 0, sizeof(struct sockaddr));
	robotAddr.sin_family = AF_INET; 
	robotAddr.sin_addr.s_addr = *((unsigned long *)server->h_addr_list[0]); 
	robotAddr.sin_port = htons(outPort); 

	
//	printf("got past weird shit\n");

	//create socket
	int sockTCP;
	if ((sockTCP = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
		printf("Failed to create socket. \n");

	//establish connection
	if(connect(sockTCP, (struct sockaddr*) &robotAddr, sizeof(robotAddr)) < 0)
		printf("Failed to connect to server. \n");

	//form http request
	char *request = (char *) malloc(1000);
	sprintf(request, "GET http://%s:%d/%s HTTP/1.1\r\nHost: myrequester/1.1\r\n\r\n",
		robotName, outPort, robotAddrPath);

//	printf("%s\n", robotAddrPath);
	printf("%s\n", request);

	//send http request
	int bytes;
	bytes = send(sockTCP, request, strlen(request), 0);
	printf("TCP bytes sent = %d\n", bytes);
     	
	char buff[988];
	memset(buff, 0, 988);
	int check = recv(sockTCP, buff, 988, 0);
	printf("Buff = %s\n", buff);
	printf("Bytes read = %d\n", check);

	char *response = strstr(buff, "\r\n\r\n");
	response+=4;

	char message[1000];
	memset(message, 0, 1000);
	memcpy(message, &ID, 4);
	unsigned int one = 1;
	memcpy(message+4, &one, 4);
	memcpy(message+8, &one, 4);
	memcpy(message+12, response, 988);

	int sentLen = sendto(sockUDP, message, 1000, 0, (struct sockaddr *) &clntAddr, sizeof(clntAddr));
	printf("sentLen = %d\n", sentLen);
	
	//cleanups
	free(request);
	free(robotAddrPath);
//	printf("Finishing sendRobotCommand. Returning\n");
	
}
