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

    sem_t *semP1, *semP2, *semP3;

    if ((semP1 = sem_open("p1", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semP2 = sem_open("p2", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if ((semP3 = sem_open("p3", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    srand(time(0));

    for(int i = 0; i < VETOR_SIZE; i++){
        sharedStruct->vetor[i] = (rand() % (5000 - 1 + 1)) + 1;
    }

    for(int i = 0; i < (VETOR_SIZE/2); i++){
        sharedStruct->vetorFinal[i] = 0;
    }

    pid_t p1 = fork();
    if(p1 == 0){
    
        int index = 0;
        for(int i = 0; i < VETOR_SIZE; i+=4){
            sem_post(semP2);
            sem_trywait(semP3);

            int multiplicacao = sharedStruct->vetor[i] * sharedStruct->vetor[i+1];
            sharedStruct->vetorFinal[index] = multiplicacao;

            index += 2;
            sem_wait(semP1);
            sem_post(semP3);
        }     
        
        exit(0);
    }

    pid_t p2 = fork();
    if(p2 == 0){

        int index = 1;
        for(int i = 2; i < VETOR_SIZE; i+=4){
            sem_post(semP1);
            sem_trywait(semP3);

            int multiplicacao = sharedStruct->vetor[i] * sharedStruct->vetor[i+1];
            sharedStruct->vetorFinal[index] = multiplicacao;
            
            index += 2;
            sem_wait(semP2);
            sem_post(semP3);
        } 

        exit(0);
    }

    
    pid_t p3 = fork();
    if(p3 == 0){
        int greatest = 0;

        for(int i = 0; i < (VETOR_SIZE/2); i++){
            sem_wait(semP3);
            
            int numero = sharedStruct->vetorFinal[i]; //Com print, a funcao funciona
            if(numero > greatest){
                greatest = numero;
                printf("There is a new greatest number: %d\n", greatest);
            }

            sem_post(semP3);
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

    if(sem_close(semP3) == -1){
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

    if (sem_unlink("p3") == -1){
        perror("Error in sem_unlink()");
        exit(1);
    }
    
    return 0;
}