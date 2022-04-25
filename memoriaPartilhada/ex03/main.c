#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define WORD_SIZE 20
#define ARRAY_SIZE 100000

typedef struct{
    int numero;
    char frase[WORD_SIZE];
}info;

int main(int argc, char* argv[]) {

    time_t start = time(NULL);

    int size = sizeof(info); 
	info *infoPartilhada;

    info arr [ARRAY_SIZE];

    for(int i = 0; i < ARRAY_SIZE; i++){
        arr[i].numero = i;
		strcpy(arr[i].frase,"ISEP - SCOMP 2020");
    }

	int fd = shm_open("/ex3",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
		perror("Creating or opening shared memory failure");
		shm_unlink("/ex3");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    pid_t pid = fork();

    if(pid == -1){
        perror("Fork Falhou");
        return 3;

    } else if (pid == 0){

        for (int i = 0; i < ARRAY_SIZE ; i++) {
            (infoPartilhada+i)->numero = arr[i].numero;
            strcpy((infoPartilhada+i)->frase,arr[i].frase);
        }

        if (munmap(infoPartilhada, size) < 0) {
            printf("Error at munmap()!\n");
            return 4;
        }

        if (close(fd) < 0) {
            printf("Error at close()!\n");
            return 5;
        }

    
        if (shm_unlink("/ex3") < 0) {
            printf("Error at shm_unlink()!\n");
            return 6;
        }

        exit(1);
    }

    wait(NULL);    

    time_t end = time(NULL);
    int total = end - start;
    printf("Tempo de execução memória partilhada: %ds\n", total);

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    }
    
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    time_t inicio = time(NULL);

    int pp[2];

    if(pipe(pp) == -1){
        perror("There was an error when creating the Pipe.\n");
        return -1;
    }

    pid_t p = fork();

    if(p == -1){
        perror("Fork Falhou");
        return 3;

    } else if (p > 0){

        close(pp[0]);

        for(int i=0; i<ARRAY_SIZE; i++){
			info temp = arr[i];
			write(pp[1], &temp, sizeof(temp));
		}

        close(pp[1]);

        wait(NULL);

        time_t stop = time(NULL);
        int dif = inicio - stop;
        printf("Tempo de execução pipes: %ds\n", dif);
        
    } else {
        close(pp[1]);
		for(int i=0; i<ARRAY_SIZE; i++){
			info aux;
			read(pp[0], &aux, sizeof(aux));
		}
			
		close(pp[0]);

        exit(1);
    }

    return 0;
}
