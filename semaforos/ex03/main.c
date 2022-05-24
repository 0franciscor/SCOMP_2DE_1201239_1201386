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

#define MAX_STRINGS 50
#define STRING_SIZE 80
#define QUANT_PROCESSOS 5

typedef struct{
    int quant_info;
    char data[MAX_STRINGS][STRING_SIZE];
}info;

int main(){

    int size = sizeof(info); 
    info *infoPartilhada;

    int fd = shm_open("/EXER03",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/EXER03");
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

    for(int i = 0; i < QUANT_PROCESSOS; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){

            int temp = 1;

            while (temp == 1){

                if (sem_wait(semWrite) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                if(infoPartilhada->quant_info == 50){
                    temp = 0;
                } else {
                    char aux[80];
	                sprintf(aux,"I'm the Father - with PID %ld\n",(long)getpid());
                    strcpy(infoPartilhada->data[infoPartilhada->quant_info],aux);
                    infoPartilhada->quant_info++;
                }    

                if (sem_post(semWrite) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                int n = (rand() % 5) + 1;
                sleep(n);

            }

            exit(0);
        
        }

    }

    for(int i = 0; i < QUANT_PROCESSOS; i++){
        wait(NULL);
    }

    for(int i = 0; i < MAX_STRINGS; i++){
        printf("%d: %s\n",i,infoPartilhada->data[i]);
    }

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/EXER03") < 0) {
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