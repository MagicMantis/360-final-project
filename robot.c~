#include "utility.h"
#include "udcp.h"
#include "robot.h"

int message_id = 0;
int step_num = 0;

struct sockaddr *servAddr;
int sock;

void actionLoop(int s, struct sockaddr *addr, int length, int sides, char *robot_id) {

	//set socket and address
	sock = s;
	servAddr = addr;

	//calculate moves/degrees
	int state1moves = sides;
	int state2moves = sides-1;
	double state1rad = (2.0/(double)state1moves)*M_PI;
	double state2rad = (2.0/(double)state2moves)*M_PI;

	//initial data snapshot
	getInfo(robot_id);
	step_num++;

	//action loop
	int i;
	for (i = 0; i < state1moves; i++) {
		alarm(5);
		move(1, robot_id);
		alarm(0);
		sleep(length);
		alarm(5);
		stop(robot_id);
		alarm(0);
		alarm(5);
		getInfo(robot_id);
		alarm(0);
		alarm(5);
		turn(1, robot_id);
		alarm(0);
		sleep(state1rad);
		alarm(5);
		stop(robot_id);
		alarm(0);
		step_num++;
		printf("\n\n");
	}
	//second polygon
	for (i = 0; i < state2moves; i++) {
		alarm(5);
		move(1, robot_id);
		alarm(0);
		sleep(length);
		alarm(5);
		stop(robot_id);
		alarm(0);
		alarm(5);
		getInfo(robot_id);
		alarm(0);
		alarm(5);
		turn(1, robot_id);
		alarm(0);
		sleep(state2rad);
		alarm(5);
		stop(robot_id);
		alarm(0);
		step_num++;
		printf("\n\n");
	}
}

//take data snapshot
void getInfo(char *robot_id) {
	//open text file for writing data
	FILE *fp;
	char filename[16];
	sprintf(filename, "position-%d.txt", step_num);
	fp = fopen(filename, "w");

	//get text info
	getTextData(fp, "GPS", "GPS", robot_id);
	getTextData(fp, "DGPS", "dGPS", robot_id);

//	lasers are messed up right now
	getTextData(fp, "LASERS", "lasers", robot_id);

	//get and save image
	getImage(robot_id);

	//close file
	fclose(fp);
}

//request an image from the server
void getImage(char *robot_id) {
	printf("Get Image\n");
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "GET IMAGE");

	int success = 0;
	while (!success) {
		//send request
		sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 100 * 1000;
		char* buff = (char*) malloc(buffer_size * sizeof(char));
		memset(buff, 0, buffer_size);
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		//save image
		FILE *fp;
		char filename[16];
		sprintf(filename, "image-%d.png", step_num);
		fp = fopen(filename, "w");
		fwrite(buff, 1, recvLen, fp);
		fclose(fp);
		free(buff);
	}

	message_id++;
}

//get the GPS coordinates from the robot
void getTextData(FILE *out, char *command, char *str, char *robot_id) {
	printf("Get Text\n");
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "GET ");
	strcpy(&(request->data[strlen(robot_id)+5]), command);

	int success = 0;
	while (!success) {
		//send request
		sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 3 * 1000;
		char* buff = (char *) malloc(buffer_size * sizeof(char));
		memset(buff, 0, buffer_size);
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		//save results
		if (strcmp(command, "GPS") == 0) fprintf(stdout, "%s %s\n", str, buff);
		fprintf(out, "%s %s\n", str, buff);
		free(buff);
	}

	message_id++;
}

//move the robot in current direction at given speed
void move(int speed, char *robot_id) {
	printf("MOVE\n");
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "MOVE ");
	char sp[4];
	sprintf(sp, "%d", speed);
	strcpy(&(request->data[strlen(robot_id)+6]), sp);

	int success = 0;
	while (!success) {
		//send request
		sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 3 * 1000;
		char* buff = (char *) malloc(buffer_size * sizeof(char));
		memset(buff, 0, buffer_size);
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		free(buff);
	}

	message_id++;

}

//turn the robot by given degrees
void turn(double degrees, char *robot_id) {
	printf("TURN\n");
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "TURN ");
	char deg[16];
	sprintf(deg, "%4.8lf", degrees);
	strcpy(&(request->data[strlen(robot_id)+6]), deg);

	int success = 0;
	while (!success) {
		//send request
		sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 10 * 1000;
		char* buff = (char *) malloc(buffer_size * sizeof(char));
		memset(buff, 0, buffer_size);
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		free(buff);
	}

	message_id++;
}

//stop the robot by setting its speed to 0
void stop(char *robot_id) {
	printf("STOP\n");
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "STOP");

	int success = 0;
	while (!success) {
		//send request
		sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 3 * 1000;
		char* buff = (char *) malloc(buffer_size * sizeof(char));
		memset(buff, 0, buffer_size);
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		free(buff);
	}

	message_id++;


}
