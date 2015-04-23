#include "utility.h"
#include "udcp.h"
#include "robot.h"

int message_id = 0;

void actionLoop(int length, int sides, char *robot_id) {

	int state1moves = sides;
	int state2moves = sides-1;
	int state1degrees = 360/state1moves;
	int state2degrees = 360/state2moves;

	int i;
	for (i = 0; i < state1moves; i++) {
		move(1);
		stop();
		getImage();
		turn(state1degrees);
		stop();
		getImage();
	}

	for (i = 0; i < state2moves; i++) {
		move(1);
		stop();
		getImage();
		turn(-state2degrees);
		stop();
		getImage();
	}

}

//request an image from the server
void getImage(char *robot_id) {
	Request *request = (Request *) malloc(sizeof(Request));
	request->robot_id = htonl(message_id);
	strcpy(request->data, robot_id);
	strcpy(&(request->data[strlen(robot_id)+1]), "IMAGE");

	//TODO implement sending / confirmation

	message_id++;
}

//get the GPS coordinates from the robot
GPS_DATA getGPS(char *robot_id) {

}

//get the dGPS coordinates (relative to another point) from the robot
void getDGPS(char *robot_id) {

}

//get the laser information from the robot
void getLasers(char *robot_id) {

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
