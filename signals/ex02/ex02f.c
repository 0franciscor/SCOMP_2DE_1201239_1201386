#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {    
    
    pid_t process = (int) atoi(argv[1]);

    sleep(2);

    kill(process, 19);
    sleep(1);
    
    kill(process, 18);
    sleep(1);

    kill(process, 9);
    sleep(1);

    return 0;
}