#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

typedef struct{
    char address[50];
    char name[30];
    int number;
    int filled;
} datarecord;

int mainMenu(){
    int opcaoSelecionada = -1;
    while(opcaoSelecionada != 0){
        printf("Menu de Opcões:\n\n1.Consult\n2.Insert\n3.Consult All\n0.Sair\n");
        printf("\nPlease Select an Option: ");
        scanf("%d", &opcaoSelecionada);

        if(opcaoSelecionada >= 0 && opcaoSelecionada < 4)
            return opcaoSelecionada;
        else
            printf("\n\nOpção inválida.\n\n\n");
    }
    return -1;
}

int main(){

    // ########################### SHARED MEMORY ################################

    int fd, size = sizeof(datarecord) * 100;

    datarecord *memAddress;
    
    fd = shm_open("/ex10", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    memAddress = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // ###########################################################################

    sem_t *sem;

    if ((sem = sem_open("10", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    int option = -1;
    pid_t p;
    while(1 && option != 0){
        option = mainMenu();
        p = fork();
        if(p == 0){
            printf("\n\n\n");
            
            switch(option){
                
                case 1:{
                    sem_wait(sem);
                    printf("Por favor insira o número desejado: ") ;
                    int numDesejado;
                    scanf("%d", &numDesejado);

                    datarecord record = *memAddress;
                    int i = 0;
                    while(record.filled != 0){
                        i++;
                        if(record.number == numDesejado){
                            printf("Name: %s\nAddress: %s\nNumber: %d", record.name, record.address, record.number);
                            break;
                        }

                        record = *(memAddress + i);
                    }

                    if(i == 0) printf("Não há nenhuma estrutura disponível.\n");
                    sem_post(sem);
                    
                    break;
                }

                case 2:{
                    sem_wait(sem);
                    printf("Número: ") ;
                    int num;
                    scanf("%d", &num);

                    printf("Nome: ");
                    char nome[30];
                    scanf("%s", nome);

                    printf("Morada: ");
                    char address[50];
                    scanf("%s", address);

                    datarecord record; 
                    record.number = num;
                    strcpy(record.name, nome);
                    strcpy(record.address, address);
                    record.filled = 1;

                    int i = 0;
                    datarecord auxRecord = *(memAddress + i);
                    while(auxRecord.filled != 0){
                        i++;
                        auxRecord = *(memAddress + i);
                    } 
                    *(memAddress + i) = record;
                     
                    sem_post(sem);

                    break;
                }

                case 3:{
                    sem_wait(sem);
                    
                    int i = 0;
                    datarecord record = *memAddress;
                    while(record.filled != 0){
                        i++;
                        printf("Name: %s\nAddress: %s\nNumber: %d", record.name, record.address, record.number);

                        record = *(memAddress + i);
                    }

                    if(i == 0) printf("Não há nenhuma estrutura disponível.\n");
                    sem_post(sem);
                    break;
                }
                
                default:
                    printf("\n\nAplicação Encerrada\n\n");
                    break;
            }

            printf("\n\n\n");

            exit(0);
        }
        wait(NULL);
    }


    fd = munmap(memAddress, size);
	fd = shm_unlink("/ex10");
	close(fd);
    
    sem_close(sem);
    sem_unlink("10");
    
    return 0;
}