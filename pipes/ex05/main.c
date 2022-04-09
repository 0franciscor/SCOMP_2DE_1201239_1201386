#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_SYZE 256

struct pipeStruct{
    char mensagem[MAX_SYZE];
};

int main(int argc, char* argv[]) {
	
	int up[2], down[2];

	if(pipe(up) == -1 || pipe(down) == -1) {
		perror("Pipe Falhou");
		return 1;
	}

	pid_t pid = fork();
    
	if(pid == -1) {
		perror("Fork Falhou");
		return 2;

	} else if( pid > 0) {

        // SERVER 

        close(up[1]);
		close(down[0]);
		
		struct pipeStruct texto;
		read(up[0], &texto, sizeof(texto));
		
		for (int i = 0; i < strlen(texto.mensagem); i++) {
			if (*(texto.mensagem + i) > 96 && *(texto.mensagem + i) < 123) {
				*(texto.mensagem + i) = *(texto.mensagem + i) - 32;
			} else if(*(texto.mensagem + i)>64 && *(texto.mensagem + i)<91){
				*(texto.mensagem + i) = *(texto.mensagem + i) + 32;
			}
		}
        
		write(down[1], &texto, sizeof(texto));
		
		wait(NULL);
		
	} else {

        // CLIENT 

		close(up[0]);
		close(down[1]);

		struct pipeStruct mensagens;
		struct pipeStruct str_upper;

		printf("Mensagem: ");
		fgets(mensagens.mensagem, MAX_SYZE, stdin);
		
		mensagens.mensagem[strlen(mensagens.mensagem) -1] = '\0';
	
		write(up[1], &mensagens, sizeof(mensagens));
		read(down[0], &str_upper, sizeof(str_upper));

		close(up[1]);
		close(down[0]);
		
		printf("%s\n", str_upper.mensagem);
		
		exit(0);

	}
	return 0;
}