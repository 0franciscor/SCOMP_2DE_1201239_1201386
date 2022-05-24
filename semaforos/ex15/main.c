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

#define QUANT_PROCESSOS 25
#define CAPACIDADE 5
#define SHOW_RUNNING_TIME 5
#define WAITING_TIME 2


int main(int argc, char *argv[]) {

    sem_t *show, *capacidade, *entrada, *saida;

    if ((show = sem_open("/show", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((capacidade = sem_open("/capacidade", O_CREAT, 0644, CAPACIDADE)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((entrada = sem_open("/entrada", O_CREAT, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((saida = sem_open("/saida", O_CREAT, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    for(int i = 0; i < QUANT_PROCESSOS ; i++) {
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

		if(pid == 0) {
            
        }

    }





    if (sem_unlink("/show") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/capacidade") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/entrada") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/saida") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }




    return 0;
}