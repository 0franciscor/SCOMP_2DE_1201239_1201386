#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* The implemented change allows only the Father Process to be able to fork and clone
other processes, which leads to it forking 4 times. */

int main(){

    int i; 
    pid_t p; 
    
    for (i = 0; i < 4; i++) { 
        if (i == 0 || p > 0) {
            sleep(1);
            p = fork();
        }
    } 
 
    printf("This is the end.\n");

    return 0;
}

