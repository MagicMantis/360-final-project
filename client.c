#include "utility.h"
#include "udcp.h"
#include "robot.h"

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

//replaced by action loop
/*
	char request[500];
	memset(request, 0, 500);
	messageNum = htonl(messageNum);
	memcpy(request, &messageNum, 4);
	char *requestPoint = request;
	requestPoint+=4;
	char tempID[sizeof(argv[3])];
	memcpy(tempID, robotID, strlen(robotID));
	tempID[sizeof(robotID)+1] = '\0';
	memcpy(requestPoint, tempID, strlen(robotID));
	requestPoint += sizeof(robotID);
	memcpy(requestPoint, image, strlen(image));
	printf("command = %s\n", requestPoint);
*/


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

	actionLoop(sock, (struct sockaddr *) &servAddr, length, N, robotID);

//replaced by action loop
/*	int sentLen = sendto(sock, request, 500, 0, (struct sockaddr *) &servAddr, sizeof(servAddr));
	printf("SentLen = %d\n", sentLen);

	alarm(5);

//	unsigned int test;
//	memcpy(&test, request, 32);
//	printf("ID sent = %d\n", test);

	const unsigned int buffer_size = 100 * 1000;
	char* buff = (char*) malloc(buffer_size * sizeof(char));
	int response_size = udcpRecv(sock, buff, messageNum);

	alarm(0);

	printf("response: %d\n", response_size);

	FILE *fp;
	fp = fopen("file.jpeg", "w");
	fwrite(buff, 1, response_size, fp);
	fclose(fp);
	free(buff);
*/

	exit(0);
}
