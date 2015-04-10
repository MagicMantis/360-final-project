/* Written by Joseph Savold
 *
 * udcp.h: This header contains function prototypes for
 * functions used by the client to establish a connection
 * and to control the robot */

#include <sys/types.h>
#include <sys/socket.h>

// connects to a server, returns 0 if successful and -1 if unsuccessful
// TODO: needs parameters
int connect();
