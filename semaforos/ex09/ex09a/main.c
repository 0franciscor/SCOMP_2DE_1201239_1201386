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

	sem_t *semChips, *semBeer;

    if ((semChips= sem_open("/SEM_CHIPS", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((semBeer = sem_open("/SEM_BEER", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }
	
	
	for(int i = 0; i < QUANT_PROCESSOS; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){

			if(i == 0) {
				buy_chips("1",semChips);
				if (sem_wait(semChips) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
				if (sem_wait(semBeer) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
        		eat_and_drink("1");
        		exit(0);
			}
	
			if(i == 1) {
				buy_beer("2",semBeer);
        		if (sem_wait(semChips) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
				if (sem_wait(semBeer) == -1) {
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

    if (sem_unlink("/SEM_CHIPS") < 0) {
        printf("Error at sem_unlink()!\n");
        exit(1);
    }

    if (sem_unlink("/SEM_BEER") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }
	
	return 0;
}
