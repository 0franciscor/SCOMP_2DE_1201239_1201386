#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    char info[64];

	sigset_t current_set;
	sigprocmask(0, NULL, &current_set); /* Examina e altera os sinais bloqueados */

    for(int i=1; i<SIGRTMAX; i++) {
        if(sigismember(&current_set, i)) { /* Verifica se o sinal especificado por i é membro do conjunto apontado por current_set */
			sprintf(info, "%2d - Sinal:  %s bloqueado.\n", i, strsignal(i)); 
			write(STDOUT_FILENO, info, strlen(info));
		}
    }

	
}

int main(int argc, char* argv[]) {

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask); /* All signals blocked */
    
    act.sa_sigaction = handle_signal;
	act.sa_flags = SA_SIGINFO;
	
	sigaction(SIGUSR1, &act, NULL);
	
	return 0;

}