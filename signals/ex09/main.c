#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) {

    pid_t p = fork();
    if (p>0){
        for(int i = 0; i < 4; i++) {
            printf("%d seconds passed\n", i);
			sleep(1);
		} 
        printf("Tarefe A terminada!\n");
		kill(p, SIGUSR1);
		wait(NULL);
    } else if (p == 0){
		 int r = (rand() % 5) + 1;
		sleep(r); 
        printf("Tarefe B terminada!\n");
		sleep(3); 
        printf("Tarefe C terminada!\n");
		exit(0);
    }

    return 0;
}