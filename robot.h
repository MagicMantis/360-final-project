/* Written by Marshall Clyburn
 *
 * robot.h: contains function headers and structs
 * for interacting with the robot on a model level.
 * These functions should be called independently
 * from the UDCP protocol. */

#ifndef ROBOT_H
#define ROBOT_H

typedef struct GPS_DATA_t {
    double x, y, z;          // Cartesion coordinates
    double q1, q2, q3, q4;   // Quaternion
} GPS_DATA;

//request an image from the server
void getImage();

//get the GPS coordinates from the robot
GPS_DATA getGPS();

//get the dGPS coordinates (relative to another point) from the robot
void getDGPS();

//get the laser information from the robot
void getLasers();

//move the robot in current direction at given speed
void move(double speed);

//turn the robot by given degrees
void turn(int degrees);

//stop the robot by setting its speed to 0
void stop();

#endif
