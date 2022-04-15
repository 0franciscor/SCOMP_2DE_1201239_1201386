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
	info *infoPartilhada;

	int fd = shm_open("/ex7",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    // Creates (O_CREAT) shared memory area (with error if existis because of O_EXCL) with name “/shmtest”, 
    // and read/write permissons (O_RDWR), and open for user to read (S_IRUSR) and write (S_IWUSR).
	
	if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/ex7");
		return 1;
	}	

	if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	

    // Maps the area in na address decided by the OS (NULL), requring to use all the size, with
    // read and write permissions (PROT_READ | PROT_WRITE) , consistente with open Area is shared (MAP_SHARED), and
    // it refers to the previous open (fd). All memory area will be used so starting from beginning (offset is 0).

  	srand(time(NULL));

  	for(int i = 0; i < ARRAY_SIZE; i++){
    	infoPartilhada->numeros[i] = rand()%((20+1)-1) + 1;
  	}

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