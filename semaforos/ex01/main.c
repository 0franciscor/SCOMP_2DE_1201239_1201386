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

    if ((semRead = sem_open("/1r", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_openR\n");
        exit(1);
    }

    if ((semWrite = sem_open("/1w", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_openW\n");
        exit(1);
    }
    
    FILE *file = fopen("input.txt","wb");

    for (int i = 0; i < 1600; i++){
        fprintf(file, "%d\n", (rand() % 100));
    }
    fclose(file);

    pid_t p;
    int array[200];

    for (int i = 0; i < 8; i++){
        p = fork();

        if (p == 0){
            
            FILE *input = fopen("input.txt", "r"), *output = fopen("output.txt", "a");
            int start = i * 200, end = start + 200;
            
            sem_wait(semRead);
            int index = 0;
            for (int i = start; i < end; i++){
                fscanf(input, "%d\n", &array[index]);            
                index++;
            }
            sem_post(semRead);
            
            sem_wait(semWrite);
            index = 0;
            for (int i = start; i < end; i++){
                fprintf(output, "%d\n", array[index]);
                index++;
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
    sem_unlink("/1r");
    sem_unlink("/1w");

    return 0;
}