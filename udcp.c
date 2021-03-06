#include "udcp.h"

//reliably send a message to the server
int udcpSend(int sock, struct sockaddr_in info, void *buffer, size_t size, unsigned int id) {
	Response *temp = malloc(sizeof(Response));
	char *data = (char *) buffer;
	int sentLen = 0;
	int msgs_needed = 0;
	int temp_size = size;
	int dataSection = 0;
	while (temp_size > 0) {
		dataSection = temp_size;
		if (dataSection > 988) dataSection = 988;
		temp_size -= dataSection;
		msgs_needed += 1;
	}

	int i = 0, processed = 0;
	while (i < msgs_needed) {
		//format udcp header
		temp->request_id = id;
		temp->num_of_msgs = htonl(msgs_needed);
		temp->sequence_num = htonl(i);		

		//calc data size
		dataSection = size;
		if (dataSection > 988) dataSection = 988;
		size -= dataSection;

		//move data into struct
		memcpy(temp->data, &data[processed], dataSection);
		processed += dataSection;

		//send data
		sentLen = sendto(sock, temp, 12+dataSection, 0, (struct sockaddr*) &info, sizeof(info));
		i++;
	}
	return sentLen;
}

//reliably receive a message from the server
int udcpRecv(int sock, void *buffer, unsigned int id) {

	Response *temp = malloc(sizeof(Response));
	Response *msgs = (Response *) buffer;

	struct sockaddr fromAddr;
	socklen_t fromAddrLen = sizeof(fromAddr);

	int bytes = -1;
	while (bytes < 0) {
		bytes = recvfrom(sock, temp, 1000, 0, &fromAddr, &fromAddrLen);
		temp->data_size = bytes - (sizeof(unsigned int) * 3);
	}

	unsigned int num_msgs = temp->num_of_msgs;
	unsigned int seq = temp->sequence_num;

	num_msgs = ntohl(num_msgs);
	seq = ntohl(seq);

	memcpy(&msgs[seq], temp, sizeof(Response));
	int i = 1;
	while (i < num_msgs) {
		bytes = recvfrom(sock, temp, 1000, 0, &fromAddr, &fromAddrLen);
		if (bytes > 0) {
			if (temp->request_id != id) continue;
			temp->data_size = bytes - (sizeof(unsigned int) * 3);
			memcpy(&msgs[ntohl(temp->sequence_num)], temp, sizeof(Response));
			i++;
		}
	}

	uint8_t *unpack = malloc(num_msgs * 1000);
	int loc = 0;
	for (i = 0; i < num_msgs; i++) {
		memcpy(&unpack[loc], &(msgs[i].data), msgs[i].data_size);
		loc += msgs[i].data_size;
	}
	memset(buffer, 0, num_msgs * 1000);
	memcpy(buffer, unpack, (size_t) loc);
	free(unpack);
	free(temp);

	return loc;
}

