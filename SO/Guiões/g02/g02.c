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
      printf("Filho com pid %d iniciou na posição %d\n", pid, status);
    }
    i++;
  }

  _exit(0);
}

int ex3_alt(){
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
      printf("Filho com pid %d iniciou na posição %d\n", child, status);
    }
    i++;
  }

  _exit(0);
}
