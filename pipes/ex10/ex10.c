#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int numGenerator();

int main(){

    int fd[2];
    if(pipe(fd) == -1){
        printf("Error when creating Pipe");
        return -1;
    }

    int fd2[2];
    if(pipe(fd2) == -1){
        printf("Error when creating Pipe");
        return -1;
    }

    int canBet = 1;
    int credit = 20;

    pid_t p = fork();
    
    if(p == 0){
        close(fd[1]);
        close(fd2[0]);
    
        while(read(fd[0], &canBet, sizeof(int)) != 0 && canBet != 0){
            sleep(2);
            read(fd[0], &credit, sizeof(int));
            int bet = numGenerator();
            printf("Child - Credit: %d Bet: %d\n", credit, bet);
            write(fd2[1], &bet, sizeof(int));
        }

        close(fd[0]);
        close(fd2[1]);
        exit(0);
    }
    
    
    close(fd[0]);
    close(fd2[1]);

    
    while(credit > 0){
        write(fd[1], &canBet, sizeof(int));
        write(fd[1], &credit, sizeof(int));

        int numGenerated = numGenerator();

        printf("Slot - Generated Number: %d Credit: %d\n", numGenerated, credit);
        
        int numReceived = read(fd2[0], &numReceived, sizeof(int));
        
        if(numReceived == numGenerated)
            credit += 10;
        else
            credit -= 5;   
    }

    canBet = 0;
    credit = 0;

    write(fd[1], &canBet, sizeof(int));
    write(fd[1], &credit, sizeof(int));

    close(fd[1]);
    close(fd2[0]);
    

    return 0;
}

int numGenerator(){
    srand(time(0));
    int lower = 1, upper = 5;
    int numToGuess = (rand() % (upper - lower + 1)) + lower;

    return numToGuess;
}