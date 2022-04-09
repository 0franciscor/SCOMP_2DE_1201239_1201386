#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE 50000
#define QUANT_FILHOS 10
#define META 20

typedef struct{
	int customer_code;
	int product_code;
	int quantity;
} sales;

struct pipeStruct{
    int productCode;
};

int main(int argc, char* argv[]) {

    int fd[2];
	if(pipe(fd) == -1) {
        perror("Pipe Falhou");
        return 1;
    }

    sales infos[SIZE];
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < SIZE; i++) {
		infos[i].customer_code = rand() % 100;
		infos[i].product_code = rand() % 100000;
		infos[i].quantity = rand() % 30;
	}

    pid_t pids[QUANT_FILHOS];

    for(int i = 0; i < QUANT_FILHOS; i++) {
		pids[i] = fork();
		if(pids[i] < 0) {
			perror("Fork Falhou");
			return 2;
		} else if(pids[i] == 0) {
			close(fd[0]);
			
			for(int j = i * 5000; j < (i+1) * 5000; j++) {
				if(infos[i].quantity > META) {
                    struct pipeStruct product;
					product.productCode = infos[j].product_code;
                    write(fd[1], &product, sizeof(product));
				}
			}

			close(fd[1]);
			exit(0);
		}
	}
	
	close(fd[1]);

    for (int i = 0; i < QUANT_FILHOS; i++) {
        wait(NULL);
    }
    
	int aux = 0;
    int resultados[SIZE];
	struct pipeStruct valoresObtidos;	
	while(read(fd[0], &valoresObtidos, sizeof(valoresObtidos)) != 0) {
			resultados[aux] = valoresObtidos.productCode;
			aux++;
	}
		
	close(fd[0]);
	
	printf("Produtos que foram vendidas mais de 20 quantidades\n");
	for(int i = 0; i < aux; i++) {
		printf("Código: %d\n", resultados[i]);
	}
	
	return 0;

}