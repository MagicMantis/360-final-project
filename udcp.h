/* Written by Joseph Savold
 *
 * udcp.h: This header contains function prototypes for
 * functions used by the client to establish a connection
 * and to control the robot */

#include <sys/types.h>
#include <sys/socket.h>

//reliably send a message to the server
int udcpSend(int sock, struct addrinfo info);

//reliably receive a message from the server
int udcpRecv(int sock);
