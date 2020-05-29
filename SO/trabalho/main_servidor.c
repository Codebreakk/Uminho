#include "servidor.h"

int open_fifo_server_client(){
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, 0666)) < 0){
    perror("fifo server client open");
    // não faz sentido terminar o servidor quando falha a abertura dos pipes.
    //exit(1);
  }
  my_printf("fifo server client is now open.\n");
}

int main(int argc, char * argv[]){

  my_printf("Starting Server...\n");

  // Verifica se os fifos já estão criados e cria-os se necessário.
  if(access(FIFO_CLIENT_SERVER, F_OK) == -1){
    if(mkfifo(FIFO_CLIENT_SERVER, 0666) < 0){
      perror("mkfifo client server");
      exit(1);
    }
    my_printf("fifo client server has been created.\n");
  }

  if(access(FIFO_SERVER_CLIENT, F_OK) == -1){
    if(mkfifo(FIFO_SERVER_CLIENT, 0666) < 0){
      perror("mkfifo server client");
      exit(1);
    }
    my_printf("fifo server client has been created.\n");
  }

  // enquanto receber dados no fifo_client_server
  while((fifo_fd[0] = open(FIFO_CLIENT_SERVER, 0666)) > 0){
    my_printf("fifo client server is now open.\n");

    char buf[100];
    int bytes_read;
    while((bytes_read = read(fifo_fd[0], buf, 100)) > 0){
      // abrir fifo para escrever do servidor para o cliente
      open_fifo_server_client();
      ajuda();
      close(fifo_fd[1]);
    }

    close(fifo_fd[0]);
  }

  return 0;
}
