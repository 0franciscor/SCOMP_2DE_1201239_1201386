#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

sig_atomic_t finishedProcs = 0;
bool efficientAlgorithm = false;

void handleSignal(int sig){
    finishedProcs++;
    if(sig == 10)
        efficientAlgorithm = true;
}

void handleSignalChild(int sig){
    efficientAlgorithm = true;
}

bool simula1(){
    return false; //testing purposes
}

bool simula2(){
    return true; //testing purposes
}

int main(){

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handleSignal;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    pid_t processArray[50];
    pid_t p;

    for(int i = 0; i < 50; i++){
        p = fork();
        if(p == 0){
            break;
        }
        processArray[i] = p;
    }
        
    if(p == 0){
        struct sigaction act2;
        memset(&act2, 0, sizeof(struct sigaction));
        sigemptyset(&act2.sa_mask); /* No signals blocked */
        act2.sa_handler = handleSignalChild;
        sigaction(SIGUSR1, &act2, NULL);

        pid_t fProcess = getppid();

        if(simula1()){
            kill(fProcess, 10); //USR1
        } else{
            kill(fProcess, 12); //USR2
        }

        struct timespec aux, aux2;
        aux.tv_sec = 0;
        aux.tv_nsec = 500000000L;
        nanosleep(&aux, &aux2);
        
        pause();
        
        if(efficientAlgorithm){
            simula2();
        }
        exit(0);
    }   

    while(finishedProcs < 25){
        sleep(1);
        printf("%d - Simula1\n", finishedProcs);
    }
    
    if(!efficientAlgorithm) {
        printf("Inefficient algorithm!\n");

        for(int i = 0; i < 50; i++) {
            kill(processArray[i], 9);
        }
    } else {
        for(int i = 0; i < 50; i++) {
            kill(processArray[i], SIGUSR1);
        }

        for(int i = 0; i < 50; i++) {
            waitpid(processArray[i], NULL, 0);
            printf("Simula 2 - Process: %d\n", processArray[i]);
        }
    }
    
    
    return 0;
}

