#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define QUANT_FILHOS 5
#define QUANT_PROCESSOS 6

int main(){
	
	int fd[QUANT_PROCESSOS][2];
    int randomNumber;

    for(int i = 0; i < QUANT_PROCESSOS; i++) {	
		if(pipe(fd[i]) == -1) {
			perror("Pipe Falhou");
			return 1;
		}	
	}

    pid_t pid;
	for(int i = 0; i < QUANT_FILHOS; i++) {
		pid = fork();
        srand((unsigned)time(NULL) * getpid());
        randomNumber = rand() % (501);

		if(pid == -1) {
			perror("Fork Falhou");
			return 2;
        }

        if(pid==0){

            close(fd[i][1]);
            int resultado;
            read(fd[i][0], &resultado, sizeof(int));
            
            printf("pid: %d, numero: %d, i= %d\n", getpid(), randomNumber,i);
            
            close(fd[i][0]);
			close(fd[i+1][0]);

            if(randomNumber>resultado){
                write(fd[i+1][1],&randomNumber,sizeof(int));  
            }else{
                write(fd[i+1][1],&resultado,sizeof(int));
            }
            close(fd[i+1][1]);
			exit(0);

        } else {

            if (i == 0){
                close(fd[i][0]);
                printf("pid: %d, numero: %d, processo pai\n", getpid(), randomNumber);
                write(fd[i][1],&randomNumber,sizeof(int));
                close(fd[i][1]);
            }
        }
    }

    close(fd[5][1]);
	read(fd[5][0],&randomNumber,sizeof(int));
	close(fd[5][0]);
	printf("Maior número: %d\n",randomNumber);

    return 0;

}