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

    sem_t *semWriter;
 
    if ((semWriter = sem_open("/sema5", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(1);
    }

    pid_t pid = fork();

    if(pid == 0){		
		printf("I'm the child!\n");
		sem_post(semWriter);
        exit(0);
	}else if (pid > 0) {	
		sem_wait(semWriter);
		printf("I'm the father!\n");
        sem_post(semWriter);
    } else {
        perror("Fork Falhou");
        exit(2);
    }

    wait(NULL);

    if (sem_unlink("/sema5") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    return 0;
}