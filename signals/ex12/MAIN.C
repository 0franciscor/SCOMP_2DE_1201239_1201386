#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int main(){

    for(int i = 3796; i < 4088; i++){
        kill(i, 9);
    }


    return 0;
}