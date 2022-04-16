#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd;
    char *arrayAddr;

    fd = shm_open("ex04", O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate(fd, sizeof(arrayAddr));
    arrayAddr = mmap(NULL, sizeof(arrayAddr),PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    int soma = 0;
	
	for(int i = 0; i < 100; i++) {
		soma += *(arrayAddr + i);
		printf("Caracter: %d\n", *(arrayAddr + i));
	}

    float media = ((float) soma / 100);
    printf("Soma: %d Média: %.2f\n", soma, media);

    return 0;
}