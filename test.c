#include <stdio.h>
#include <stdlib.h>
#include "robot.h"

int main(int argc, char *argv[]) {
	int len = atoi(argv[1]);
	int side = atoi(argv[2]);
	actionLoop(len, side);
}
