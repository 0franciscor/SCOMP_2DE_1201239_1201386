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

typedef struct{
  int numeros[];
} info;

int main(int argc, char* argv[]) {

    int size = sizeof(info); 
	info *infoPartilhada;

    srand(time(NULL));

  	for(int i = 0; i < ARRAY_SIZE; i++){
    	infoPartilhada->numeros[i] = rand()% 256;
  	}

    return 0;

}