#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int createProcess();
void handle_signal(int sign);

int numProcessos = 0, maxProcessos = 5;

int main(){

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_signal;
    sigaction(SIGUSR1, &act, NULL);

    for(int i = 0; i < maxProcessos; i++){
        int processIndex = createProcess();

        if(processIndex != -1){
            int min = processIndex * 200;
            int max = (processIndex + 5) * 200;
            printf("Interval is [%d, %d]\n", min, max);
            
            pid_t fProcess = getppid();
            kill(fProcess, 10);
            return 0;
        }

    }

    return 0;
}

int createProcess(){
    numProcessos++;
    
    if(numProcessos <= maxProcessos){
        pid_t p = fork();

        if(p == 0)
            return numProcessos;
        else
            pause();

    }
    return -1;
}


void handle_signal(int sign){
    char mensagem[60];
    sprintf(mensagem, "SIGUSR1 signal captured, There are still %d processes left.\n", (5-numProcessos));
    
	write(STDOUT_FILENO, mensagem, strlen(mensagem));
}