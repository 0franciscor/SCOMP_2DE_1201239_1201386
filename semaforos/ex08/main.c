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

    sem_t *semChild, *semFather;
    
    if ((semChild = sem_open("8c", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semFather = sem_open("8f", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    pid_t p = fork();

    if(p == -1){ perror("There was an error when forking the process\n"); exit(-1);}
    
    if(p == 0){
        while(1){
            sem_wait(semChild);
            printf("c");
            fflush(stdout);
            sem_post(semFather);
        }
        
        exit(0);
    }

    while(1){
        sem_wait(semFather);
        printf("s");
        fflush(stdout);
        sem_post(semChild);
    }

    sem_close(semChild);
    sem_close(semFather);
    shm_unlink("8c");
    shm_unlink("8f");

    wait(NULL);

    return 0;
}