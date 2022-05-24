#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <time.h>

/*
 * STRATEGY
 * 
 * There are 11 semaphores. The processes will execute sequentially through semaphores syncronization the buy_deer() OR buy_chips (chosen randomly using rand()) ;
 * Each will only execute the eat_and_drink() only when the last process will unblock the SEM_11.
 * 
 */ 


#define NUM_SEMAPHORES 11
#define QUANT_PROCESSOS 10
#define SEM_1 0
#define SEM_2 1
#define MIN 0
#define MAX 1

void buy_chips(int processId) {
    printf("%d: buy_chips()\n", processId);
}

void buy_beer(int processId) {
    printf("%d: buy_beer()\n", processId);
}

void eat_and_drink(int processId) {
    printf("%d: eat_and_drink()\n", processId);
}

int main(int argc, char *argv[]) {
	sem_t *sem[NUM_SEMAPHORES];
	char sem_names[NUM_SEMAPHORES][20] = {"SEM_1", "SEM_2", "SEM_3", "SEM_4", "SEM_5", "SEM_6", "SEM_7", "SEM_8", "SEM_9", "SEM_10", "SEM_11"};
	int sem_values[NUM_SEMAPHORES] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int option;
	int value;
	
	for (int i = 0; i < NUM_SEMAPHORES; i++) {
		if ((sem[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i])) == SEM_FAILED) {
			perror("Error at sem_open().\n");
			exit(0);
		}
	}
	
	srand ( time(NULL) );
	
	for(int i = 0; i < QUANT_PROCESSOS; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){
			if (sem_wait(sem[i]) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
			switch(rand() % MAX) {
				case 0: buy_beer(i+1); break;
				case 1: buy_chips(i+1); break;
			}
			if (sem_post(sem[i+1]) == -1) {
        		perror("Error at sem_post().");
        		exit(3);
    		}
			if (sem_wait(sem[NUM_SEMAPHORES - 1]) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
			eat_and_drink(id);
			if (sem_post(sem[NUM_SEMAPHORES - 1]) == -1) {
        		perror("Error at sem_post().");
        		exit(3);
    		}
			exit(0);
        }

    }
	
	for (int i = 0; i < QUANT_PROCESSOS; i++) {
        wait(NULL);
    }
	
    for (int i = 0; i < NUM_SEMAPHORES; i++) {
		if (sem_close(sem[i]) == -1) {
        	perror("Error at sem_close().\n");
        	exit(0);
		}
	}

    for (int i = 0; i < NUM_SEMAPHORES; i++) {
		if (sem_unlink(sem_names[i]) == -1) {
        	perror("Error at sem_unlink().\n");
			exit(0);
		}
	}

	return 0;
}
