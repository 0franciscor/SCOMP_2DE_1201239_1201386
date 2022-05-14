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

    sem_t *sem[8];
    char name[8][10];

    for (int i = 0; i < 8; i++) {
        sprintf(name[i], "2%d", i);
    
        if ((sem[i] = sem_open(name[i], O_CREAT, 0644, 1)) == SEM_FAILED) {
            perror("Error in sem_open function\n");
            exit(1);
        }
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
            sem_wait(sem[i]);

            FILE *input = fopen("input.txt", "r"), *output = fopen("output.txt", "a");
            int start = i * 200, end = start + 200;            
            
            int index = 0;
            for (int j = start; j < end; j++){
                fscanf(input, "%d\n", &array[index]);            
                index++;
            }
            
            index = 0;
            for (int j = start; j < end; j++){
                fprintf(output, "%d\n", array[index]);
                index++;
            } 

            fclose(input);
            fclose(output);

            sem_post(sem[i]);

            sem_close(sem[i]);
            sem_unlink(name[i]);
            
            exit(0);
        }
    }

    for (int i = 0; i < 8; i++){
        wait(NULL);
    }

    return 0;
}