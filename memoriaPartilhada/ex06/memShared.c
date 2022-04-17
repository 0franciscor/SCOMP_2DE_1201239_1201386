#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<sys/wait.h>

#define ARRAY_SIZE 1000000

int main(){
    
    time_t start = time(NULL);
    
    int fd, size = sizeof(int) * ARRAY_SIZE;
    int *arrayAddr; 
    
    fd = shm_open("/ex06", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0) {
        perror("Error when creating shared memory file.\n");
    }

    if (ftruncate (fd, size) < 0) {
        perror("Error when executing ftruncate() function.\n");
    }
    
    arrayAddr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    for(int i = 0; i < ARRAY_SIZE; i++){
        *(arrayAddr + i) = i;
    }

    pid_t p = fork();

    if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

    if(p == 0){

        int fd2 = shm_open("ex06", O_RDWR, S_IRUSR|S_IWUSR);
        
        if(fd < 0) {
            perror("Error when creating shared memory file.\n");
        }

        if (ftruncate (fd, size) < 0) {
            perror("Error when executing ftruncate() function.\n");
        }

        int *arrayAddr2;
        arrayAddr2 = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0); 
        
        for(int i = 0; i < ARRAY_SIZE; i++) {
            printf("Numero: %d\n", *(arrayAddr2 + i));
        }

        exit(0);
    }

    wait(NULL);    

    time_t end = time(NULL);
    int total = end - start;
    printf("Tempo de execução: %ds\n", total);
    

    return 0;
}