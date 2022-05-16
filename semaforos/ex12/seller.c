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

int mainMenu(){
    int opcaoSelecionada = -1;
    while(opcaoSelecionada != 0){
        printf("\n\n\nMenu de Opcões:\n\n1.Sell Ticket\n2.Last Sold Ticket Number\n0.Sair\n");
        printf("\nPlease Select an Option: ");
        scanf("%d", &opcaoSelecionada);

        if(opcaoSelecionada >= 0 && opcaoSelecionada < 3)
            return opcaoSelecionada;
        else
            printf("\n\nOpção inválida.\n\n\n");
    }
    return -1;
}

typedef struct {
    int number;
} ticket;

int main(){

    // ########################### SHARED MEMORY ################################

    int fd, size = sizeof(ticket);

    ticket *sTicket;
    
    fd = shm_open("/ex12", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    sTicket = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // ###########################################################################

    // ############################### SEMAPHORES ################################

    sem_t *sem;

    if ((sem = sem_open("12", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    // ###########################################################################

    int ticketNumber = 1;

    int option = -1;
    while(1 && option != 0){
        option = mainMenu();
        if (option == 1) {    
            int value; sem_getvalue(sem, &value);

            if(value == 0){
                sTicket->number = ticketNumber;
                ticketNumber++;

                sem_post(sem);
            }

        } else if (option == 2) {
            printf("\n\n\nCurrent ticket Number: %d\n\n\n", ticketNumber);
        }
    }

    fd = munmap(sTicket, size);
	fd = shm_unlink("/ex12");
	close(fd);

    sem_close(sem);
    sem_unlink("12");

    
    return 0;
}