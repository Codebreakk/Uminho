#include "cliente.h"

int open_fifo_client_server(){
  // Abrir fifo de escrita para o servidor
  if((fifo_fd[0] = open(FIFO_CLIENT_SERVER, O_CREAT | O_TRUNC | O_WRONLY)) < 0){
    perror("fifo client server open");
    exit(1);
  }

  return fifo_fd[0];
}

int close_fifo_client_server(){
  close(fifo_fd[0]);

  return 0;
}

int open_fifo_server_client(){
  // Abrir fifo de leitura do servidor
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, O_RDONLY)) < 0){
    perror("fifo server client open");
    exit(1);
  }

  return fifo_fd[1];
}

int close_fifo_server_client(){
  close(fifo_fd[1]);

  return 0;
}

// Substitui as flags do comando "./argus" pelo nome do comando
void replace_flags_with_names(char * argv[]){
  if(strcmp(argv[1], FLAG_TEMPO_INACTIVIDADE) == 0){
    strcpy(argv[1], TEMPO_INACTIVIDADE);
  }else if(strcmp(argv[1], FLAG_TEMPO_EXECUCAO) == 0){
    strcpy(argv[1], TEMPO_EXECUCAO);
  }else if(strcmp(argv[1], FLAG_EXECUTAR) == 0){
    strcpy(argv[1], EXECUTAR);
  }else if(strcmp(argv[1], FLAG_LISTAR) == 0){
    strcpy(argv[1], LISTAR);
  }else if(strcmp(argv[1], FLAG_TERMINAR) == 0){
    strcpy(argv[1], TERMINAR);
  }else if(strcmp(argv[1], FLAG_HISTORICO) == 0){
    strcpy(argv[1], HISTORICO);
  }else if(strcmp(argv[1], FLAG_AJUDA) == 0){
    strcpy(argv[1], AJUDA);
  }
}

// Recebe os comandos já tratados pela main e, se o comando for válido, envia
// para o servidor
int run_argus(int argc, char * argv[]){

  if(strcmp(argv[0], TEMPO_INACTIVIDADE) == 0 && argc == 2){
    // tempo-inactividade: ./argus -i n
    // Pedir ao servidor para executar "tempo_inactividade(n)"
  }else if(strcmp(argv[0], TEMPO_EXECUCAO) == 0 && argc == 2){
    // tempo-execução:     ./argus -m n
    // Pedir ao servidor para executar "tempo_execucao(n)"
  }else if(strcmp(argv[0], EXECUTAR) == 0 && argc == 2){
    // executar:           ./argus -e "p1|p2...|pn"
    // Pedir ao servidor para executar "executar("p1|p2...|pn")"
  }else if(strcmp(argv[0], LISTAR) == 0 && argc == 1){
    // listar:             ./argus -l
    // Pedir ao servidor para executar "listar()"
  }else if(strcmp(argv[0], TERMINAR) == 0 && argc == 2){
    // terminar:           ./argus -t n
    // Pedir ao servidor para executar "terminar(n)"
  }else if(strcmp(argv[0], HISTORICO) == 0 && argc == 1){
    // historico:          ./argus -r
    // Pedir ao servidor para executar "historico()"
  }else if(strcmp(argv[0], AJUDA) == 0 && argc == 1) {
    // ajuda:              ./argus -h
    // abrir fifo de escrita do cliente para o servidor
    open_fifo_client_server();
    // escrever comando para o servidor
    write(fifo_fd[0], argv[0], strlen(argv[0]));
    // fechar fifo de escrita do cliente para o servidor
    close_fifo_client_server();


    char buf[BUF_SIZE];
    ssize_t bytes_read = 0;
    // abrir fifo de leitura do servidor para o cliente
    open_fifo_server_client();
    // ler os dados escritos para o fifo e escrever para o buf
    while((bytes_read = read(fifo_fd[1], buf, BUF_SIZE)) != 0){
      // escrever do buf para o STDOUT
      write(1, buf, bytes_read);
    }
    // fechar fifo de leitura do servidor para o cliente
    close_fifo_server_client();
  }

  return 0;
}
