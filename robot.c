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
	int state1degrees = 360/state1moves;
	int state2degrees = 360/state2moves;

	//initial data snapshot
	getInfo(robot_id);
	step_num++;

/*	//action loop
	int i;
	for (i = 0; i < state1moves; i++) {
		move(1, robot_id);
		//TODO: wait for move to finish
		stop(robot_id);
		getInfo(robot_id);
		turn(state1degrees, robot_id);
		stop(robot_id);
		step_num++;
	}
	//second polygon
	for (i = 0; i < state2moves; i++) {
		move(1, robot_id);
		//TODO: wait for move to finish
		stop(robot_id);
		getInfo(robot_id);
		turn(-state2degrees, robot_id);
		stop(robot_id);
		step_num++;
	} */
}

//take data snapshot
void getInfo(char *robot_id) {
	//open text file for writing data
	FILE *fp;
	char filename[16];
	sprintf(filename, "position-%d.txt", step_num);
	fp = fopen(filename, "w");

	//get text info
	getGPS(fp, robot_id);
	getDGPS(fp, robot_id);
	getLasers(fp, robot_id);

	//get and save image
	getImage(robot_id);

	//close file
	fclose(fp);
}

//request an image from the server
void getImage(char *robot_id) {
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "GET IMAGE");

	int success = 0;
	while (!success) {
		//send request
		int sendLen = sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 100 * 1000;
		char* buff = (char*) malloc(buffer_size * sizeof(char));
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
void getGPS(FILE *out, char *robot_id) {
	Request *request = (Request *) malloc(sizeof(Request));
	request->request_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "GET GPS");

	int success = 0;
	while (!success) {
		//send request
		int sendLen = sendto(sock, request, sizeof(Request), 0, servAddr, sizeof(*servAddr));

		//recieve response
		const unsigned int buffer_size = 2 * 1000;
		char* buff = (char *) malloc(buffer_size * sizeof(char));
		int recvLen = udcpRecv(sock, buff, message_id);
		if (recvLen == -1) continue;
		success = 1;

		//save results
		fprintf(stdout, "GPS %s\n", buff);
		fprintf(out, "GPS %s\n", buff);
		free(buff);
	}

	message_id++;
}

//get the dGPS coordinates (relative to another point) from the robot
void getDGPS(FILE *out, char *robot_id) {

}

//get the laser information from the robot
void getLasers(FILE *out, char *robot_id) {

}

//move the robot in current direction at given speed
void move(double speed, char *robot_id) {
	printf("move %lf\n", speed);
}

//turn the robot by given degrees
void turn(int degrees, char *robot_id) {
	printf("turn %d\n", degrees);
}

//stop the robot by setting its speed to 0
void stop(char *robot_id) {
	printf("stop\n");
}
