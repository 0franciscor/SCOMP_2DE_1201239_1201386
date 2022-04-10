#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>

#define MAQUINAS 4
#define PIECES 1000
#define M1_TIME 5
#define M2_TIME 5
#define M3_TIME 10
#define M4_TIME 100

int main(){
    pid_t pids[MAQUINAS];
	int fd[MAQUINAS][2]; 
	int totalPecas = 0;
    int pecas = 0;
    int pecasRecebidas = 0;
	
	for(int i = 0; i < MAQUINAS; i++) {
		if (pipe(fd[i]) == -1){
            perror("Pipe failed!");
            exit(-1);
        }
	}

    for (int i=0;i < MAQUINAS; i++){
        pids[i] = fork();
        if(pids[i] == -1){
            perror("Fork Falhou");
            return 2;
        }
        if (pids[i]==0){
            close(fd[i][0]);
            if (i>0){
                close(fd[i-1][1]);
            }
            while(totalPecas < PIECES) {
                if (i==0){
                    if (M1_TIME == pecas){
						totalPecas += pecas;
						write(fd[i][1], &pecas, sizeof(pecas));
                        printf("M1 enviou %d peças para M2, tendo já enviado um total de %d peças. \n", pecas,totalPecas);
						pecas = 0;
                    }
                    pecas++;
                } else {
                    read(fd[i-1][0], &pecasRecebidas, sizeof(pecas));
                    pecas += pecasRecebidas;   
                    if (i==1){
                        if (M2_TIME == pecas){
							totalPecas += pecas;
							write(fd[i][1], &pecas, sizeof(pecas));
							printf("M2 enviou %d peças para M3, tendo já enviado um total de %d peças.\n", pecas,totalPecas);
							pecas = 0;  
                        }
                    }
                    if (i==2){
                        if (M3_TIME == pecas){ 
							totalPecas += pecas;
							write(fd[i][1], &pecas, sizeof(pecas));
							printf("M3 enviou %d peças para M4, tendo já enviado um total de %d peças.\n", pecas,totalPecas);	
							pecas = 0;   
                        }
                    }
                    if (i==3){
                        if (M4_TIME == pecas){
							totalPecas += pecas;
							write(fd[i][1], &pecas, sizeof(pecas));
							printf("M4 enviou %d peças para A1, tendo já enviado um total de %d peças.\n", pecas,totalPecas);
							pecas = 0; 
                        }
                    }
                }
            }
            close(fd[i][1]);
            exit(0);
        }
    }
    close(fd[3][1]);
    
    while(pecas < PIECES){
        read(fd[3][0], &totalPecas, sizeof(totalPecas));
        pecas += totalPecas;
    }
    close(fd[3][0]);
    printf("Operação concluída com sucesso!\n");
    
    return 0;
}