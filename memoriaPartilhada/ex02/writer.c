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

int main(){

    int fd;
    sharedStruct *addr;
    
    fd = shm_open("/ex02", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate (fd, sizeof(sharedStruct));
    addr = mmap(NULL, sizeof(sharedStruct), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Code:\n");
    scanf("%d", &addr->code);
    printf("Price:\n");
    scanf("%f", &addr->price);
    printf("Description:\n");
    scanf("%s", addr->description);

    return 0;
}