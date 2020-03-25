#include "main.h"

// pid_t getpid(void);
// pid_t getppid(void);
// pid_t fork(void);
// void _exit(int status);
// pid_t wait(int *status);
// pid_t waitpid(pid_t pid, int *status, int options);
// int WIFEXITED(int status); /* macro */
// int WEXITSTATUS(int status); /* macro */

// exercício 1: implemente um programa que imprima o seu identificador de pro-
//              cesso e o do seu pai.
int ex1(){
  printf("Sou o filho com pid %d e o meu pai tem pid %d.\n", getpid(), getppid());

  _exit(0);
}

// exercício 2: implemente um programa que crie um processo filho.
//              Ambos os processos pai e filho devem imprimir o identificador de
//              processo próprio e o do seu pai. O processo pai deve ainda im-
//              primir o PID do seu filho.
int ex2(){
  pid_t pid;

  if((pid = fork()) == 0){
    // processo-filho
    printf("O meu pid é %d e o do meu pai é %d.\n", getpid(), getppid());
  }else{
    // processo-pai
    printf("O meu pid é %d, o do meu pai é %d e o do meu filho é %d.\n", getpid(), getppid(), pid);
  }

  _exit(0);
}

int ex3(){
  pid_t pid;
  int i = 0, status = -1;

  while(i < 10){
    if((pid = fork()) == 0){
      // processo-filho
      printf("Sou filho com pid %d e o meu pai tem pid %d.\n", getpid(), getppid());
      _exit(i);
    }else{
      // processo-pai
      //pid_t child = wait(&status);
      pid_t terminated_pid = wait(&status);
      if(WIFEXITED(status)){
        printf("Filho com pid %d iniciou na posição %d\n", terminated_pid, WEXITSTATUS(status));
      }
    }
    i++;
  }

  _exit(0);
}

/** exercício 4: Implemente um programa que crie 10 processos filhos que deverão
*              executar concorrentemente. O pai deverá esperar pelo fim de exe-
*              cução de todos os seus filhos, imprimindo os respectivos códigos
*              de saída.
*/
int ex4(){
  pid_t pid;
  int i = 0;
  int status;

  while(i < 10){
    if((pid = fork()) == 0){
      // processo-filho
      printf("Sou filho com pid %d e o meu pai tem pid %d.\n", getpid(), getppid());
      _exit(i);
    }
    i++;
  }

  i = 0;
  while(i < 10){
    if(pid != 0){
      pid_t child = wait(&status);
      if(WIFEXITED(status)){
        printf("Filho com pid %d iniciou na posição %d\n", child, WEXITSTATUS(status));
      }
    }
    i++;
  }

  _exit(0);
}

int ex5(){
  pid_t pid;
  int i = 0;
  int status;
  for(i = 0; i < 10; i++){
    if((pid = fork()) == 0){
      // processo-filho
      printf("Sou filho com pid %d e o meu pai tem pid %d.\n", getpid(), getppid());
    }else{
      pid_t terminated_pid = wait(&status);

      if(WIFEXITED(status)){
        printf("Filho com pid %d iniciou na posição %d\n", terminated_pid, WEXITSTATUS(status));
      }else{
        printf("[pai] process %d exited.", terminated_pid);
      }
      _exit(0);
    }
  }

  _exit(0);
}

int ex6(int argc, const char* argv[]){
  pid_t pid;
  int needle = atoi(argv[1]);
  int rows = 10;
  int cols = 10000;
  int rand_max = 10000;
  int status;
  int matrix[rows][cols];
  int occurrences = 0;

  //
  printf("generating numbers from 0 to %d... \n", rand_max);
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      matrix[i][j] = rand() % rand_max;
    }
  }

  // criar 10 processos por linha
  for (i = 0; i < rows; i++) {
    if((pid == fork()) == 0){
      //... matrix[i][coluna...]
      // _exit(0) || _exit(1)
    }
  }

  // Aguardar processos filho
  for (i = 0; i < rows; i++) {
    //... esperar por 1 processo
    //verificar código de saída, se saiu bem.
    // se WEXITSTATUS(status) = -> ocurrences++
  }

  _exit(0);
}

// a partir do cenário do exercício anterior, pretende-se que imprima por ordem
// crescente os números de linha onde existem ocorrências do número.
int ex7(int argc, const char* argv[]){
  pid_t pid;
  int needle = atoi(argv[1]);
  int rows = 10;
  int cols = 10000;
  int rand_max = 10000;
  int status;
  int matrix[rows][cols];
  int occurrences = 0;

  //
  printf("generating numbers from 0 to %d... \n", rand_max);
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      matrix[i][j] = rand() % rand_max;
    }
  }

  // criar 10 processos: 1 por linha
  for (i = 0; i < rows; i++) {
    if((pid == fork()) == 0){
      // ... matrix[i][coluna...]
      // _exit(0) || _exit(i)
    }
    // guardar pid na lista de pids.
  }

  // Aguardar por processos filho #linhas = 10
  for (i = 0; i < rows; i++) {
    // ... esperar por 1 (qualquer) processo
    //     OU por 1 processo filho em particular (lista de pids).
    // verificar código de saída, se saiu bem.
    // se WEXITSTATUS(status) = -> occurrences++
  }

  _exit(0);
}

// Exercício adicional
/** Implemente uma nova versão do programa feito nos exercícios anteriores que
*   opere sobre uma matriz persistida em ficheiro (em formato binário). A matriz
*   deve ser gerada aleatóriamente pelo processo pai e escrita em ficheiro, no
*   início da execução do programa. Após este passo deve ser possível pesquisar
*   a existência de um determinado número utilizando múltiplos processos.
*/
int ex_extra(){
  _exit(0):
}
