#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define ARRAY_SIZE 1000000

int main(){
    
    time_t start = time(NULL);
    
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
        close(fd[1]);
        int num;
        for(int i = 0; i < ARRAY_SIZE; i++){
            read(fd[0], &num, sizeof(int));
            printf("O número é: %d\n", num);
        }
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    int array[ARRAY_SIZE];
    for(int i = 0; i < ARRAY_SIZE; i++){
        array[i] = i;
        write(fd[1], &array[i], sizeof(int));
    }
    close(fd[1]);

    wait(NULL);

    time_t end = time(NULL);
    int total = end - start;
    printf("Tempo de execução: %ds\n", total);
    

    return 0;
}