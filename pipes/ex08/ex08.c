#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

struct gameStruct {
    int round;
    char message[10];
};

int main(){

    struct gameStruct gStruct;
    strcpy(gStruct.message, "Win");

    int fd[2];
    if(pipe(fd) == -1){
        printf("Error when creating Pipe");
        return -1;
    }

    pid_t p;
    pid_t processArray [10];
    for(int i = 0; i < 10; i++){
        p = fork();
        processArray[i] = p;

        if(p == 0){
            close(fd[1]);
            struct gameStruct sStruct;
            if(read(fd[0], &sStruct, sizeof(sStruct)) != 0){
                printf("%s - %d\n", sStruct.message, sStruct.round);
                exit(i + 1);
            }
            close(fd[0]);
        }
    }

    int roundArray [10];
    for(int i = 0; i < 10; i++){
        gStruct.round = i + 1;

        close(fd[0]);
        write(fd[1], &gStruct, sizeof(gStruct));

        int status;
        processArray[i] = wait(&status);
        roundArray[i] = WEXITSTATUS(status);    
        sleep(2);
    }

    close(fd[1]);
    printf("\nResults:\n");

    for(int i = 0; i < 10; i++){
        printf("Process: %d Won round: %hd\n", processArray[i], roundArray[i]);
    }

    return 0;
}