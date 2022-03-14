#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    int status;
    int valor1; /* valor de saída do filho 1 */
    int valor2; /* valor de saída do filho 2 */
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
            waitpid(p1, &status, 0);
            if (WIFEXITED(status)) {
                valor1= WEXITSTATUS(status);
            }  
            waitpid(p2, &status, 0);
            if (WIFEXITED(status)) {
                valor2= WEXITSTATUS(status);
            } 
            printf("O filho 1 retornou o valor: %d\n",valor1);
            printf("O filho 2 retornou o valor: %d\n",valor2);
        }
    }

    return 0;
}