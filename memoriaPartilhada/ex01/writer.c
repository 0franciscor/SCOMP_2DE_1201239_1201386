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

	int fd = shm_open("/ex1",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    // Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL) with name “/shmtest”, 
    // and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
	
	if(fd == -1){
		perror("Memória Partilhada já existe");
		shm_unlink("/ex1");
		return 1;
	}	
	
	if(ftruncate(fd,size) == -1) {
		perror("Truncar Memória Partilhada Falhou");
		return 2;
	}
	
	infoPartilhada = (aluno*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    // Maps the area in na address decided by the OS (NULL), requring to use all the size, with
    // read and write permissions (PROT_READ | PROT_WRITE) , consistente with open Area is shared (MAP_SHARED), and
    // it refers to the previous open (fd). All memory area will be used so starting from beginning (offset is 0).
	
	char str[MAX_NOME];
    int num;
	printf("Nome:");
	fgets(str, sizeof(char) * MAX_NOME, stdin);
	strcpy((infoPartilhada->nome), str); // escrever na memória partilhada
	printf("Número:");
	scanf("%d", &infoPartilhada->numero); // escrever na memória partilhada

    return 0;

}