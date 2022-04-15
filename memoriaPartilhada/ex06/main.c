#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define ARRAY_SIZE 1000000

int main(){
    
    clock_t t;
    t = clock();

    int fd[2];

    if(pipe(fd) == -1){
        perror("There was an error when creating the Pipe.\n");
        return -1;
    }

    pid_t p = fork();

    if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

    if(p == 0){
        int arrayRead[ARRAY_SIZE];
        close(fd[1]);
        read(fd[0], &arrayRead, sizeof(arrayRead));
        close(fd[0]);
        exit(0);
    }

    int array[ARRAY_SIZE];
    for(int i = 0; i < ARRAY_SIZE; i++){
        array[i] = 1;
    }

    close(fd[0]);
    write(fd[1], &array, sizeof(array));
    close(fd[1]);

    wait(NULL);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    
    printf("Tempo de execução: %.2f\n", time_taken);

    return 0;
}