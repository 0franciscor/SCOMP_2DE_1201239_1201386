#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

typedef struct {
	int array[10];
	int index;
	int canRead;
	int beenConsumed;
} ringStruct;

int main() {

    ringStruct *estrutura;

    int fd = shm_open("/ex14", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0) perror("Error when creating shared memory file.\n");
    if (ftruncate (fd, sizeof(estrutura)) < 0) perror("Error when executing ftruncate() function.\n");
    estrutura = mmap(NULL, sizeof(estrutura), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	estrutura->index = 0;
	estrutura->beenConsumed = 0;
	estrutura->canRead = 0;

	pid_t p = fork();

	if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

	if (p == 0){

		while(estrutura->beenConsumed == 0){

			while(estrutura->canRead == 0){}

			printf("\nProcesso Consumidor:\n");
			int lastIndex = estrutura->array[0];
			for(int i = 0; i < 10; i++){
				if(i != 0 && lastIndex >= estrutura->array[i]){
					printf("Os números não são sucessivamente superiores. Logo, a apresentação será terminada.\n");
					break;
				}
				printf("Numero Escrito: %d\n", estrutura->array[i]);
			}

			estrutura->beenConsumed = 1;
			estrutura->canRead = 0;
			estrutura->index++;

			if(estrutura->index != 3)
				while(estrutura->beenConsumed == 1){}
		}
		
		exit(1);
	}
	
	int ronda = 1;
	while(estrutura->index < 3){

		printf("\nRonda: %d ====== Processo Produtor:\n", ronda);
		for(int j = 0; j < 10; j++){
			printf("Insira o valor de índice %d: ", j+1);
			scanf("%d", &estrutura->array[j]);
		}
		ronda++;
		estrutura->canRead = 1;			
		estrutura->beenConsumed = 0;
		
		while(estrutura->beenConsumed == 0){}
	}
		
	wait(NULL);

	fd = munmap(estrutura, sizeof(ringStruct));

	fd = shm_unlink("/ex14");

	close(fd);

	return 0;
}
