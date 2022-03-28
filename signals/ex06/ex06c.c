#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

void handle_signal(int sign);
int USR1_COUNTER;

int main(int argc, char* argv[]){

    USR1_COUNTER = 0;
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_signal;
    sigaction(SIGUSR1, &act, NULL);

    pid_t p = fork();

    if(p!=0){
        for(;;){
            printf("Waiting for a Signal\n");
            sleep(1);
        }
    } else{
        struct timespec rqtp, rmtp = {0,10000000}; //tempo em milisegundos
        pid_t fProcess = getppid();
        for(int i = 10; i < 22; i++){
            kill(fProcess, i);
            nanosleep(&rqtp, &rmtp);
        }

    }

    return 0;
}

void handle_signal(int sign){
    USR1_COUNTER++;
    countSignals();
}

void countSignals(){
    char mensagem[50];
    sprintf(mensagem, "SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER);
    
	write(STDOUT_FILENO, mensagem, strlen(mensagem));
}