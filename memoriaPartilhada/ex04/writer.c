#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(){

    int fd;
    char *arrayAddr; 
    
    fd = shm_open("/ex04", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate (fd, 100);
    arrayAddr = mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    srand(time(0));

    for(int i = 0; i < 100; i++){
        *(arrayAddr + i) = rand() % (90 - 65 + 1) + 65;
    }

    return 0;
}