CPSC 3600 Final Project
=======================
Moving robots in regular polygons with the experimental UDCP protocol.

About
-----
360 Final Project for Joseph Savold, Marshall Clyburn, and Jackson Wood

Robot Information
-----------------
ID: `0theory`

Camera ID: `19`

How To Run
----------
To run the client, the parameters must be entered in the following order: 
<IP of the middleware> <Port of middleware> <robot ID> <Length of sides> <Num of sides>

To run the middleware, the parameters must be eneterd in the following order:
<Port to listen to> <IP of robot server> <robot ID> <Image ID>

Comments
--------
The server is stateless and can continue to communicate with a client through multiple sessions while the client will run through the routine once and then exit.

Known Problems
--------------
One of the known problems is the robot is slightly inaccurate when it moves. When it is finished with its route, it is off from the beginning point by a couple tenths of a meter.

Design
------
We split the program into two programs, the middleware server and the client. The client was in charge of generating the commands to send to the middleware. It abided by the specifications set by the class for the project. It sent UDP packets of 1000 bytes to the middleware. It is able to detect if something went wrong when communicating with the middleware. If anything goes wrong, it timeouts. 

Our middleware is responsible for generating the HTTP packets to send to a server that controls a robot. If the TCP packet received from the server is larger than 988 bytes, it will fragment the data into multiple UDCP packets to send to the client. The client is then able to repackage them in the correct order. 
