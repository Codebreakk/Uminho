#include "main.h"

/** Chamadas ao sistema
* sighandler_t signal(int signum, sighandler_t handler);
* int kill(pid_t pid, int sig);
* unsigned int alarm(unsigned int seconds);
* int pause(void);
*/

/** Exercício 1
Usando SIGINT, SIGQUIT e SIGALRM, escreva um programa que vá contando o tempo em
segundos desde que começou. Se, entretanto, o utilizador carregar em Ctrl+C, o
programa deverá imprimir o tempo passado. Se carregar em Ctrl+\ o programa deve-
rá indicar quantas vezes o utilizador carregou em Ctrl+C e terminar de seguida.
*/
int ctrl_c_counter = 0;
int seconds = 0;

void sigint_handler(int signum){
  printf("sigint received: code %d\n", signum);
  printf("%d\n", seconds);
  ctrl_c_counter++;
}

void sigquit_handler(int signum){
  printf("sigquit received: code %d\n", signum);
  printf("%d\n", ctrl_c_counter);
  exit(0);
}

void sigalrm_handler(int signum){
  //printf("sigalrm received: code %d\n", signum);
  seconds++;
  alarm(1);
}

int ex1(){
  alarm(1);
  if(signal(SIGINT, sigint_handler) == SIG_ERR){
    perror("SIGINT Failed.");
    exit(1);
  }

  if(signal(SIGQUIT, sigquit_handler) == SIG_ERR){
    perror("SIGQUIT Failed.");
    exit(1);
  }

  if(signal(SIGALRM, sigalrm_handler) == SIG_ERR){
    perror("SIGALRM Failed.");
    exit(1);
  }

  while(1){
    pause();
    printf("interrupted\n");
  }

  return 0;
}
