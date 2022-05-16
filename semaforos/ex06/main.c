#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

int main(){

    sem_t *semFather, *semChild;
 
    if ((semFather = sem_open("6.1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(1);
    }

    if ((semChild = sem_open("6.2", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(1);
    }

    pid_t p = fork();
    if(p == 0){	
        int i = 0;	
        while(i < 7){
            sem_wait(semChild);
            printf("I'm the child!\n");
            fflush(stdout);
            sleep(1);
            sem_post(semFather);
            i++;
        }
        exit(0);   
    }
    int i = 0;
    while(i < 7){
        sem_wait(semFather);
        printf("I'm the father!\n");
        fflush(stdout);
        sleep(1);
        sem_post(semChild);
        i++;
    }
    
    if (sem_close(semFather) == -1){
        perror("Error in sem_close()");
        exit(1);
    }

    if (sem_close(semChild) == -1){
        perror("Error in sem_close()");
        exit(1);
    }

    if (sem_unlink("6.1") == -1){
        perror("Error in sem_unlink()");
        exit(1);
    }

    if (sem_unlink("6.2") == -1){
        perror("Error in sem_unlink()");
        exit(1);
    }

    wait(NULL);

    return 0;
}