#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define ARRAY_SIZE 10

typedef struct{
	int numeros[ARRAY_SIZE];
} info;

int main(int argc, char* argv[]) {

	int size = sizeof(info); 
	info* infoPartilhada;

	int fd = shm_open("/ex7",O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	
	if(fd == -1){
		perror("Erro Memória Partilhada");
		return 1;
	}	
	
	ftruncate(fd,size);
	infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	

    for (int i = 0; i < ARRAY_SIZE; i++){
        printf("Posição %d: %d\n", i, infoPartilhada->numeros[i]);
    }
	

	shm_unlink("/ex7");
    
	return 0;
    
}