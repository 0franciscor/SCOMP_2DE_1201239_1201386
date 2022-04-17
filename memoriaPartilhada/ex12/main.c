#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<sys/wait.h>

#define STR_SIZE 50
#define NR_DISC 10

struct aluno{
    int numero;
    int disciplinas[NR_DISC];
    int readyToRead;
    char nome[STR_SIZE];
};

int main(){

    struct aluno *student;
    
    int fd = shm_open("/ex12", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0) perror("Error when creating shared memory file.\n");
    if (ftruncate (fd, 100) < 0) perror("Error when executing ftruncate() function.\n");
    student = mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    student->readyToRead = 0;

    printf("Student's Name: ");
	scanf("%s", student->nome);

	for(int i = 0;i < 10; i++) {
		printf("Class %d: ", (i + 1));
		scanf("%d",&student->disciplinas[i]);
	}

	student->readyToRead = 1;

    pid_t p = fork();

    if(p == -1){
        perror("There was an error when forking the process.\n");
        return -1;
    }

    if(p == 0){

        struct aluno *student2;

        do{
            int fd2 = shm_open("/ex12", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
            if(fd2 < 0) perror("Error when creating shared memory file.\n");
            if (ftruncate (fd2, 100) < 0) perror("Error when executing ftruncate() function.\n");
            student2 = mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

            sleep(1);
        } while(student2->readyToRead == 0);

		int max = student2->disciplinas[0];
		int min = student2->disciplinas[0];
        float average = 0;

		for(int i = 0; i < NR_DISC; i++) {
			average += student2->disciplinas[i];

			if (student2->disciplinas[i] > max)
				max = student2->disciplinas[i];
			
            if (student2->disciplinas[i]< min)
				min = student2->disciplinas[i];
		}
        average /= NR_DISC;

        printf("Student: %s Minimum grade: %d Maximum grade: %d Average: %.2f\n", student2->nome, min, max, average);

        exit(0);
    }

    wait(NULL);

    return 0;
}