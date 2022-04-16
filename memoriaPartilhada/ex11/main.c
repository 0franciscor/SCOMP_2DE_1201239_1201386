#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define ARRAY_SIZE 1000
#define QUANT_FILHOS 10

struct info{
    int i;
    int numero;
};


int main(int argc, char* argv[]) {

    int arr[ARRAY_SIZE];

    srand(time(NULL));

    for(int i = 0; i < ARRAY_SIZE; i++){
        arr[i] = rand()% 256;
    }

    int fd[2];

    if(pipe(fd) == -1){
        perror("Pipe Falhou");
        return 1;
    }

    for(int i = 0; i < QUANT_FILHOS; i++) {
			
		pid_t pid = fork();

		if(pid == -1){
            perror("Fork Falhou");
            return 2;
        } else if(pid == 0) {
            close(fd[0]);

            int start = i * 100;
            int final = start + 100;
            int max = -1;

            struct info enviar;
            enviar.i=i;

            for(int j = start; j < final; j++){
                if(arr[j] > max){
                    max = arr[j];
                }
            } 
            enviar.numero=max;

            printf("Maior valor encontrado pelo filho %d: %d\n", i, enviar.numero);
            write(fd[1], &enviar, sizeof(enviar));
            close(fd[1]);

            exit(1);

        }

    }

    close(fd[1]);


    for(int i = 0; i < QUANT_FILHOS; i++){
		wait(NULL);
	}

    int result = -1;

    for (int i = 0; i < QUANT_FILHOS; i++) {
        struct info resultados;
        read(fd[0], &resultados, sizeof(resultados));
        if (resultados.numero > result){
            result = resultados.numero;
        }
    }

    close(fd[0]);

    printf("O maior valor global é: %d\n", result);


    return 0;

}