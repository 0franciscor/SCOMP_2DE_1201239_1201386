#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define ARRAY_MAX_SIZE 1000

int main(){

    // Criacao dos Arrays

    int array1[ARRAY_MAX_SIZE];
    int array2[ARRAY_MAX_SIZE];

    for(int i = 0; i < ARRAY_MAX_SIZE; i++){
        array1[i] = 1;
        array2[i] = 1;
    }

    // Criacao de procs filhos para calcular as somas de diferentes posicoes

    pid_t p;
    int soma = 0;
    for(int i = 1; i <= 5; i++){
        
        // Criacao do Pipe para inserir os dois arrays
        int fd[2];
        if(pipe(fd) == -1){
            printf("Error when creating Pipe");
            return -1;
        }

        close(fd[0]);
        write(fd[1], &array1, sizeof(array1));
        write(fd[1], &array2, sizeof(array2));
        close(fd[1]);
        
        int fd2[2];
        
        p = fork();
        if(p == 0){
            close(fd[1]);

            int arrayReceived1[ARRAY_MAX_SIZE];
            int arrayReceived2[ARRAY_MAX_SIZE];
            read(fd[0], &arrayReceived1, sizeof(arrayReceived1));
            read(fd[0], &arrayReceived2, sizeof(arrayReceived2));
            close(fd[0]);

            int temp = 0;
            for(int j = i; j < (i*200); j++){
                temp += arrayReceived1[(j-1)] + arrayReceived2[(j-1)];
            }

            if(pipe(fd2) == -1){
                printf("Error when creating Pipe");
                return -1;
            }
            
            close(fd2[0]);
            write(fd2[1], &temp, sizeof(int));
            close(fd2[1]);
            close(fd2[1]);
            
            exit(0);
        }    

        wait(NULL);
        int readValue = 0;
        close(fd2[1]);
        read(fd2[0], &readValue, sizeof(int));
        close(fd2[0]);
        soma += readValue;
    }    

    printf("A soma dos valores é: %d\n", soma);

    return 0;
}