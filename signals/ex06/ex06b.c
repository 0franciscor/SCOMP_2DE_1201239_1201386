#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handle_signal(int sign);
void blockSignals();
struct sigaction act;

int main(int argc, char* argv[]){

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_signal;
    sigaction(SIGUSR1, &act, NULL);

    for(;;){
        printf("I'm Working!\n");
        sleep(1);
    }

    return 0;
}

void handle_signal(int sign){
    blockSignals(act); 
}

void blockSignals(struct sigaction act){
    sigfillset(&act.sa_mask); 
}