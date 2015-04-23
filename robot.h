/* Written by Marshall Clyburn
 *
 * robot.h: contains function headers and structs
 * for interacting with the robot on a model level.
 * These functions should be called independently
 * from the UDCP protocol. */

#ifndef ROBOT_H
#define ROBOT_H

/* makes commands to make the robot trace two polygons
 * parameters:
 *  s: socket for udcp communication
 *  addr: address of server
 *  length: length of each side in the polygons
 *  num_of_sides: sides of the larger polygon
 *  robot_id: key for the server to accept communication */
void actionLoop(int s, struct sockaddr *addr, int length, int num_of_sides, char *robot_id);

//calls all the data get commands
void getInfo(char *robot_id);

//request an image from the server
void getImage(char *robot_id);

//get the GPS coordinates from the robot
void getTextData(FILE *out, char *command, char *str, char *robot_id);

//get the laser information from the robot
void getLasers(FILE *out, char *robot_id);

//move the robot in current direction at given speed
void move(double speed, char *robot_id);

//turn the robot by given degrees
void turn(int degrees, char *robot_id);

//stop the robot by setting its speed to 0
void stop(char *robot_id);

#endif
