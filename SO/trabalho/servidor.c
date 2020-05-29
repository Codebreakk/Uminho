#include "servidor.h"

#define AJUDA_TEMPO_INACTIVIDADE "\nargus -i n: Define o tempo máximo 'n' (em segundos) de inactividade de comunicação num pipe anónimo.\n"

/** Definir o tempo máximo (em segundos) de inactividade de comunicação num pipe
* anónimo (opção ""-i n" da linha de comando).
*/
int tempo_inactividade(int segundos){
  return 0;
}

/** Definir o tempo máximo de execução de uma tarefa (opção "-m n" da linha de
* comandos).
*/
int tempo_execucao(int segundos){
  return 0;
}

/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos){
  return 0;
}

/** Listar tarefas em execução (opção "-l" da linha de comando). */
int listar(){
  return 0;
}

/** Terminar uma tarefa em execução (opção "-t n"). */
int terminar(int tarefa){
  return 0;
}

/** Listar registo histórico de tarefas terminadas (opção -r). */
int historico(){
  return 0;
}

/** Apresentar ajuda à sua utilização (opção "-h"). */
int ajuda(){
  my_printf("we're in.\n");
  write(fifo_fd[1], AJUDA_TEMPO_INACTIVIDADE, strlen(AJUDA_TEMPO_INACTIVIDADE));

   return 0;
}
