#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <string.h>
#include <time.h>

#define VETOR_SIZE 10000

typedef struct{
    int vetor[VETOR_SIZE];
    int vetorFinal[(VETOR_SIZE/2)];
} estrutura;

int main(){

    // ########################### SHARED MEMORY ################################

    int fd, size = sizeof(estrutura);

    estrutura *sharedStruct;
    
    fd = shm_open("ex18", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    sharedStruct = (estrutura*) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // ###########################################################################

    sem_t *semP1, *semP2;

    if ((semP1 = sem_open("p1", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semP2 = sem_open("p2", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    srand(time(0));

    estrutura initialStruct;

    for(int i = 0; i < VETOR_SIZE; i++){
        initialStruct.vetor[i] = (rand() % (5000 - 1 + 1)) + 1;
    }

    *sharedStruct = initialStruct;

    pid_t p1 = fork();
    if(p1 == 0){
        
        int i = 0, soma;
        for(; i < VETOR_SIZE; i+=3){
            sem_post(semP2);

            soma = sharedStruct->vetor[i] + sharedStruct->vetor[i + 1];
            sharedStruct->vetorFinal[i] = soma;

            sem_wait(semP1);
        }
        exit(0);
    }

    pid_t p2 = fork();
    if(p2 == 0){
        
        int i = 1, soma;
        for(; i < VETOR_SIZE; i+=3){
            sem_post(semP1);

            soma = sharedStruct->vetor[i] + sharedStruct->vetor[i + 1];
            sharedStruct->vetorFinal[i] = soma;

            sem_wait(semP2);
        }
        exit(0);
    }

    
    pid_t p3 = fork();
    if(p3 == 0){
        int index = 0, greatest = 0;
        while(index < (VETOR_SIZE/2)){
            sem_trywait(semP1);
            sem_trywait(semP2);
            fflush(stdout);
            if(sharedStruct->vetorFinal[index] > greatest){
                greatest = sharedStruct->vetorFinal[index];
                printf("There is a new greatest number: %d\n", greatest);
                fflush(stdout);
            }  

            sem_post(semP1);
            sem_post(semP2);

            index++;
        }
        exit(0);
    }

    for(int i = 0; i < 3; i++)
        wait(NULL);

    if (munmap(sharedStruct, size) < 0) {
        printf("Error at munmap()!\n");
        exit(1);
    }

    if (shm_unlink("ex18") < 0){
        perror("Error in sem_unlink()");
        exit(1);
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(1);
    }
    
    // CLOSING SEMAPHORES

    if(sem_close(semP1) == -1){
        printf("There was an error when using the sem_close() function.\n");
        exit(1);
    }

    if(sem_close(semP2) == -1){
        printf("There was an error when using the sem_close() function.\n");
        exit(1);
    }

    if (sem_unlink("p1") == -1){
        perror("Error in sem_unlink()");
        exit(1);
    }

    if (sem_unlink("p2") == -1){
        perror("Error in sem_unlink()");
        exit(1);
    }
    

    return 0;
}