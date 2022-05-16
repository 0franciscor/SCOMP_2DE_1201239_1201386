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

    int fd = shm_open("/ex3",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/ex3");
		exit(1);
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		exit(2);
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    sem_t *semWrite;

    if ((semWrite = sem_open("/sem3", O_CREAT, 0644, 1)) == SEM_FAILED ){
        perror("Error in sem_open()");
        exit(3);
    }















    
    if (sem_unlink("/sem3") == -1 ){
        perror("Error in sem_unlink()");
        exit(4);
    }

    return 0;
}