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
        close(fd[1]);
		
		
		close(fd[0]);
		exit(0);
        
    } else if (p==0){
        close(fd[0]);
		
		
		
		close(fd[1]);
    }

    return 0;
}