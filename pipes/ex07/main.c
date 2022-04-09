#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define SYZE 1000
#define QUANT_FILHOS 5

struct pipeStruct{
    int child;
    int soma[200];
};

int main(int argc, char* argv[]) {
	
	int fd[QUANT_FILHOS][2];
	
	int vec1[SYZE];
	int	vec2[SYZE];

	time_t t; 
    srand((unsigned)time(&t));

    for (int i = 0; i < SYZE; i++){
        vec1[i] = rand() % (11);
        vec2[i] = rand() % (11);
    }

	pid_t pid;

	for(int i = 0; i < 5; i++) {
		if(pipe(fd[i]) == -1){
            perror("Pipe Falhou");
            return 1;
        }
			
		pid = fork();

		if(pid == -1){
            perror("Fork Falhou");
            return 2;
        }
			
		else if(pid == 0) {
			
			close(fd[i][0]);
            struct pipeStruct somas;
            somas.child = i;
            int aux = 0;

			for(int j = i * 200; j < (i * 200) + 200; j++) {
				somas.soma[aux] += vec1[j] + vec2[j];
                aux++;
			}

			write(fd[i][1], &somas, sizeof(somas));
			close(fd[i][1]);
			exit(0);
		}
	}
	
	int resultado[SYZE];
	for(int i = 0; i < QUANT_FILHOS; i++) {
        close(fd[i][1]);
        wait(NULL);
	}

    for (int i = 0; i < QUANT_FILHOS; i++) {
        struct pipeStruct resultados;
        read(fd[i][0], &resultados, sizeof(resultados));
        int inicio = resultados.child  * 200;
        int fim = inicio + 200;
        int aux = 0;
        for (int i = inicio; i < fim; i++){
            resultado[i]= resultados.soma[aux];
            aux++;
        }
    }

    for (int i = 0; i < SYZE; i++) {
        printf(" Valor Vec1: %d \n", vec1[i]);
        printf(" Valor Vec2: %d \n", vec2[i]);
        printf(" Soma: %d \n", resultado[i]);
    }
    

	return 0;
}