#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define MAX_NOME 100

typedef struct{
	char nome[MAX_NOME];
	int numero;	
} aluno;

int main(int argc, char* argv[]) {

	int size = sizeof(aluno); 
	aluno* infoPartilhada;

	int fd = shm_open("/ex1",O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	
	if(fd == -1){
		perror("Creating or opening shared memory failure");
		return 1;
	}	

	if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

	infoPartilhada = (aluno*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	printf("Nome: %s", infoPartilhada->nome);
	printf("Número: %d\n", infoPartilhada->numero);

	
    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    }

   
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }

    
    if (shm_unlink("/ex1") < 0) {
        printf("Error at shm_unlink()!\n");
        return 5;
    }
	
	return 0;
    
}