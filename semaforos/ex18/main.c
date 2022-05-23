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

int main(){

    // ########################### SHARED MEMORY ################################

    int fd, size = size = sizeof(char*) * 100;

    char *string;
    
    fd = shm_open("ex14", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    string = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // ###########################################################################

    sem_t *sem;

    if ((sem = sem_open("14r", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    printf("Reader With ID: %d\n", getpid());
    
    sleep(1);

    sem_wait(sem);
    if(*string == '\0') printf("There is no String available.\n");
    else printf("The content of the String is: %s\n", string);
    sem_post(sem);
    
    return 0;
}