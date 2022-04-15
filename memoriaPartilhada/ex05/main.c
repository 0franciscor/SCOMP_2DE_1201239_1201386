#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define ARRAY_SIZE 1000000

typedef struct{
  int num1;
  int num2;
} info;

int main(int argc, char* argv[]) {

    int size = sizeof(info); 
	info *infoPartilhada;

	int fd = shm_open("/ex5",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/ex5");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}
    
    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
    infoPartilhada->num1 = 8000;
    infoPartilhada->num2 = 200;

    pid_t pid = fork();

    if(pid == -1){
        perror("Fork Falhou");
        return 3;

    } else if (pid > 0){
        for(int i = 0; i < ARRAY_SIZE; i++){
            infoPartilhada->num1 += 1;
            infoPartilhada->num2 -= 1;
        }

    } else {
        for(int i = 0; i < ARRAY_SIZE; i++){
            infoPartilhada->num1 -= 1;
            infoPartilhada->num2 += 1;
        }

        if (munmap(infoPartilhada, size) < 0) {
            printf("Error at munmap()!\n");
            return 4;
        }

        if (close(fd) < 0) {
            printf("Error at close()!\n");
            return 5;
        }

    
        if (shm_unlink("/ex5") < 0) {
            printf("Error at shm_unlink()!\n");
            return 6;
        }

        exit(1);
    }

    
    printf("Número 1: %d\n", infoPartilhada->num1);
    printf("Número 2: %d\n", infoPartilhada->num2);

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    }

    
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }

    return 0;
}