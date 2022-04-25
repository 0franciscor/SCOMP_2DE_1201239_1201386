#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

typedef struct {
	int array[10];
	int index;
} ringStruct;

int main() {

	int fd[2], fd2[2];

	if(pipe(fd) == -1 || pipe(fd2) == -1){
		printf("Error when creating Pipe\n");
		return -1;
	}

	pid_t p = fork();

	if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

	if (p == 0){
		close(fd[1]); close(fd2[0]);
		
		ringStruct estruturaF;
		read(fd[0], &estruturaF, sizeof(estruturaF));
		
		while(estruturaF.index <= 3){

			printf("\nProcesso Consumidor:\n");
			int lastIndex = estruturaF.array[0];
			for(int i = 0; i < 10; i++){
				if(i != 0 && lastIndex >= estruturaF.array[i]){
					printf("Os números não são sucessivamente superiores. O resto da sequência não será apresentada.\n");
					break;
				}
				printf("Numero Escrito: %d\n", estruturaF.array[i]);
				lastIndex = estruturaF.array[i];
			}

			if(estruturaF.index >= 3)
				break;

			int wait;
			write(fd2[1], &wait, sizeof(int));

			read(fd[0], &estruturaF, sizeof(estruturaF));
		}
		
		close(fd[0]); close(fd2[1]);
		exit(1);
	}

	close(fd[0]); close(fd2[1]);

	ringStruct estrutura;    
	estrutura.index = 0;
	
	while(estrutura.index < 3){

		printf("\nRonda: %d ====== Processo Produtor:\n", estrutura.index + 1);
		for(int j = 0; j < 10; j++){
			printf("Insira o valor de índice %d: ", j+1);
			scanf("%d", &estrutura.array[j]);
		}
		estrutura.index++;
		
		int wait;

		write(fd[1], &estrutura, sizeof(estrutura));
		read(fd2[0], &wait, sizeof(int));
	}

	close(fd[1]); close(fd2[0]);

	return 0;
}
