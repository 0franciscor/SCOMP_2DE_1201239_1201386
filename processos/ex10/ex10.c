#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_FILHOS 10

int criarProcesso(int numProcesso);
 
int main(){
    
    int array[2000];
    for(int i = 0; i < 2000; i++){
        array[i] = rand() % 1000;
    }
    
    int numProcesso = criarProcesso(NUM_FILHOS);
    
    if(numProcesso != -1){

        int numToFind = array[rand() % 1000];

        int bottomScale = 0 + (numProcesso*200);
        int topScale = bottomScale + 199;

        for(int i = bottomScale; i < topScale; i++){
            if(array[i] == numToFind){
                exit(i - bottomScale);
            }
        }
        exit(255);
    } 
    
    for(int i = 0; i < NUM_FILHOS; i++){
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            printf("O filho retornou o seguinte valor: %d\n", WEXITSTATUS(status));
        }
    }
    
    return 0;
}  

int criarProcesso(int numProcesso){
    for(int i = 0; i < numProcesso; i++){ 
        if(fork() == 0){
            return i; 
        }
    }
    return -1;
}
