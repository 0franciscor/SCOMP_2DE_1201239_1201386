#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

int main(){

    sem_t *semRead, *semWrite;

    if ((semRead = sem_open("1r", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_openR\n");
        exit(1);
    }

    if ((semWrite = sem_open("1w", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_openW\n");
        exit(1);
    }
    
    FILE *input = fopen("input.txt","w"), *output = fopen("output.txt","w");

    for (int i = 0; i < 200; i++){
        fprintf(input, "%d\n", (rand() % 100));
    }

    pid_t p;
    int array[200];

    fclose(input); fclose(output);

    for (int i = 0; i < 8; i++){
        p = fork();

        if (p == 0){
            input = fopen("input.txt", "r");
            output = fopen("output.txt", "a");
            sem_wait(semRead);

            for (int i = 0; i < 200; i++){
                fscanf(input, "%d\n", &array[i]);
            }
            sem_post(semRead);
            
            sem_wait(semWrite);
            fprintf(output, "Process ID: %d\n", getpid());
            for (int i = 0; i < 200; i++){
                fprintf(output, "%d\n", array[i]);
            } 
            sem_post(semWrite);
            fclose(input);
            fclose(output);

            exit(0);
        }
    }

    for (int i = 0; i < 8; i++){
        wait(NULL);
    }    

    sem_close(semRead);
    sem_close(semWrite);
    sem_unlink("1r");
    sem_unlink("1w");

    return 0;
}