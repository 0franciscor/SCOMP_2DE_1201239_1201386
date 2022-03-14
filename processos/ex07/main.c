#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000

int main (){

    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n; /* the number to find */
    int status;
    time_t t; /* needed to init. the random number generator (RNG) */
    int i;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand () % 10000;
    }

    /* initialize n */
    n = rand () % 10000;
    
    int metade = ARRAY_SIZE/2;
    int soma = 0;
    pid_t p1 = fork();

    if (p1==0){
        for (i = 0; i < metade; i++){
            if (numbers[i] == n){
                soma ++;
            }
            exit(soma);
        }
    } else if (p1 > 0){
        for (i = metade; i < ARRAY_SIZE; i++){
            if (numbers[i] == n){
                soma ++;
            }
        }
        waitpid(p1, &status, 0);
        if (WIFEXITED(status)) {
           soma += WEXITSTATUS(status);
        }  

        printf("O número %d aparece %d vezes no array\n",n,soma);
    }


} 
