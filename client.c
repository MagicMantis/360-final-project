#include <stdio.h>
#include <stdlib.h>
#include "udcp.h"
#include "utility.h"
#include <string.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
	
	char *image = "GET IMAGE";
	char *gps = "GET GPS";
	char *dgps = "GET DGPS";
	char *laser = "GET LASERS";
	char *move = "MOVE 1";
	char *turn = "TURN 1";
	char *stop = "STOP";

	int sock; //socket id
	struct sockaddr_in servAddr;
	struct sockaddr_in fromAddr;
	struct hostent *thehost;
	unsigned int fromSize;

	//usage error check
	if (argc != 6) printf("Usage error: ./robotClient <ip/host> <port> <robot_id> <length> <number_of_sides>\n");

	char *server = argv[1];
	int port = atoi(argv[2]);
	char *robotID = argv[3];
	int length = atoi(argv[4]);
	int N = atoi(argv[5]);
	unsigned int messageNum = 0;

	printf("server = %s\nport = %d\nrobotID = %s\nlength = %d\nN = %d\n", server, port, robotID, length, N);

	char request[500];
	memcpy(request, &messageNum, 32);
	char *requestPoint = request;
	requestPoint+=32;
	char tempID[sizeof(argv[3])];
	memcpy(tempID, robotID, sizeof(robotID));
	tempID[sizeof(robotID)+1] = '\0';
	memcpy(requestPoint, tempID, sizeof(robotID));
	requestPoint += sizeof(robotID);
	memcpy(requestPoint, move, sizeof(move));
	
	if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		printf("Can't set up socket\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(server);
	servAddr.sin_port = htons(port);

	if(servAddr.sin_addr.s_addr == -1){
		thehost = gethostbyname(server);
		servAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);
	}

	sendto(sock, request, 500, 0, (struct sockaddr *) &servAddr, sizeof(servAddr));

	unsigned int test;
	memcpy(&test, request, 32);
	printf("ID sent = %d\n", test);

        char response[1000];
        fromSize = sizeof(fromAddr);
        int check = recvfrom(sock, response, 1000, 0, (struct sockaddr *) &fromAddr, &fromSize);
        char body[904];
        memcpy(body, response+96, 904);
        printf("check = %d\n", check);
        printf("%s\n", body);

	exit(0);
}
