#include "servidor.h"

/** Abre o fifo do servidor para o cliente no modo de escrita */
int open_fifo_server_client(){
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, O_CREAT | O_TRUNC | O_WRONLY)) < 0){
    perror("fifo server client open");
    // não faz sentido terminar o servidor quando falha a abertura dos pipes,
    // mas temos de fazer alguma coisa quando falha (ex.: escrever para o termi-
    // nal a informar que falhou).
    //exit(1);
    return -1;
  }
  my_printf("fifo server client is now open.\n");

  return fifo_fd[1];
}

/** Fecha o fifo do servidor para o cliente */
int close_fifo_server_client(){
  close(fifo_fd[1]);
  my_printf("fifo server client is now closed.\n");

  return 0;
}

/** Abre o fifo do cliente para o servidor no modo de leitura */
int open_fifo_client_server(){
  if((fifo_fd[0] = open(FIFO_CLIENT_SERVER, O_RDONLY)) < 0){
    perror("fifo client server open");
    return -1;
  }
  my_printf("fifo client server is now open.\n");

  return fifo_fd[0];
}

/** Fecha o fifo do cliente para o servidor */
int close_fifo_client_server(){
  close(fifo_fd[0]);
  my_printf("fifo client server is now closed.\n");
  
  return 0;
}

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
  char buf[BUF_SIZE];

  // Abrir ficheiro com a informação a imprimir
  int file_help = open(HELP, O_RDONLY);
  if(file_help < 0){
    perror("open file help");
    return 1;
  }

  // ler do ficheiro para o buf e escrever para o fifo_client_server.
  ssize_t bytes = 0;
  while((bytes = read(file_help, buf, BUF_SIZE)) != 0){
    write(fifo_fd[1], buf, bytes);
  }

  close(file_help);
  my_printf("Ajuda terminated.\n");

  return 0;
}
