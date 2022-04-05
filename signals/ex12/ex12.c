#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

sig_atomic_t simula1();
sig_atomic_t simula2();
void handleSignal(int sig);
void handleSignalChild(int sig);

sig_atomic_t finishedProcs = 0, efficientAlgorithm = 0;

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
        pid_t fProcess = getppid();

        if(simula1() != 0){
            kill(fProcess, 10); //USR1
        } else{
            kill(fProcess, 12); //USR2
        }
        
        pause();
        if(efficientAlgorithm == 1){
            simula2();
        }
        exit(0);
    }

    while(finishedProcs < 25) {
        printf("%d - Simula1\n", finishedProcs);
        sleep(1);
    }

    if(efficientAlgorithm == 0) {
        printf("Inefficient algorithm!\n");

        for(int i = 0; i < 50; i++) {
            kill(processArray[i], 9);
        }

    } else {
        act.sa_handler = handleSignalChild;
        sigaction(SIGUSR1, &act, NULL);

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

sig_atomic_t simula1(){
    return 0; //testing purposes
}

sig_atomic_t simula2(){
    return 1; //testing purposes
}

void handleSignal(int sig){
    finishedProcs++;
    if(sig == 10)
        efficientAlgorithm = 1;
}

void handleSignalChild(int sig){
    efficientAlgorithm = 1;
}