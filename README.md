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

To-Do List
----------
- Make client generate commands (robot.c's actionLoop())
- Implement robot functions (robot.c's functions)
- Add in timeouts for the client
- check in middle parameters with the one it receives in the message
- Deal with message ID's (maybe can do something with the action loop)
- Write images to a file in client when they are received (can be done with the code in client.c, but needs to be adapted to dynamically save files. Try placing in robot.c's getImage())

