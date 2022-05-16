#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>

int main(){

    sem_t *sem1,*sem2,*sem3;

	if ((sem1 = sem_open("/sem1", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((sem2 = sem_open("/sem2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((sem3 = sem_open("/sem3", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
    pid_t pid = fork();

    if (pid < 0){
        perror("Fork Falhou");
        exit(2);
    } else if (pid == 0){
        sem_wait(sem1);
        printf("de ");
	    fflush(stdout);
        sem_post(sem2);
        sem_wait(sem1);
		printf("melhor ");
		fflush(stdout);
        sem_post(sem2);

        exit(0);
    } else {
        pid_t p = fork();
        if (p < 0){
            perror("Fork Falhou");
            exit(2);
        } else if (pid == 0){
            sem_wait(sem2);
            printf("Computadores ");
	        fflush(stdout);
            sem_post(sem3);
            sem_wait(sem2);
		    printf("disciplina! ");
		    fflush(stdout);
            sem_post(sem3);

            exit(0);
        }else{
            printf("Sistemas ");
	        fflush(stdout);
            sem_post(sem1);
            sem_wait(sem3);
			printf("a ");
			fflush(stdout);
            sem_post(sem1);
        }
    }
    

    sem_unlink("/sem1");
	sem_unlink("/sem2");
	sem_unlink("/sem3");

    return 0;
}