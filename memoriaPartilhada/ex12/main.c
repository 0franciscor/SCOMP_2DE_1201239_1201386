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

typedef struct {
    int numero;
    int disciplinas[NR_DISC];
    int readyToRead;
    char nome[STR_SIZE];
} aluno;

int main(){

    aluno *student;
    
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
        
		int max = student->disciplinas[0];
		int min = student->disciplinas[0];
        float average = 0;

		for(int i = 0; i < NR_DISC; i++) {
			average += student->disciplinas[i];

			if (student->disciplinas[i] > max)
				max = student->disciplinas[i];
			
            if (student->disciplinas[i]< min)
				min = student->disciplinas[i];
		}
        average /= NR_DISC;

        printf("Student: %s Minimum grade: %d Maximum grade: %d Average: %.2f\n", student->nome, min, max, average);

        exit(0);
    }

    wait(NULL);

    fd = munmap(student, sizeof(aluno));

	fd = shm_unlink("/ex12");

	close(fd);

    return 0;
}