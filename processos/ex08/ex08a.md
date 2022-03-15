
# Exercício 8

> A) In the shell, list the running processes with ps (look for the PIDs printed). Are all parent and child processes listed? Why?
>> Após analisar a lista de processos em execução através do comando ps da bash, foi possível verificar que apenas 1 dos 2 processos filhos criados constam na lista de  processos em execução pelo sistema.\
\
O processo que ainda se encontra em execução foi o processo cujo pai (que executou o fork()) não executou a função waitpid() (ou wait()) para. Visto que este processo filho executou a função exit(), caso não se use waitpid(), a informação de término do processo não será obtida.\
\
Consequentemente, até que esta seja recolhida, o processo passa a um estado zombie (representado por \<defunct\> na bash) e mantém-se em execução. Daí constar no output do comando ps.\
\
Já o processo identificado na função waitpid(), não vai constar, visto que após o exit(), a sua informação de saída vai ser obtida e o processo é então terminado.


> B) Is there anything particular about the child process? Explain.
>> The child process uses the exit(status) function, which requires the process who cloned it to obtain the exit information. Otherwise, it will not terminate by itself.