#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define QUANTIDADE_FILHOS 10

int main() {

    pid_t pid[QUANTIDADE_FILHOS]; 

    for(int i=0; i<QUANTIDADE_FILHOS; i++){
        pid[i] = fork();
        if (pid[i] == 0){
            int numeroInicial = 100*i +1;
            int numeroFinal = 100*(i+1);
            for(int j=numeroInicial; j<=numeroFinal;j++){
                printf("%d\n",j);
            }
            exit(0);
        }
    }

    if (pid[QUANTIDADE_FILHOS-1] > 0){
        for(int aux = 0; aux< QUANTIDADE_FILHOS; aux++){
            wait(NULL);
        }
    }
    
    return 0;
}