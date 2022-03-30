#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

struct pipeStruct{
    int numStruct;
    char stringStruct[20];
}

int main(){

    int num;
    char string[20];

    int fd[2];
    if(pipe(fd) == -1){
        printf("Error when creating Pipe");
        return -1;
    }
    
    pid_t p = fork();
    
    if (p > 0){
        printf("Please insert an integer:\n");
        scanf("%d", &num);
        
        printf("Please insert a string with a maximum of 20 chars:\n");
        scanf("%s", string);

        struct pipeStruct fStruct;
        s.numStruct = num;
        strcpy(s.stringStruct, string);

        close(fd[0]);
        write(fd[1], &fStruct, sizeof(fStruct));
        close(fd[1]);

		wait(NULL);

    } else if (p == 0){
        close(fd[1]);

        struct s sonStruct;
		
        read(fd[0], &sonStruct, sizeof(sonStruct));
		close(fd[0]);

		printf("Integer: %d and String: %s\n", sonStruct.numStruct, sonStruct.stringStruct);

		exit(0);
    }

    return 0;
}