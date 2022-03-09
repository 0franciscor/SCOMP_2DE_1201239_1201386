#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    int status;
    pid_t p1 = fork();

    if ( p1 == 0){
        sleep(1);
        exit(1);
    } else if (p1 >0){
        pid_t p2 = fork();
        if(p2==0){
            sleep(2);
            exit(2);
        } else if (p2 > 0){
            wait(NULL);
            wait(NULL);
            printf("O filho retornou o valor: %d\n",WEXITSTATUS(status));
            printf("O filho retornou o valor: %d\n",WEXITSTATUS(status));
        }
    }

    return 0;
}