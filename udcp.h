/* udcp.h: This header contains function prototypes for
 * functions used by the client to establish a connection
 * and to control the robot */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct GPS_DATA_T {
   double x, y, z;		/* 3 dimensional coords */
   double pitch, yaw, roll;	/* angular position */
   double q1, q2, q3, q4;	/* quaternion stuff? */
} GPS_DATA;

//connects to a server, returns 0 if successful and -1 if unsuccessful
int connect(//TODO parameters);

//request an image from the server
void getImage();

//get the GPS coordinates from the robot
GPS_DATA_T getGPS();

//get the dGPS coordinates (relative to another point) from the robot
void getDGPS();

//get the lazer information from the robot
void getLazers();

//move the robot in current direction at given speed
void move(double speed);

//turn the robot by given degrees
void turn(int degrees);

//stop the robot by setting its speed to 0
void stop();
