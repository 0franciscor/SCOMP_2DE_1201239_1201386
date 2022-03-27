#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    pid_t p = fork();
    
    if (p>0){
        close(fd[0]); // o pai não irá efetuar nenhuma leitura
        pid_t pid = getpid(); 
		printf("O meu pid: %d\n", pid);
        write(fd[1], &pid, sizeof(pid_t));
        close(fd[1]);
		wait(NULL);
    } else if (p==0){
        close(fd[1]);
		pid_t pid_parent;
		read(fd[0], &pid_parent, sizeof(pid_t));
		close(fd[0]);
		printf("Pid do pai: %d\n", pid_parent);
		exit(0);
    }

    return 0;
}