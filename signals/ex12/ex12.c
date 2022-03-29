#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

bool simula1();
bool simula2();
void createChild(pid_t processArray[50]);
void handleUSR1(int sig);
void handleUSR2(int sig);

bool efficientAlgorithm = false;
int numProcessos = 0;

int main(){

    struct sigaction actUSR1;
    struct sigaction actUSR2;

    memset(&actUSR1, 0, sizeof(struct sigaction));
    memset(&actUSR2, 0, sizeof(struct sigaction));
    sigemptyset(&actUSR1.sa_mask); /* No signals blocked */
    sigemptyset(&actUSR2.sa_mask); /* No signals blocked */
    actUSR1.sa_handler = handleUSR1;
    actUSR2.sa_handler = handleUSR2;
    sigaction(SIGUSR1, &actUSR1, NULL);
    sigaction(SIGUSR2, &actUSR2, NULL);

    pid_t processArray[50];
    
    createChild(processArray);

    return 0;
}

void createChild(pid_t processArray[50]){
    
    for(int i = 0; i < 50; i++){
        pid_t p = fork();
        processArray[i] = p;

        if(p == 0){
            bool sim1Result = simula1();
            pid_t fProcess = getppid();
            
            if(sim1Result)
                kill(fProcess, 10);
            else
                kill(fProcess, 12);
            
            sleep(1);

            exit(0);
        }
        
        if(p != 0 && i == 24 && numProcessos == 25){
            if(!efficientAlgorithm){
                printf("Inefficient algorithm!\n");
                for(int j = 0; j < 24; j++){
                    kill(processArray[j], 9);
                }
                break;
            }
                
            else{
                for(int j = 0; j < 24; j++){
                    kill(processArray[j], 12);
                }
            }
        }
    }
}

bool simula1(){
    return 0 + (rand() % (1 - 0 + 1)) == 1;
}

bool simula2(){
    return true;
}

void handleUSR1(int sig){
    numProcessos++;
    efficientAlgorithm = true;
}

void handleUSR2(int sig){
    simula2();
}