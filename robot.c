#include <stdio.h>
#include "robot.h"

void actionLoop(int length, int sides) {

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
void getImage() {
	printf("get image\n");
}

//get the GPS coordinates from the robot
GPS_DATA getGPS() {

}

//get the dGPS coordinates (relative to another point) from the robot
void getDGPS() {

}

//get the laser information from the robot
void getLasers() {

}

//move the robot in current direction at given speed
void move(double speed) {
	printf("move %lf\n", speed);
}

//turn the robot by given degrees
void turn(int degrees) {
	printf("turn %d\n", degrees);
}

//stop the robot by setting its speed to 0
void stop() {
	printf("stop\n");
}
