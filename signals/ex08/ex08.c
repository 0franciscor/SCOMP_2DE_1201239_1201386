#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int createProcess();
void handle_signal(int sign);

sig_atomic_t numProcessos = 5;

int main(){

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT | SA_NOCLDSTOP;
    act.sa_handler = handle_signal;
    sigaction(SIGCHLD, &act, NULL);
    
    int processIndex = createProcess();

    if(processIndex != -1){
        int min = processIndex * 200;
        int max = (processIndex + 5) * 200;
        printf("Interval is [%d, %d]\n", min, max);
        
        pid_t fProcess = getppid();
        kill(fProcess, 17);
        exit(0);
    }

    while(numProcessos) {
        pause();
    }

    int i = 0;
    while(i < 5) {
        wait(NULL);
        i++;
    }

    return 0;
}

int createProcess(){   
    pid_t p; 
    for(int i = 0; i < numProcessos; i++){
        p = fork();
        if(p == 0)
            return (i+1);
    }
    return -1;
}


void handle_signal(int sign){
    numProcessos--;
}