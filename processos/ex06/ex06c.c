#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

    int i;
    pid_t p; 
    
    for (i = 0; i < 4; i++) { 
        if (i == 0 || p > 0) {
            sleep(1);
            p = fork();
            if(p % 2 == 0)
                waitpid(p, NULL, 0);
        }
    } 
 
    printf("This is the end.\n");

    return 0;
}
