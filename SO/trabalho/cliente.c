#include "cliente.h"

// Substitui as flags do comando "./argus" pelo nome do comando.
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

int run_argus(int argc, char * argv[]){
  int fifo_fd[2];

  // Abrir fifo de escrita para o servidor
  if((fifo_fd[0] = open(FIFO_CLIENT_SERVER, 0666)) < 0){
    perror("fifo client to server open");
    exit(1);
  }

  // Abrir fifo de leitura do servidor
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, 0666)) < 0){
    perror("fifo server to client open");
    exit(1);
  }

  if(strcmp(argv[0], TEMPO_INACTIVIDADE) == 0 && argc == 2){
    // tempo-inactividade: ./argus -i n
    // Pedir ao servidor para executar "tempo_inactividade(n)"
    //my_printf(argv[0]);
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
    // Pedir ao servidor para executar "ajuda()"
    //my_printf("we're in\n");
    write(fifo_fd[0], argv[0], strlen(argv[0]));

    char buf[100];
    int bytes_read;
    while((bytes_read = read(fifo_fd[1], buf, 100)) > 0){
      write(1, buf, bytes_read);
    }
  }

  return 0;
}
