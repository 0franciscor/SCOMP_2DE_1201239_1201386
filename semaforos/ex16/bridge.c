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
#include <string.h>

int main(){

    sem_t *semFirst, *semSecond;

    if ((semFirst = sem_open("firstWay", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semSecond = sem_open("secondWay", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }
    
    printf("The Bridge has now opened!\n");
    int firstValue, secondValue;

    while(1){   
        sem_getvalue(semFirst, &firstValue);
        sem_getvalue(semSecond, &secondValue);

        if(firstValue > 1){
            while(secondValue > 0){
                sem_trywait(semSecond);
                sem_getvalue(semSecond, &secondValue);
            } 
        }


        if(secondValue > 1){
            while(firstValue > 0){
                sem_trywait(semFirst);
                sem_getvalue(semFirst, &firstValue);
            } 
        }

        sem_getvalue(semFirst, &firstValue);
        sem_getvalue(semSecond, &secondValue);

        if(firstValue == 0){
            while(firstValue != 1){
                sem_post(semFirst);
                sem_getvalue(semFirst, &firstValue);
            } 
        }


        if(secondValue == 0){
            while(secondValue != 1){
                sem_post(semSecond);
                sem_getvalue(semSecond, &secondValue);
            } 
        }

        sleep(1);
    }
    
    return 0;
}