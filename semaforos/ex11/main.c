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

#define CAPACIDADE 200
#define QUANT_PORTAS 3
#define PASSAGEIROS 400

int main(){

    sem_t *semPortas, *semCap;

    if ((semPortas = sem_open("/s111", O_CREAT | O_EXCL, 0644, QUANT_PORTAS)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((semCap = sem_open("/s112", O_CREAT | O_EXCL, 0644, CAPACIDADE)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}


    for(int i = 0; i < PASSAGEIROS; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if(pid == 0){
            
            sem_wait(semCap);
            sem_wait(semPortas);
            printf("O passageiro %d entrou no comboio!\n", i);
            sem_post(semPortas);

            if( i >= 100 && i < 400 ) {
                sleep(5);

                sem_wait(semPortas);
                printf("O passageiro %d saiu do comboio!\n", i);
                sem_post(semPortas);
                sem_post(semCap);
            }
            
            exit(0);
        }

	
    }

    for(int i = 0; i < PASSAGEIROS; i++){
        wait(NULL);
    }



    if (sem_unlink("/s111") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/s112") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }
    
    return 0;
}