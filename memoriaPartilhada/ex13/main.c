#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define QUANT_FILHOS 10

typedef struct{
    char* path;
    char* word;
    int occurrences;	
}info;

int main(int argc, char* argv[]) {

    int size = QUANT_FILHOS * sizeof(info); 
    info* infoPartilhada;

    int fd = shm_open("/ex13",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
	    perror("Creating or opening shared memory failure");
		shm_unlink("/ex13");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    
	infoPartilhada->path="1.txt";
	infoPartilhada->word="animal";
	infoPartilhada->occurrences=0;
	
	(infoPartilhada+1)->path="2.txt";
	(infoPartilhada+1)->word="faculdade";
	(infoPartilhada+1)->occurrences=0;
		
	(infoPartilhada+2)->path="3.txt";
	(infoPartilhada+2)->word="SCOMP";
	(infoPartilhada+2)->occurrences=0;
	
	(infoPartilhada+3)->path="4.txt";
	(infoPartilhada+3)->word="idade";
	(infoPartilhada+3)->occurrences=0;
		
	(infoPartilhada+4)->path="5.txt";
	(infoPartilhada+4)->word="morango";
	(infoPartilhada+4)->occurrences=0;
	
	(infoPartilhada+5)->path="6.txt";
	(infoPartilhada+5)->word="Este";
	(infoPartilhada+5)->occurrences=0;
		
	(infoPartilhada+6)->path="7.txt";
	(infoPartilhada+6)->word="trabalho";
	(infoPartilhada+6)->occurrences=0;
	
	(infoPartilhada+7)->path="8.txt";
	(infoPartilhada+7)->word="assim";
	(infoPartilhada+7)->occurrences=0;

	(infoPartilhada+8)->path="9.txt";
	(infoPartilhada+8)->word="o";
	(infoPartilhada+8)->occurrences=0;

	(infoPartilhada+9)->path="10.txt";
	(infoPartilhada+9)->word="computador";
	(infoPartilhada+9)->occurrences=0;

    pid_t pid;

    for (int i = 0; i < QUANT_FILHOS ; ++i) {
        pid = fork();

        if(pid==0){
			
			FILE* file = fopen((infoPartilhada+i)->path,"r");

			if(file!=NULL){
				
				char line[100];
                int count = 0;

				while(fgets(line,sizeof(line),file)){

                    char * procurar = (infoPartilhada+i)->word;
                    char * str = line;

                    int tamanhoLinha = strlen(line);      
                    int tamanhoPalavra = strlen((infoPartilhada+i)->word); 

        
                    int found;

                    for(int j = 0; j <= tamanhoLinha-tamanhoPalavra; j++){
                        found = 1;

                        for(int k =0; k <tamanhoPalavra; k++){
                            if(str[j + k] != procurar[k]){
                                found = 0;
                                break;
                            }
                        }

                        if(found == 1){
                            count++;
                        }
                    }
                }

                (infoPartilhada+i)->occurrences = count;
                fclose(file);

			}else{
			    printf("Não foi possível encontrar o ficheiro: %s\n",(infoPartilhada+i)->path);
			}

            if (munmap(infoPartilhada, size) < 0) {
                printf("Error at munmap()!\n");
                 return 4;
            }

            if (close(fd) < 0) {
                printf("Error at close()!\n");
                return 5;
            }
			
			exit(1);
		}
    }

    for(int i=0;i<QUANT_FILHOS;i++){
		wait(NULL);
	}
	
	for(int i=0;i<QUANT_FILHOS;i++){
		printf("Ficheiro %s encontrou a palavra \"%s\" %d vezes\n",(infoPartilhada+i)->path,(infoPartilhada+i)->word, (infoPartilhada+i)->occurrences);
	}

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/ex13") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }
	
    return 0;

}
