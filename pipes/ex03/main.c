#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    pid_t p = fork();
    
    if (p>0){

        close(fd[0]);
    
        char frase1[] = "Hello World";
        int size = strlen(frase1);
        write(fd[1], &size, sizeof(int));
		write(fd[1], frase1, strlen(frase1));
		
		char frase2[] = "Goodbye!";
        size = strlen(frase2);
        write(fd[1], &size, sizeof(int));
		write(fd[1], frase2, strlen(frase2));
		
        int status;

		waitpid(p, &status, 0);
		printf("Child %d exited with value %d.\n", p, WEXITSTATUS(status));
		
		close(fd[1]);

    } else if (p==0){

        close(fd[1]);
		
		char frase1[100];
		char frase2[100];
        int size;
		
        read(fd[0], &size, sizeof(int));
        read(fd[0], frase1, sizeof(char) * size);
		printf("%s\n", frase1);

		read(fd[0], &size, sizeof(int));
        read(fd[0], frase2, sizeof(char) * size);
		printf("%s\n", frase2);
		
		close(fd[0]);
		exit(0);

    }

    return 0;
}