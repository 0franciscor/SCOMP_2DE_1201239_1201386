#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
#define QUANTIDADE_FILHOS 5

int main() {

    int numbers[ARRAY_SIZE]; /* array to lookup */
    pid_t pid[QUANTIDADE_FILHOS]; 
    time_t t; /* needed to init. the random number generator (RNG) */

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (int i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand () % 256;
    }

    int max = 0; 

    for (int i = 0; i< QUANTIDADE_FILHOS; i++){
        pid[i] = fork();
        if (pid[i] == 0){
            int indiceInicial = i * 200;
            int indiceFinal =  indiceInicial + 200; 
            for (int j = indiceInicial; j < indiceFinal; j++){
                if (numbers[j] > max){
                    max = numbers[j];
                }
            }
            exit(max);
        }
    }

    if (pid[QUANTIDADE_FILHOS-1] > 0){
        int maxValue = 0;
        for (int i = 0; i < QUANTIDADE_FILHOS; i++) {
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                if (maxValue < WEXITSTATUS(status)) {
                    maxValue = WEXITSTATUS(status);
                }
            }
        }

        int result[ARRAY_SIZE];
        int metade = ARRAY_SIZE/2;
        pid_t p1 = fork();
        if (p1==0){
            for (int i = 0; i < metade; i++){
                result[i]= ((int) numbers[i]/maxValue)*100;
                printf("%d - %d\n", i, result[i]);
            }
        } else if (p1 > 0){
            for (int i = metade; i < ARRAY_SIZE; i++){
                result[i]= ((int) numbers[i]/maxValue)*100;
            }
            int status;
            wait(&status);
            if (WIFEXITED(status)){
                for (int i =metade; i < ARRAY_SIZE; i++){
                printf("%d - %d\n", i, result[i]);
            }
        }
        }
        
    }


    return 0;
}