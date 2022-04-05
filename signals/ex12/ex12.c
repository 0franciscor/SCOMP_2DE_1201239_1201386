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
    int index; 

    for(int i = 0; i < 50; i++){
        p = fork();
        if(p == 0){
            index = i;
            break;
        }
        processArray[i] = p;
    }
        
    if(p == 0){
        pid_t fProcess = getppid();

        if(index < 25){
            if(simula1() != 0){
                kill(fProcess, 10); //USR1
            } else{
                kill(fProcess, 12); //USR2
            }
        }
        pause();
        if(efficientAlgorithm == 1){
            simula2();
        }
        exit(0);
    }

    while(finishedProcs < 25) {
        printf("%d processes finished Simula1\n", finishedProcs);
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
            printf("Process: %d finished task2\n", processArray[i]);
        }
        
    }
    

    return 0;
}

sig_atomic_t simula1(){
    sleep(1);
    return 0;
}

sig_atomic_t simula2(){
    sleep(1);
    return 1;
}

void handleSignal(int sig){
    finishedProcs++;
    if(sig == 10)
        efficientAlgorithm = 1;
}

void handleSignalChild(int sig){
    efficientAlgorithm = 1;
}