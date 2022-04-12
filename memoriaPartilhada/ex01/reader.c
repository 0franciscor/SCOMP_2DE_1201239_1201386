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
		perror("Erro Memória Partilhada");
		return 1;
	}	
	
	ftruncate(fd,size);
	infoPartilhada = (aluno*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	printf("Nome: %s", infoPartilhada->nome);
	printf("Número: %d\n", infoPartilhada->numero);
	
	shm_unlink("/ex1");
	return 0;
    
}