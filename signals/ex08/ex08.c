#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int createProcess();
void handle_signal(int sign);

sig_atomic_t numProcessos = 0, maxProcessos = 6;

int main(){

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_signal;
    sigaction(SIGCHLD, &act, NULL);

    for(int i = 0; i < maxProcessos; i++){
        int processIndex = createProcess();

        if(processIndex != -1){
            int min = processIndex * 200;
            int max = (processIndex + 5) * 200;
            printf("Interval is [%d, %d]\n", min, max);
            
            exit(0);
        }

    }

    return 0;
}

int createProcess(){    
    pid_t p;

    if(numProcessos <= maxProcessos){
        p = fork();

        if(p == 0)
            return numProcessos;
        else{
            int status;
            pause();
            waitpid(p, &status, 0);
        }

    }
    return -1;
}


void handle_signal(int sign){
    char mensagem[60];
    sprintf(mensagem, "SIGCHLD signal captured, There are %d processes left.\n", (5-numProcessos));
    
	write(STDOUT_FILENO, mensagem, strlen(mensagem));
    numProcessos++;
}