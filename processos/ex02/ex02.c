#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    
    printf("I'm..\n");

    pid_t p1 = fork();

    if(p1 == 0){
        printf("I'll never join you!\n");        
    } else if (p1 > 0){
        printf("the..\n");
        
        pid_t p2 = fork();

        if(p2 == 0){
            printf("I'll never join you!\n");        
        } else if (p2 > 0){
            printf("father!\n");

            pid_t p3 = fork();

            if(p3 == 0){
                printf("I'll never join you!\n");
            } else if (p3 > 0){
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }
    }

    return 0;
}