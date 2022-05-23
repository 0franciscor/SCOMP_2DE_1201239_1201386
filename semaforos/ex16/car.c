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
#include <time.h>

int main(){

    srand(time(0));
    int num = rand() % 2; // 0 IF FIRST WAY, 1 IF SECOND WAY

    sem_t *semFirst, *semSecond;

    if ((semFirst = sem_open("firstWay", 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semSecond = sem_open("secondWay", 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if(num == 0){
        printf("Crossing the Bridge on the first way.\n");
        
        sem_wait(semSecond);

        sem_post(semFirst);
        sleep(10);
        sem_wait(semFirst);
    }

    if(num == 1){
        printf("Crossing the Bridge on the second way.\n");

        sem_wait(semFirst);

        sem_post(semSecond);
        sleep(10);
        sem_wait(semSecond);
    }

    printf("Bridge Crossed\n");

    return 0;
}