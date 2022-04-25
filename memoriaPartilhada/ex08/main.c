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

typedef struct{
    int num;
} sharedStruct;

int main(){

    sharedStruct *numAddr;
    
    int fd = shm_open("/ex08", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0) perror("Error when creating shared memory file.\n");
    if (ftruncate (fd, sizeof(int)) < 0) perror("Error when executing ftruncate() function.\n");
    numAddr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    numAddr->num = 10;

    for(int i = 0; i < NUM_ITERACTIONS; i++){
        (numAddr->num)++;
        (numAddr->num)--;
    }

    pid_t p = fork();

    if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

    if(p == 0){
        
        for(int i = 0; i < NUM_ITERACTIONS; i++){
            (numAddr->num)++;
            (numAddr->num)--;
        }

        exit(0);
    }

    wait(NULL);

    printf("Value: %d\n", numAddr->num);

    fd = munmap(numAddr, sizeof(sharedStruct));

	fd = shm_unlink("/ex08");

	close(fd);

    return 0;
}