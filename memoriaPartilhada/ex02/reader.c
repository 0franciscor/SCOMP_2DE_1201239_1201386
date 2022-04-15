#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    float price;
    int code;
    char description[20];
} sharedStruct;

int main() {
    int fd;
    sharedStruct *addr;

    fd = shm_open("ex02", O_RDWR, S_IRUSR|S_IWUSR);

    if(fd == -1){
		perror("Shared Memory File already exists.");
		shm_unlink("/ex02");
		return -1;
	}

    ftruncate(fd, sizeof(sharedStruct));
    addr = mmap(NULL, sizeof(sharedStruct),PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Code: %d\n", addr->code);
    printf("Description: %s\n", addr->description);
    printf("Price: %.2f€\n", addr->price);

    return 0;
}