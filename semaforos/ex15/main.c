#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <semaphore.h>

#define VISITORS 10
#define VISITORS_PER_ROOM 5
#define SHOW_OPEN 1
#define SHOW_CLOSED 0
#define NUMBER_OF_SHOWS 3
#define SHOW_DURATION 5
#define SHOW_DELAY 1

typedef struct{
	int show;
} show_room;

//create n-child process from same parent:
int make_children(int n) {
    pid_t pid;
    int i;
	pid_t pidF = getpid();	//parent's pid
	for (i = 0; i < n; i++) {
		//fork the parent process
		pid = fork();

		//check if an error occurred
		if (pid < 0)
			return -1;
		//if it's a child, return index
		else if (pid == 0)
			return i;
		//if it's the parent and all the children 
		//have been created, return parent pid
		else if (getpid() == pidF && i == n - 1)
			return pidF;
	}
    return 0;
}

int main(){
	
	int i;
	int interactions = 0;

//--Shared-Memory:----------------------------------------------------------------------------------------------

	//shared memory variables
	int shm_field;
	int data_size = sizeof(show_room);
	
	shm_unlink("/ex15_shm");	//remove shared memory (if exists)

	//opening a new shared memory
	shm_field = shm_open("/ex15_shm",  O_CREAT | O_EXCL | O_RDWR , S_IRUSR|S_IWUSR);

	//handle shared memory opening error
    if (shm_field == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	//defining shared memory size and handling memory error	
	if (ftruncate (shm_field, data_size) == -1){
        perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
    }

	//mapping shared memory space to process address space
	show_room * shared_data = (show_room*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,shm_field,0);

	//handle mapping error
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}

//--Semaphores:-------------------------------------------------------------------------------------------------

	sem_unlink("/ex15_sem");	//remove semaphore (if exists)
	
	//opening a new semaphore
	sem_t *semaphore = sem_open("/ex15_sem", O_CREAT|O_EXCL, 0644, VISITORS_PER_ROOM);

	//handle semaphore creation error
	if(semaphore == SEM_FAILED){
		perror("Error creating the semaphore!\n");
		exit(EXIT_FAILURE);
	} 

//--Main-Body:--------------------------------------------------------------------------------------------------

	srand(time(NULL));
	shared_data->show = SHOW_CLOSED;

	//make children:
	int id = make_children(VISITORS); 

	//iterate though each child (visitor):
	for (i = 0; i < VISITORS; i++) {
		
		if (id == i){
			for(;;){
				//wait if show is full, enter and occupy a space in the show of not
				sem_wait(semaphore);
				if(shared_data->show == SHOW_OPEN){
					//random time in show from 1 to 5 seconds
					int time_in_show_room = rand() % 5 + 1;
					printf("Visitante %d entrou e ficou por %d segundos no show.\n", i + 1, time_in_show_room);
					sleep(time_in_show_room);
					printf("Visitante %d saiu do show.\n", i + 1);
				}
				//open space in show
				sem_post(semaphore);
			}
			exit(EXIT_SUCCESS);
		}
	}

	//parent opens and closes the shows
	for(interactions = 0; interactions < QUANT_SHOWS; interactions++){
		printf("========= Abriu o show por %d segundo(s).  =========\n", SHOW_DURATION);
		//open the show
		shared_data -> show = SHOW_OPEN;
		//wait for show duration
		sleep(SHOW_DURATION);
		//close the show
		shared_data -> show = SHOW_CLOSED;
		printf("========= Fechou o show por %d segundo(s). =========\n", SHOW_DELAY);
		//wait for next show delay
		sleep(SHOW_DELAY);
	}
	
	//wait for all children to exit:
    int status;
	for(i = 0; i < VISITORS; i++){
		wait(&status);
	}

//--Closings:-----------------------------------------------------------------------------------------

	//unmapping shared memory and handling errors
	if(munmap(shared_data, data_size) == -1){
		perror("Munmap failed.\n");
		exit(EXIT_FAILURE);
	}	

	//closing shared memory object and handling errors
	if(close(shm_field) == -1){	
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	//unlinking shared memory and handling errors
	if (shm_unlink("/ex15_shm") < 0){
        perror("unlink failed\n");
        exit(EXIT_FAILURE);
    }

	//closing semaphore objects and handling errors
	if (sem_close(semaphore) < 0) {
		perror("Error at sem close");
		exit(EXIT_FAILURE);
	}

	//closing semaphores and handling errors
	if (sem_unlink("/ex15_sem") < 0) {
		perror("Error at unlink sem");
		exit(EXIT_FAILURE);
	}

	return 0;
}