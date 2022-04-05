#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

struct pipeStruct{
    char frase1[20];
    char frase2[20];
};

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    pid_t p = fork();
    
    if (p>0){

        close(fd[0]);
    
        struct pipeStruct mensagens;
        strcpy(mensagens.frase1,"Hello World");
        strcpy(mensagens.frase2,"Goodbye!");
        
		write(fd[1], &mensagens, sizeof(mensagens));

        close(fd[1]);
		
        int status;

		waitpid(p, &status, 0);
		printf("Child %d exited with value %d.\n", p, WEXITSTATUS(status));

    } else if (p==0){

        close(fd[1]);

        struct pipeStruct recebido;
		
        read(fd[0], &recebido, sizeof(recebido));
        
		close(fd[0]);
        
		printf("%s\n", recebido.frase1);
		printf("%s\n", recebido.frase2);
	
		exit(0);

    }

    return 0;
}