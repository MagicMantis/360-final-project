#include "utility.h"
#include "udcp.h"
#include "robot.h"

int main(int argc, char *argv[]) {

	int sock; //socket id
	struct sockaddr_in servAddr;
	struct hostent *thehost;

	//usage error check
	if (argc != 6) printf("Usage error: ./robotClient <ip/host> <port> <robot_id> <length> <number_of_sides>\n");

	char *server = argv[1];
	int port = atoi(argv[2]);
	char *robotID = argv[3];
	int length = atoi(argv[4]);
	int N = atoi(argv[5]);


	if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		printf("Can't set up socket\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(server);
	servAddr.sin_port = htons(port);

	if(servAddr.sin_addr.s_addr == -1){
		thehost = gethostbyname(server);
		servAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);
	}

	actionLoop(sock, (struct sockaddr *) &servAddr, length, N, robotID);

	exit(0);
}
