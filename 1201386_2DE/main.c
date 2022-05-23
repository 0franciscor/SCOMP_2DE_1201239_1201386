/********************************************************************************

VERSÃO 2

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

#define QUANT_FILHOS 3
#define TAMANHO_VETOR 100

#define N1 3
#define N2 3
#define N3 3

typedef struct{
  int vetor[TAMANHO_VETOR];
} info;

typedef struct{
  info vec[N1];
} dados;


int main(){

    // Criação da memória partilhada

    int size = sizeof(dados); 
	dados *infoPartilhada;

    int fd = shm_open("/teste",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
	  perror("Creating or opening shared memory failure");
		shm_unlink("/ex9");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (dados*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    // Criação dos semáforos

    sem_t *semTeste, *semTeste1,*semTeste2,*semTeste3;

    // Semáforo de exclusão mútua 

    if ((semTeste = sem_open("/sTeste", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    // Semáforos de sincronização de processos

	if ((semTeste1 = sem_open("/sTeste1", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((semTeste2 = sem_open("/sTeste2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((semTeste3 = sem_open("/sTeste3", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    // Criação do pipe 

    int pp[2];
	if(pipe(pp) == -1) {
        perror("Pipe Falhou");
        return 1;
    }

    // Criação dos processos filho 
    for(int i = 0; i < QUANT_FILHOS; i++){
		
		pid_t pid = fork();

		if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}
		
		if(pid == 0){

            //Processo A1
            if(i == 0){
                for(int j = 0; j < N1; j++){

                    pid_t p = fork();

		            if(p == -1){
			            perror("Fork Falhou");
                        exit(2);
		            }   

                    if(p == 0){

                        if (sem_wait(semTeste) == -1) {
                            perror("Error at sem_wait().");
                            exit(4);
                        }
                
                        srand ((unsigned int) time (NULL));

                        for(int a = 0; a < TAMANHO_VETOR; a++){
    	                    infoPartilhada->vec[j].vetor[a] = rand()% 101;
  	                    }


                        unsigned int seconds = rand()% 2000001; 
                        int usleep(seconds);
                       

                        if (sem_post(semTeste) == -1) {
                            perror("Error at sem_post().");
                            exit(3);
                        }

                        exit(0);
                    }
                }
                
                for(int j = 0; j < N1; j++){
                    wait(NULL);
                }

                if (sem_post(semTeste1) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                }

                exit(0);
            }

            //Processo A2
            if(i == 1){

                if (sem_wait(semTeste1) == -1) {
                    perror("Error at sem_wait().");
                    exit(4);
                }

                for(int j = 0; j < N2; j++){

                    pid_t p = fork();

		            if(p == -1){
			            perror("Fork Falhou");
                        exit(2);
		            }   

                    if(p == 0){

                        if (sem_wait(semTeste) == -1) {
                            perror("Error at sem_wait().");
                            exit(4);
                        }

                        for(int b = 0; b < TAMANHO_VETOR; b++){
                            int aux = infoPartilhada->vec[j].vetor[b];
    	                    infoPartilhada->vec[j].vetor[b] = aux + 10;
  	                    }

                        unsigned int mSeconds = rand()% 3000001; 
                        int usleep(mSeconds);
                        

                        if (sem_post(semTeste) == -1) {
                            perror("Error at sem_post().");
                            exit(3);
                        }

                        exit(0);
                    }
                }

                for(int j = 0; j < N2; j++){
                    wait(NULL);
                }

                if (sem_post(semTeste2) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                }

                exit(0);
            }

            //Processo A3
            if(i == 2){

                if (sem_wait(semTeste2) == -1) {
                    perror("Error at sem_wait().");
                    exit(4);
                }

                struct dados informacao = NULL;

                for(int j = 0; j < N3; j++){

                    pid_t p = fork();

		            if(p == -1){
			            perror("Fork Falhou");
                        exit(2);
		            }   

                    if(p == 0){

                        close(pp[0]);

                        if (sem_wait(semTeste) == -1) {
                            perror("Error at sem_wait().");
                            exit(4);
                        }

                        for(int c = 0; c < TAMANHO_VETOR; c++){

                            int aux = infoPartilhada->vec[j].vetor[c];
                            informacao.vec[j].vetor[c]= aux + 100;

  	                    }
                        unsigned int second = rand()% 2000001; 
                        int usleep(seconds);
                        
                        if (sem_post(semTeste) == -1) {
                            perror("Error at sem_post().");
                            exit(3);
                        }

                        exit(0);
                    }
                }

                for(int j = 0; j < N3; j++){
                    wait(NULL);
                }

                write(pp[1], &informacao, sizeof(informacao));
			    close(pp[1]);

                if (sem_post(semTeste3) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                }

                exit(0);
            }

        }

    }

    for(int l = 0; l < QUANT_FILHOS; l++){
        wait(NULL);
    }

    if (sem_wait(semTeste3) == -1) {
        perror("Error at sem_wait().");
        exit(4);
    }

    close(pp[1]);
    struct pipeStruct resultado;
    read(pp[0], &resultado, sizeof(resultado));

    for(int temp = 0; temp < N1; temp++){
        printf("----- VETOR -----\n");
        for(int k = 0; k<TAMANHO_VETOR;k++){
            printf("Posição %d: %d\n",k,resultado.vec[temp].vetor[k]);
        }
    }

    close(pp[0]);

    // fechar memória partilhada

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }

    // fechar semáforos

    if (sem_unlink("/sTeste") == -1 ){
        perror("Error in sem_unlink()");
        exit(6);
    }

    if (sem_unlink("/sTeste1") == -1 ){
        perror("Error in sem_unlink()");
        exit(6);
    }

    if (sem_unlink("/sTeste2") == -1 ){
        perror("Error in sem_unlink()");
        exit(6);
    }

    if (sem_unlink("/sTeste3") == -1 ){
        perror("Error in sem_unlink()");
        exit(6);
    }

    return 0;
}