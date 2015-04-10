#include "utility.h"

void failProgram(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
    return;
}
