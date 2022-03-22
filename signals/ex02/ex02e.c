#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    printf("process: %s command: %s\n", argv[1], argv[2]);
    
    pid_t process = (int) atoi(argv[1]);

    kill(process, atoi(argv[2]));

    return 0;
}