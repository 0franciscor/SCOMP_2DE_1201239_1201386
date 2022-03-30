#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(){

    int fd[2];
    if(pipe(fd) == -1){
        printf("Error when creating Pipe");
        return -1;
    }
    
    pid_t p = fork();
    
    if (p > 0) {

        char *filename = "Readme.txt";
        FILE *fp = fopen(filename, "r");

        if (fp == NULL){
            printf("Error: could not open file %s", filename);
            return 1;
        }

        char string[100];

        while (fgets(string, 100, fp));
        fclose(fp);

        close(fd[0]);
    
        write(fd[1], &string, sizeof(string));
        close(fd[1]);

		wait(NULL);

    } else if (p == 0) {
        close(fd[1]);

        char string[100];
		
        read(fd[0], &string, sizeof(string));
		close(fd[0]);

		printf("String: %s\n", string);

		exit(0);
    }

    return 0;
}