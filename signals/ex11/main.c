#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "struct.h"

#define MAX_CMD 2

int main(int argc, char* argv[]) {	

    comando vec[MAX_CMD];
	strcpy(vec[0].cmd, "");
	vec[0].tempo = 10;
	strcpy(vec[1].cmd, "");
	vec[1].tempo = 2;

    return 0;
}