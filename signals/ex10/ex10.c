#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void handle_signal(int sig);

int main(){

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_signal;
    sigaction(SIGALRM, &act, NULL);

    printf("Input a String:\n");
    char string[100];
    scanf("%s", string);

    int i = 0, stringSize = 0; 
    while(string[i] != '\0'){
        stringSize++;
        i++;
    }
    
    printf("Number of characters: %d\n", stringSize);

    sleep(20);
    printf("Successful execution!\n");

//Now on to inserting the second string:

    alarm(10);

    char string2[stringSize];
    scanf("%s", string2);

    return 0;
}

void handle_signal(int sig){
    char mensagem[50];
    sprintf(mensagem, "Too slow, that is why the program will end!\n");
    
	write(STDOUT_FILENO, mensagem, strlen(mensagem));
}