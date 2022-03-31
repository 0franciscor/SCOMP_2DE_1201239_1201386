#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "struct.h"

#define QUANTIDADE_COMANDOS 2

volatile sig_atomic_t concluido;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
	concluido = 1;
}

int main(int argc, char* argv[]) {	

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handle_signal;
	sigaction(SIGCHLD, &act, NULL);

	comando comandos[QUANTIDADE_COMANDOS];
	strcpy(comandos[0].cmd, "SCOMP");
	comandos[0].tempo = 7;
	strcpy(comandos[1].cmd, "SCOMP");
	comandos[1].tempo = 5;
	
	
	pid_t p;
	
	int ret;
	for(int i = 0; i < QUANTIDADE_COMANDOS; i++) {
		concluido = 0;
		p = fork();
		if(p == -1) {
			exit(1);
		} else if(p > 0) {
			
			sleep(comandos[i].tempo);
			if(concluido == 0) {
				kill(p, SIGKILL);
				printf("The command %s didn't end in its allowed time!\n", comandos[i].cmd);
			} else 
				printf("Execution ended!\n");
			wait(NULL);
		} else {

			printf("Comando: \"%s\" \t", comandos[i].cmd);
			ret = execlp(comandos[i].cmd, comandos[i].cmd, (char *) NULL);
			exit(ret);
		}
	}
	
	return 0;
}
