/********************************************************************************

Optou-se que o valor inicial dos semáforos que fariam a sincronização entre os
processos fosse 0, devido a que a realização do que foi atribuído a um processo 
estar dependente dos outros. 

*********************************************************************************/


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

	if ((sem1 = sem_open("/s1", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((sem2 = sem_open("/s2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((sem3 = sem_open("/s3", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
    
    for(int i = 0; i < 3; i++){
		
		pid_t pid = fork();

		if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}
		
		if(pid == 0){

			if(i == 0){				
				printf("Sistemas ");
	            fflush(stdout);
                sem_post(sem2);
                sem_wait(sem1);
	            printf("a ");
	            fflush(stdout);
                sem_post(sem2);
				
				exit(0);
			}
			if(i == 1){				
				sem_wait(sem2);
                printf("de ");
	            fflush(stdout);
                sem_post(sem3);
                sem_wait(sem2);
		        printf("melhor ");
		        fflush(stdout);
                sem_post(sem3);

                exit(0);
			}
			if(i == 2){				
				sem_wait(sem3);
                printf("Computadores ");
	            fflush(stdout);
                sem_post(sem1);
                sem_wait(sem3);
		        printf("disciplina! ");
		        fflush(stdout);

                exit(0);
			}
		}
	}

    sem_unlink("/s1");
	sem_unlink("/s2");
	sem_unlink("/s3");

    return 0;
}