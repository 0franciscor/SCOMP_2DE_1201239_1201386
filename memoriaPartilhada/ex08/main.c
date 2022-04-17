#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<sys/wait.h>

#define NUM_ITERACTIONS 1000000

int main(){

    int *numAddr;
    
    int fd = shm_open("/ex08", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0) perror("Error when creating shared memory file.\n");
    if (ftruncate (fd, sizeof(int)) < 0) perror("Error when executing ftruncate() function.\n");
    numAddr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    *numAddr = 10;

    for(int i = 0; i < NUM_ITERACTIONS; i++){
        (*numAddr)++;
        (*numAddr)--;
    }

    pid_t p = fork();

    if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

    if(p == 0){
        int *numAddr2;

        int fd2 = shm_open("/ex08", O_RDWR, S_IRUSR|S_IWUSR);
        if(fd2 < 0) perror("Error when creating shared memory file.\n");
        if (ftruncate (fd2, sizeof(int)) < 0) perror("Error when executing ftruncate() function.\n");
        numAddr2 = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);

        for(int i = 0; i < NUM_ITERACTIONS; i++){
            (*numAddr2)++;
            (*numAddr2)--;
        }

        exit(0);
    }

    wait(NULL);

    printf("Value: %d\n", *numAddr);

    return 0;
}