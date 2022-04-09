#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define PECAS 100
#define MAQUINAS 4
#define M1_SPEED 5
#define M2_SPEED 5
#define M3_SPEED 10
#define M4_SPEED 100

int main(){

    int fd[MAQUINAS][2];
    int pecasProduzidas = 0;

    for(int i = 0; i < MAQUINAS; i++) {
		if(pipe(fd[i]) == -1) {
			perror("Pipe Falhou");
			return 1;
		}
	}

    pid_t pid;
	for(int i = 0; i < MAQUINAS; i++) {

		if(i == 0 || (i > 0 && pid > 0)) {
			if((pid = fork()) == -1) {
				perror("Fork Falhou");
				return 2;
			} 
            if (pid == 0){

                int maquina = i+1;

                for(i = 0; i < MAQUINAS; i++) {
				    if(i == maquina - 2) {
					    close(fd[i][1]);
				    } else if(i == maquina - 1) {
					    close(fd[i][0]);
				    } else {
					    close(fd[i][0]);
					    close(fd[i][1]);
				    }
			
                    if (i == 0){

                        while(pecasProduzidas < PECAS) {
                            int pecasCortadas = 0;
						    for(i = 0; i < M1_SPEED; i++) {
                                pecasCortadas ++;
						    } 
                            pecasProduzidas += pecasCortadas;
                            write(fd[i][1], &pecasCortadas, sizeof(int));
                            printf("M1 enviou %d peças para M2, tendo já enviado um total de %d peças. \n", pecasCortadas,pecasProduzidas);
					    }
                        close(fd[i][0]);
                        exit(0);

                    } else if (i == 1) {
                        int pecasRecebidas = 0;

                        while(pecasRecebidas < PECAS) {
                            int aux = 0;
                            while(aux < M2_SPEED) {
                                read(fd[i-1][0], &aux, sizeof(int));
					        }
                            pecasRecebidas += aux;

                            int foldpieces = 0;
                            for(i = 0; i < M2_SPEED; i++) {
                                foldpieces ++;
					        } 
                            write(fd[i][1], &foldpieces, sizeof(int));
                            printf("M2 enviou %d peças para M3, tendo já enviado um total de %d peças. \n", foldpieces, pecasRecebidas);
                        }
                        close(fd[i-1][0]);
                        close(fd[i][0]);
                        exit(0);
                    } else if (i==2){
                        int pecasRecebidas = 0;
                        while(pecasRecebidas < PECAS) {
                            int aux = 0;
                            while(aux < M3_SPEED) {
                                read(fd[i-1][0], &aux, sizeof(int));
					        }

                            pecasRecebidas += aux;

                            int weldspieces = 0;
                            for(i = 0; i < M3_SPEED; i++) {
                                weldspieces ++;
					        } 

                            write(fd[i][1], &weldspieces, sizeof(int));
                            printf("M3 enviou %d peças para M4, tendo já enviado um total de %d peças. \n", weldspieces,pecasRecebidas);
                        }
                        close(fd[i-1][0]);
                        close(fd[i][0]);
                        exit(0);

                    } else if (i == 3){
                        int pecasRecebidas = 0;
                        while(pecasRecebidas < PECAS) {
                            int aux = 0;
                            while(aux < M4_SPEED) {
                                read(fd[i-1][0], &aux, sizeof(int));
					        }

                            pecasRecebidas += aux;

                            int packpieces = 0;
                            for(i = 0; i < M4_SPEED; i++) {
                                packpieces ++;
					        } 

                            write(fd[i][1], &packpieces, sizeof(int));
                            printf("M4 enviou %d peças para A1, tendo já enviado um total de %d peças.\n", packpieces, pecasRecebidas);
                        }
                        close(fd[i-1][0]);
                        close(fd[i][0]);
                        exit(0);
                    }    
                }  
		    }
        }   

    close(fd[3][1]);

    int pecasConcluidas = 0;
    int aux = 0;
    while(pecasConcluidas < PECAS){
        read(fd[3][0], &aux, sizeof(int));
        pecasConcluidas += aux;
    }
    printf("Operação Concluída!\n");
    close(fd[3][0]);

    return 0;

    }
}
