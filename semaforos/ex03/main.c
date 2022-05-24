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

#define MAX_STRINGS 50
#define STRING_SIZE 80
#define QUANT_PROCESSOS 4

typedef struct{
    int quant_info;
    char data[MAX_STRINGS][STRING_SIZE];
}info;

int main(int argc, char *argv[]) {
	int size = sizeof(info); 
    info *infoPartilhada;

    int fd = shm_open("/EXERCICIO03",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/EXERCICIO03");
		exit(1);
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		exit(2);
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    infoPartilhada->quant_info = 0;


	sem_t *semWrite, *capacidade;

    if ((semWrite = sem_open("/cWrite3", O_CREAT, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }

    if ((capacidade = sem_open("/capacidade", O_CREAT, 0644, 50)) == SEM_FAILED ){
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
			
			if (sem_wait(semWrite) == -1) {
                perror("Error at sem_wait().");
                exit(3);
            }
			
			int count = 0;
			
			while(sem_trywait(capacidade) == 0) {
				
				if(count != 0) {
					if (sem_wait(semWrite) == -1) {
                		perror("Error at sem_wait().");
                		exit(3);
            		}
				}
			
				sprintf(infoPartilhada->data[infoPartilhada ->quant_info], "I’m the Father - with PID %ld\n",(long)getpid());
				infoPartilhada ->quant_info++;
		

				if (sem_post(semWrite) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                }
				
				int n = (rand() % 5) + 1;
			
				sleep(n);
				
				count++;
			} 

            if (sem_post(semWrite) == -1) {
                perror("Error at sem_post().");
                exit(3);
            }
			
			exit(0);
			
		}
	}
	
    for (int i = 0; i < QUANT_PROCESSOS; i++) {
		wait(NULL);
	}
	
	for (int i = 0; i < infoPartilhada ->quant_info; i++) {
        printf("%d: %s\n", i, infoPartilhada ->data[i]);
    }
    printf("Number of Lines: %d\n", infoPartilhada ->quant_info);
    
    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/EXERCICIO03") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }

    if (sem_unlink("/cWrite3") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    if (sem_unlink("/capacidade") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

	return 0;
}
