#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handle_USR1 (int signo, siginfo_t *sinfo, void *context) {
    char mensagem[100];
	sprintf(mensagem, "SIGUSR1 has been captured and was sent by the process with PID %d\n", sinfo->si_pid);
	write(STDOUT_FILENO, mensagem, strlen(mensagem));
}


int main (int argc, char *argv[]){
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act, NULL);

    return 0;
}