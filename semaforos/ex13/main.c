/********************************************************************************

Padrão: Produtor/Consumidor
Exemplo: Farmácia

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

#define BUF_SIZE 10
#define PRODUTORES 2

typedef struct {
    int buffer[BUF_SIZE];
    int head;
    int tail;
    int isFull;
} SharedDataStruct;


int main(){

    // Criação dos semáforos

    sem_t *canWrite, *canRead, *index_access;

    if ((canWrite = sem_open("/canwrite", O_CREAT | O_EXCL, 0644, BUF_SIZE)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((canRead = sem_open("/canread", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((index_access = sem_open("/access", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	} 
    
    // Criação da memória partilhada

    int size = sizeof(SharedDataStruct); 
	SharedDataStruct *infoPartilhada;

    int fd = shm_open("/ex13",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
	  perror("Creating or opening shared memory failure");
		shm_unlink("/ex13");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (SharedDataStruct*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    // Inicialização

    for (int i = 0; i < BUF_SIZE; ++i) {
        infoPartilhada->buffer[i] = 0;
    }
    infoPartilhada->head = 0;
    infoPartilhada->tail = 0;
    infoPartilhada->isFull = 0;


    for(int i = 0; i < PRODUTORES; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){
            // Produtores

            for(int j = 0; j < 30; j++) {
            if (sem_wait(canWrite) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
            
            if (sem_wait(index_access) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
            int head = infoPartilhada->head;

            // Alterar os números escritos

            infoPartilhada->buffer[head] = 1;

            if(head == BUF_SIZE - 1) {
                infoPartilhada->head = 0;
            } else {
                infoPartilhada->head = head + 1;
            }

            if (sem_post(index_access) == -1) {
                perror("Error at sem_post().");
                exit(3);
            }
           
            if (sem_post(canRead) == -1) {
                perror("Error at sem_post().");
                exit(3);
            }
        }

            exit(0);
        }

    }

    // Consumidor
    for(int i = 0; i < 60; i++) {

            if (sem_wait(canRead) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
            
            if (sem_wait(index_access) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
            
            int tail =  infoPartilhada->tail;

            // int readData =  infoPartilhada->buffer[tail];

            if(tail == BUF_SIZE - 1) {
                infoPartilhada->head = 0;
            } else {
                 infoPartilhada->head = tail + 1;
            }

            // imprimir a informação

            if (sem_post(index_access) == -1) {
                perror("Error at sem_post().");
                exit(3);
            }
           
            if (sem_post(canWrite) == -1) {
                perror("Error at sem_post().");
                exit(3);
            }

        }



    for (int i = 0; i < PRODUTORES; i++) {
        wait(NULL);
    }

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/ex13") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }

    if (sem_unlink("/canwrite") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/canread") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/access") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    return 0;
}