/********************************************************************************

Padrão: Leitores / Escritores

Pseudo-Código:
	criação de 3 semáforos
	criação e iniciaização da memória partilhada 

	criação dos sensores
	for every sensor {

		for each medição {

		
			verifica se pode escrever
			gera a medição
			guarda a medição na posição indicada
			
			if(existir medição anterior) {
				obter medição anterior
				if(medição atual < 50 and medição anterior < 50) {
					diminui o número de sensores em estado de alarme 
				}
			} else {
				if(medição atual < 50) {
					diminui o número de sensores em estado de alarme 
				}
			}

            if(medição> 50) 
			    aumenta o número de sensores em estado de alarme 
			}
		}

		altera o index do próximo valor a escrever
		guarda o sensor que
		
		desbloqueia semáforo de leitura
		espera pelo semáforo de sincronização
		desbloqueia semáforo de escrita
		sleeps
		}
		exit
	}

	controlador 
	for each medição {
		obtêm o sensor que escreveu
		obtêm o valor escrito
		imprime a informação
		desbloqueia semáforo de sincronização
	}

	elimina os semáforos e a área de memória partilhada
	
********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>

#define QUANT_SENSORES 5
#define QUANT_VALORES 6
#define LIMITE 50

typedef struct {
    int measurement[QUANT_SENSORES * QUANT_VALORES];
    int numSensorsAlarmState;
    int indexValorAEscrever[QUANT_SENSORES];
    int sensorQueEscreveu;
} info;

int main(){

    sem_t *canWrite, *canRead, *sincronizacao;

    if ((canWrite = sem_open("/swrite", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((canRead = sem_open("/sread", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((sincronizacao = sem_open("/sincronizacao", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	
	
	int size = sizeof(info); 
	info *infoPartilhada;

    int fd = shm_open("/exer19",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
	  perror("Creating or opening shared memory failure");
		shm_unlink("/exer19");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    infoPartilhada->numSensorsAlarmState = 0;

    for (int i = 0; i< QUANT_SENSORES; i++){
        infoPartilhada->indexValorAEscrever[i] = 0;

    }
    
    for(int i = 0; i < QUANT_SENSORES; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){
            // Sensores

            for(int j = 0; j < QUANT_VALORES; j++) {

                if (sem_wait(canWrite) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                srand(time(NULL) ^ (i*j));
                int currentMeasurement = rand() % 101;

                int indexEscrever = infoPartilhada->indexValorAEscrever[i];

                infoPartilhada->measurement[i*QUANT_VALORES + indexEscrever] = currentMeasurement;

                if(j != 0) {
                    int previousMeasurement =  infoPartilhada->measurement[i*QUANT_VALORES + indexEscrever];
                    if(currentMeasurement < LIMITE && previousMeasurement < LIMITE) {
                        infoPartilhada->numSensorsAlarmState--;
                    }
                } else {
                    if(currentMeasurement < LIMITE){
                        infoPartilhada->numSensorsAlarmState--;
                    }

                }
                
                if(currentMeasurement > LIMITE) {
                    infoPartilhada->numSensorsAlarmState++;
                }

                infoPartilhada->indexValorAEscrever[i]++;

                infoPartilhada->sensorQueEscreveu = i;

                if (sem_post(canRead) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                if (sem_wait(sincronizacao) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                if (sem_post(canWrite) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

                sleep(1);
                
            }

            exit(0);

        }

    }

    // Controller
    for (int i = 0; i < QUANT_SENSORES * QUANT_VALORES; i++) {
        if (sem_wait(canRead) == -1) {
            perror("Error at sem_wait().");
            exit(3);
        }

        int sensor = infoPartilhada->sensorQueEscreveu;
        int aux =  infoPartilhada->indexValorAEscrever[sensor] - 1;

        printf("Sensor %d Measurement %d: %d\n", sensor,aux, infoPartilhada->measurement[sensor*QUANT_VALORES + aux]);
        printf("Quantidade de sensores em estado de alarme: %d\n",infoPartilhada->numSensorsAlarmState);

        if (sem_post(sincronizacao) == -1) {
            perror("Error at sem_wait().");
            exit(3);
        }
        
    }

    for (int i = 0; i < QUANT_SENSORES; i++) {
        wait(null);
    }

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/exer19") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }

    if (sem_unlink("/swrite") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/sread") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

     if (sem_unlink("/sincronizacao") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    return 0;
}
