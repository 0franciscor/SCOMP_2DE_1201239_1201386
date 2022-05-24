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


#define NUM_SEMAPHORES 2
#define QUANT_PROCESSOS 2
#define SEM_CHIPS 0
#define SEM_BEER 1

void down(sem_t *sem) {
	if(sem_wait(sem) == -1) {
		perror("Error at sem_wait()!");
		exit(EXIT_FAILURE);
	}
}

void up(sem_t *sem) {
	if(sem_post(sem) == -1) {
		perror("Error at sem_post()!");
		exit(EXIT_FAILURE);
	}
}

/*
 * In methods buy_ships and buy_beer they up the corresponding semaphore twice as there are 2 processes and so that both of them can only proceed once the process bought chips/beer
 */

void buy_chips(char *processName, sem_t *semaforo) {
    printf("%s: buy_chips()\n", processName);
    if (sem_post(semaforo) == -1) {
        perror("Error at sem_post().");
        exit(3);
    }

	if (sem_post(semaforo) == -1) {
        perror("Error at sem_post().");
        exit(3);
    }
}

void buy_beer(char *processName, sem_t *semaforo) {
    printf("%s: buy_beer()\n", processName);
    if (sem_post(semaforo) == -1) {
        perror("Error at sem_post().");
        exit(3);
    }

	if (sem_post(semaforo) == -1) {
        perror("Error at sem_post().");
        exit(3);
    }
}

void eat_and_drink(char *processName) {
    printf("%s: eat_and_drink()\n", processName);
}

int main(int argc, char *argv[]) {
	sem_t *sem[NUM_SEMAPHORES];
	char sem_names[NUM_SEMAPHORES][20] = {"SEM_CHIPS", "SEM_BEER"};
	int sem_values[NUM_SEMAPHORES] = {0, 0};
	
	for (int i = 0; i < NUM_SEMAPHORES; i++) {
		if ((sem[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i])) == SEM_FAILED) {
			perror("Error at sem_open().\n");
			exit(0);
		}
	}
	
	for(int i = 0; i < QUANT_PROCESSOS; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){

			if(i == 0) {
				buy_chips("1", sem[SEM_CHIPS]);
				if (sem_wait(sem[SEM_CHIPS]) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
				if (sem_wait(sem[SEM_BEER]e) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
        		eat_and_drink("1");
        		exit(0);
			}
	
			if(i == 1) {
				buy_beer("2", sem[SEM_BEER]);
        		if (sem_wait(sem[SEM_CHIPS]) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
				if (sem_wait(sem[SEM_BEER]) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
        		eat_and_drink("2");
				exit(0);
			}
        
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
