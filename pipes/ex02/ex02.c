#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

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

        close(fd[0]);
        
        write(fd[1], &num, sizeof(num));
        write(fd[1], &string, sizeof(string));

        close(fd[1]);

		wait(NULL);

    } else if (p == 0){
        close(fd[1]);
		
        read(fd[0], &num, sizeof(int));
        read(fd[0], &string, sizeof(string));
		close(fd[0]);

		printf("Integer: %d and String: %s\n", num, string);

		exit(0);
    }

    return 0;
}