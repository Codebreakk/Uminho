#include "servidor.h"

int main(int argc, char * argv[]){

  my_printf("Starting Server...\n");

  // Verifica se os fifos já estão criados e cria-os se necessário.
  if(access(FIFO_CLIENT_SERVER, F_OK) == -1){
    if(mkfifo(FIFO_CLIENT_SERVER, 0666) < 0){
      perror("mkfifo client server");
      //exit(1);
    }
    my_printf("fifo client server has been created.\n");
  }

  if(access(FIFO_SERVER_CLIENT, F_OK) == -1){
    if(mkfifo(FIFO_SERVER_CLIENT, 0666) < 0){
      perror("mkfifo server client");
      //exit(1);
    }
    my_printf("fifo server client has been created.\n");
  }

  // enquanto receber dados no fifo_client_server
  while((fifo_fd[0] = open_fifo_client_server()) > 0){

    char buf[BUF_SIZE];
    int bytes_read;
    while((bytes_read = read(fifo_fd[0], buf, BUF_SIZE)) > 0){
      // não podemos ler o buf por completo, apenas até à posição "bytes_read"
      if(strncmp(buf, "ajuda", bytes_read) == 0){
        // abrir fifo para escrever do servidor para o cliente
        open_fifo_server_client();
        // executar comando
        ajuda();
        my_printf("Finished ajuda.\n");
        // fechar fifo
        close_fifo_server_client();
      }
    }

    close_fifo_client_server();
  }

  my_printf("Closing server...");
  return 0;
}
