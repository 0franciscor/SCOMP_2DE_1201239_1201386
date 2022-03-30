#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handle_signais(int sig) {
	char info[100];
	if(sig == SIGINT){
		sprintf(info, "I won't let the process end with CTRL-C!\n");
    } else if (sig == SIGQUIT) {
		sprintf(info, "I won't let the process end with CTRL-\\!\n");
    }
	write(STDOUT_FILENO, info, strlen(info));
}

int main(int argc, char* argv[]) {

    struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handle_signais;
	sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    for(;;){
        printf("I Like Signal\n");
        sleep(1);
    }

    return 0;
}