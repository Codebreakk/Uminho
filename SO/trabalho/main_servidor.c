#include "servidor.h"

int main(int argc, char * argv[]){

  my_printf("Starting Server...\n");

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

  // enquanto receber dados no fifo_client_server
  while((fifo_fd[0] = open_fifo_client_server()) > 0){

    char buf[BUF_SIZE];
    int bytes_read = 0;
    char * args [ARRAY_SIZE];
    while((bytes_read = read(fifo_fd[0], buf, BUF_SIZE)) > 0){
      // usado apenas pela função executar.
      // char * buf_copy;
      // memcpy(buf_copy, buf, bytes_read);
      buf[bytes_read] = '\0';
      //my_printf2("comando (before): %s\n", buf);

      // não podemos ler o buf por completo, apenas até à posição "bytes_read"
      if(strncmp(buf, TEMPO_INACTIVIDADE, bytes_read) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos = atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_inactividade(segundos);
        my_printf2("Finished tempo-inactividade (%d seconds).\n", segundos);
        close_fifo_server_client();

      }else if(strncmp(buf, TEMPO_EXECUCAO, bytes_read) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos =  atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_execucao(segundos);
        my_printf2("Finished tempo-execucao (%d seconds).\n", segundos);
        close_fifo_server_client();

      }else if(strncmp(buf, EXECUTAR, 8) == 0){
        char * comando = malloc(bytes_read);
        strncpy(comando, buf, strlen(buf)-9);
        //my_printf2("comando (after): %s\n", buf);
        open_fifo_server_client();
        executar(buf + 9);
        close_fifo_server_client();
      }else if(strncmp(buf, LISTAR, bytes_read) == 0){

      }else if(strncmp(buf, TERMINAR, bytes_read) == 0){

      }else if(strncmp(buf, HISTORICO, bytes_read) == 0){

      }else if(strncmp(buf, AJUDA, bytes_read) == 0){
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
  my_printf("Closing server...");
  return 0;
}
