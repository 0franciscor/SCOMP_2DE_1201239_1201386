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

typedef struct{
    char frase[100];
} sharedSentence;

int main(){

    // ########################### SHARED MEMORY ################################

    int fd, size = sizeof(sharedSentence);

    sharedSentence *string;
    
    fd = shm_open("ex14", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    string = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // ###########################################################################

    sem_t *semWriter, *semReader;

    if((semWriter = sem_open("14w", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    if((semReader = sem_open("14r", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_open function\n");
        exit(1);
    }

    sem_wait(semWriter);
    sem_wait(semReader);

    sleep(5);

    sharedSentence writtenString;

    sprintf(writtenString.frase, "The Writer with %d PID wrote this message.", getpid());

    *string = writtenString;

    sem_post(semWriter);
    sem_post(semReader);

    return 0;
}