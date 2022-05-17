/********************************************************************************

Como neste exercício existem processos com diferentes níveis de prioridades, o 
padrão que optei seguir a lógica foi o do leitor/escritor, pois os escritores têm
prioridade sobre os processos leitores.

*********************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

#define CAPACIDADE 300

typedef struct{
	int lotacao;
	int vip_waiting;
	int special_waiting;
	int normal_waiting;
	int maxed_out;
}Club;
	
typedef struct{
	int prioridade;
}Cliente;

int main(){


    return 0;
}