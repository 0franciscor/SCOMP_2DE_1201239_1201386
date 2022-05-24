/********************************************************************************

Padrão:

Pseudo-Código:
	cria 3 semáforos
	criação e iniciaização da memória partilhada 
	inicializa o buffer circular
    cria os filhos 

    

    pai espera que os filhos esperem de executar


	elimina os semáforos e a área de memória partilhada

*********************************************************************************/

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
#define QUANT_SALAS 4
#define CAPACIDADE_SALA 5
#define SHOW_TIME 5
#define WAITING_TIME 1


int main(int argc, char *argv[]) {

    if (sem_unlink("/entrada") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/capacidade") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    sem_t *porta, *capacidade;

    // NÃO PERMITIR QUE ENTREM PESSOAS DURANTE O SHOW

    if ((porta = sem_open("/porta", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    // NÃO PERMITIR Ultrapassar o limite 
    if ((capacidade = sem_open("/capacidade", O_CREAT, 0644, CAPACIDADE_SALA*QUANT_SALAS)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    int temp = QUANT_PROCESSOS;

    for(int i = 0; i < QUANT_PROCESSOS ; i++) {
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

		if(pid == 0) {
            while(1){
                
	            if (sem_wait(capacidade) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                if (sem_wait(porta) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
	                
	            printf("Entrei na sala!\n");

                if (sem_post(porta) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

	            sleep(SHOW_TIME);

	            printf("Sai da sala!\n");

	            if (sem_post(capacidade) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

	        }
            temp--;
            exit(1);

        } else {
            while (temp>0){

                printf("Portas Abertas!\n");
	            if (sem_post(porta) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
	            sleep(WAITING_TIME);
	            if (sem_wait(porta) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
	            printf("Espetáculo começou!\n");
	            sleep(SHOW_TIME);
	
	            printf("Espetáculo Terminou!\n");
            }
            
        } 
            
    }

    for (int i = 0; i < QUANT_PROCESSOS; i++){
		wait(NULL);
	}

    if (sem_unlink("/entrada") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/capacidade") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    return 0;
}