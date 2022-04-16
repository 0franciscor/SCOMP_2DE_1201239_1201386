#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
#define QUANT_FILHOS 10

typedef struct{
  int numeros[QUANT_FILHOS];
} info;

int main(int argc, char* argv[]) {

  int size = sizeof(info); 
	info *infoPartilhada;

  int arr[ARRAY_SIZE];

  srand(time(NULL));

  for(int i = 0; i < ARRAY_SIZE; i++){
    arr[i] = rand()% 256;
  }

  int fd = shm_open("/ex9",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

  if(fd == -1){
	  perror("Creating or opening shared memory failure");
		shm_unlink("/ex9");
		return 1;
	}

  if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

  infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
  pid_t pid;

  for(int i = 0; i < QUANT_FILHOS; i++){
    pid = fork();

    if(pid == -1){
      perror("Fork Falhou");
      return -1;
    } else if (pid == 0){

      int start = i * 100;
      int final = start + 100;
      int max = -1;

      for(int j = start; j < final; j++){
        if(arr[j] > max){
          max = arr[j];
        }
      }

      infoPartilhada->numeros[i] = max;
      printf("Maior valor encontrado pelo filho %d: %d\n", i, infoPartilhada->numeros[i]);


      if (munmap(infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 4;
      }

      if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 5;
      }

      exit(1);
    }
    
  }

  for(int i = 0; i < QUANT_FILHOS; i++){
		wait(NULL);
	}

  int result = -1;

  for(int i = 0; i < QUANT_FILHOS; i++){
    if(infoPartilhada->numeros[i] > result){
      result = infoPartilhada->numeros[i];
    }
  }

  printf("O maior valor global é: %d\n", result);

  if (munmap((void *)infoPartilhada, size) < 0) {
    printf("Error at munmap()!\n");
    return 3;
  }

  if (close(fd) < 0) {
    printf("Error at close()!\n");
    return 4;
  }

  return 0;

}