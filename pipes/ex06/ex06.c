#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#define ARRAY_MAX_SIZE 1000

struct pipeStruct {
    int array1[ARRAY_MAX_SIZE];
    int array2[ARRAY_MAX_SIZE];
};

int main(){

    struct pipeStruct fStruct;
    for(int i = 0; i < ARRAY_MAX_SIZE; i++){
        fStruct.array1[i] = 1;
        fStruct.array2[i] = 1;
    }

    pid_t p;
    int soma = 0;
    for(int i = 1; i <= 5; i++){
        
        int fd[2];
        if(pipe(fd) == -1){
            printf("Error when creating Pipe");
            return -1;
        }

        //close(fd[0]); //Closing the unused part of the pipe lead to a broken Pipe.
        write(fd[1], &fStruct, sizeof(fStruct));
        close(fd[1]);
        
        int fd2[2];
        if(pipe(fd2) == -1){
            printf("Error when creating Pipe");
            return -1;
        }
        
        p = fork();
        if(p == 0){
            struct pipeStruct sonStruct;
            close(fd[1]);
            read(fd[0], &sonStruct, sizeof(sonStruct));
            close(fd[0]);

            int temp = 0;
            for(int j = ((i-1) * 200); j < (i *200); j++){
                temp += sonStruct.array1[i] + sonStruct.array2[i];
            }

            close(fd2[0]);
            write(fd2[1], &temp, sizeof(int));
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