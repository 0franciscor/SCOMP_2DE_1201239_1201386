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

#define EXIT_FAILURE 1

#define QUANT_PROCESSOS 500
#define CAPACIDADE 300
#define TEMPO 1

typedef struct{
	int quant;
    int vipsWaiting;
    int specialsWaiting;
}info;

void up(sem_t *sem) {
	if(sem_post(sem) == -1) {
		perror("Error at sem_post()!");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[]) {
	/************ Creating Needed Semaphores ************/
	sem_t *exclusion, *capacidade, *semSpecial, *semVip;

	// mutual exclusion

    if ((exclusion = sem_open("/exclusion", O_CREAT, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

	// capacidade máxima

    if ((capacidade = sem_open("/capacidade", O_CREAT, 0644, CAPACIDADE)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

	// sincronização

    if ((semSpecial= sem_open("/semSpecial", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((semVip = sem_open("/semVip", O_CREAT, 0644, 0)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }
	/************ End of Creating Semaphores ************/
	
	/************ Creating Shared Memory Area ************/

	int size = sizeof(info); 
    info *infoPartilhada;

    int fd = shm_open("/exerc",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/exerc");
		exit(1);
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		exit(2);
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

	/************ End of Creating Shared Memory Area ************/
	
	//Initializing Cinema
	infoPartilhada->quant=0;
	infoPartilhada->vipsWaiting=0;
	infoPartilhada->specialsWaiting=0;

	/************ CLIENTS ************/
	for (int i = 0; i < QUANT_PROCESSOS; i++)
	{
		pid_t pid = fork();

		if (pid == -1) {
            perror("Error at fork()!");
            exit(EXIT_FAILURE);
        }

        if(pid == 0) {

            srand(clock());
		    int num = rand() % 3;

            if (num == 2){
                printf("Process %d: Normal -> Waiting to enter...\n", i);
			    
                if(sem_wait(exclusion) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
			    int specialsWaiting = infoPartilhada->specialsWaiting;
                int vipsWaiting = infoPartilhada->vipsWaiting;
                if(sem_post(exclusion) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }
			
			    if (specialsWaiting > 0) {
                    if(sem_wait(semSpecial) == -1) {
		                perror("Error at sem_wait()!");
		                exit(EXIT_FAILURE);
	                }
                    
                }
                if (vipsWaiting > 0) {

                    if(sem_wait(semVip) == -1) {
		                perror("Error at sem_wait()!");
		                exit(EXIT_FAILURE);
	                }
                }
            
                if(sem_wait(capacidade) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
                infoPartilhada->quant ++;

                printf("Process %d: Normal -> Entered! --- Lotação %d\n", i,infoPartilhada->quant);
                sleep(TEMPO);
                if(sem_post(capacidade) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }
                infoPartilhada->quant --;
                printf("Process %d: Normal left! -> Lotação: %d\n", i,infoPartilhada->quant);
            
                exit(0);
            } else if (num == 1) {
                printf("Process %d: Special -> Waiting to enter...\n", i);
                if(sem_wait(exclusion) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
				infoPartilhada->specialsWaiting++;
				int vipsWaiting = infoPartilhada->vipsWaiting;
				if(sem_post(exclusion) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }
				
				if (vipsWaiting > 0) {
                    if(sem_wait(semVip) == -1) {
		                perror("Error at sem_wait()!");
		                exit(EXIT_FAILURE);
	                }
				}
				
                if(sem_wait(capacidade) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
				
                if(sem_wait(exclusion) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
				infoPartilhada->specialsWaiting--;
				int specialsWaiting = infoPartilhada->specialsWaiting;
				if(sem_post(exclusion) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }
				
				if (specialsWaiting == 0) {
					if(sem_post(semSpecial) == -1) {
		                perror("Error at sem_post()!");
		                exit(EXIT_FAILURE);
	                }
				}
				
				infoPartilhada->quant ++;

            	printf("Process %d: Special -> Entered! --- Lotação %d\n", i,infoPartilhada->quant);
			
				sleep(TEMPO);
				if(sem_post(capacidade) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }
                infoPartilhada->quant --;
				printf("Process %d: Special left! -> Lotação: %d\n", i,infoPartilhada->quant);
				exit(0);
            } else if (num == 0){
                printf("Process %d: VIP -> Waiting to enter...\n", i);
				infoPartilhada->specialsWaiting++;
			
                if(sem_wait(exclusion) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
				infoPartilhada->vipsWaiting++;
				if(sem_post(exclusion) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }

                if(sem_wait(capacidade) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }

                if(sem_wait(exclusion) == -1) {
		            perror("Error at sem_wait()!");
		            exit(EXIT_FAILURE);
	            }
				infoPartilhada->vipsWaiting--;
				int vipsWaiting = infoPartilhada->vipsWaiting;
				if(sem_post(exclusion) == -1) {
		            perror("Error at sem_post()!");
		            exit(EXIT_FAILURE);
	            }

				if (vipsWaiting == 0) {
					if(sem_post(semVip) == -1) {
		                perror("Error at sem_post()!");
		                exit(EXIT_FAILURE);
	                }
				}

				infoPartilhada->quant ++;

            	printf("Process %d: VIP -> Entered! --- Lotação %d\n", i,infoPartilhada->quant);
				
				sleep(TEMPO);
				up(capacidade);
                infoPartilhada->quant --;
				printf("Process %d: VIP left! -> Lotação: %d\n", i,infoPartilhada->quant);
				exit(0);
            }
            exit(0);
		}
	}
	
	for (int i = 0; i < QUANT_PROCESSOS; i++){
		wait(NULL);
	}
	
	if (sem_unlink("/exclusion") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/capacidade") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

	if (sem_unlink("/semSpecial") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/semVip") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }
	
	if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/exerc") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }
	
	
	
	
	return 0;
}