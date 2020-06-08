#include "servidor.h"

int main(int argc, char * argv[]){

  my_printf("Starting Server...\n");

  queue_tarefas = malloc(sizeof(Queue));
  initQueue(queue_tarefas);
  // Verifica se os fifos já estão criados e cria-os se necessário.
  if(access(FIFO_CLIENT_SERVER, F_OK) == -1){
    if(mkfifo(FIFO_CLIENT_SERVER, 0640) < 0){
      perror("mkfifo client server");
      //exit(1);
    }
    my_printf("fifo client server has been created.\n");
  }

  if(access(FIFO_SERVER_CLIENT, F_OK) == -1){
    if(mkfifo(FIFO_SERVER_CLIENT, 0640) < 0){
      perror("mkfifo server client");
      //exit(1);
    }
    my_printf("fifo server client has been created.\n");
  }

  /** Abrir/criar ficheiro de log do servidor */
  log_fd = open(LOG, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  if(log_fd < 0){
    perror("open log_fd.");
  }else{
    my_printf("log_fd is now open.\n");
  }

  /** Abrir/criar ficheiro com histórico do servidor */
  historico_fd = open(HISTORICO, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  if(historico_fd < 0){
    perror("open historico_fd.");
  }else{
    my_printf("historico_fd is now open.\n");
  }

  // enquanto receber dados no fifo_client_server
  while((fifo_fd[0] = open_fifo_client_server()) > 0){

    char buf[BUF_SIZE];
    int bytes_read = 0;
    char * args [ARRAY_SIZE];
    while((bytes_read = read(fifo_fd[0], buf, BUF_SIZE)) > 0){
      // usado apenas pela função executar.
      buf[bytes_read] = '\0';
      // my_printf2("comando (before): %s\n", buf); // DEBUG

      // não podemos ler o buf por completo, apenas até à posição "bytes_read"
      if(strncmp(buf, TEMPO_INACTIVIDADE, strlen(TEMPO_INACTIVIDADE)) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos = atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_inactividade(segundos);
        my_printf2("Finished tempo-inactividade (%d seconds).\n", segundos);
        close_fifo_server_client();

      }else if(strncmp(buf, TEMPO_EXECUCAO, strlen(TEMPO_EXECUCAO)) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos =  atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_execucao(segundos);
        my_printf2("Finished tempo-execucao (%d seconds).\n", segundos);
        close_fifo_server_client();

      }else if(strncmp(buf, EXECUTAR, strlen(EXECUTAR)) == 0){
        char * comando = malloc(bytes_read);
        strncpy(comando, buf, strlen(buf)-9);
        //my_printf2("comando (after): %s\n", buf); // DEBUG
        open_fifo_server_client();
        setup_executar(buf + 9);
        // executar(buf + 9);
        my_printf("Finished executar.\n");
        close_fifo_server_client();
      }else if(strncmp(buf, LISTAR, strlen(LISTAR)) == 0){
        // my_printf("LISTAR\n"); // DEBUG
        open_fifo_server_client();
        listar();
        my_printf("Finished listar.\n");
        close_fifo_server_client();
      }else if(strncmp(buf, TERMINAR, strlen(TERMINAR)) == 0){

      }else if(strncmp(buf, HISTORICO, strlen(HISTORICO)) == 0){
        open_fifo_server_client();
        historico();
        my_printf("Finished historico.\n");
        close_fifo_server_client();
      }else if(strncmp(buf, AJUDA, strlen(AJUDA)) == 0){
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        // executar comando
        ajuda();
        my_printf("Finished ajuda.\n");
        // fechar fifo de escrita do servidor para o cliente
        close_fifo_server_client();
      }
    }
    // fechar fifo de leitura cliente para o servidor
    close_fifo_client_server();
  }

  close(log_fd);
  close(historico_fd);
  my_printf("Closing server...");
  return 0;
}
