#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int spawn_childs(int n);

int main() {
    int id = spawn_childs(6);

    if(id > 0)
        exit(id * 2);
    
    for(int i = 0; i < 6; i++){
        int status;
        
        wait(&status);
        if(WIFEXITED(status)){
            printf("%d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}

int spawn_childs(int n){
    pid_t p;

    for (int i = 0; i < n; i++){
        p = fork();
        
        if (p == 0)
            return i + 1;
        else if (p < 0)
            return -1;
    }
    return 0;
}