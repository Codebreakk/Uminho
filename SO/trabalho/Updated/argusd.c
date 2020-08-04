#include "argus.h"
#include "servidor.h"

int main(int argc, char * argv[]){

  my_printf("A iniciar o Servidor...\n");

  queue_tarefas = malloc(sizeof(Queue));
  initQueue(queue_tarefas);
  // Verifica se os fifos já estão criados e cria-os se necessário.
  if(access(FIFO_CLIENT_SERVER, F_OK) == -1){
    if(mkfifo(FIFO_CLIENT_SERVER, 0640) < 0){
      perror("Erro ao criar o FIFO cliente servidor.");
      //exit(1);
    }
    my_printf("FIFO cliente servidor foi criado com sucesso.\n");
  }

  if(access(FIFO_SERVER_CLIENT, F_OK) == -1){
    if(mkfifo(FIFO_SERVER_CLIENT, 0640) < 0){
      perror("Erro ao criar o FIFO servidor cliente.");
      //exit(1);
    }
    my_printf("FIFO servidor cliente foi criado com sucesso.\n");
  }

  /** Abrir/criar ficheiro de log do servidor */
  log_fd = open(LOG, O_CREAT | O_TRUNC | O_WRONLY, 0640);
  if(log_fd < 0){
    perror("Erro ao abrir o log_fd.");
  }else{
    my_printf("log_fd foi aberto com sucesso.\n");
  }

  /** Abrir/criar ficheiro com histórico do servidor */
  historico_fd = open(HISTORICO, O_CREAT | O_TRUNC | O_RDWR, 0640);
  if(historico_fd < 0){
    perror("Erro ao abrir o historico_fd.");
  }else{
    my_printf("historico_fd foi aberto com sucesso.\n");
  }

  // enquanto receber dados no fifo_client_server
  while((fifo_fd[0] = open_fifo_client_server()) > 0){

    char buf[BUF_SIZE];
    int bytes_read = 0;
    char * args [ARRAY_SIZE];
    while((bytes_read = read(fifo_fd[0], buf, BUF_SIZE)) > 0){
      // usado apenas pela função executar.
      buf[bytes_read] = '\0';

      // não podemos ler o buf por completo, apenas até à posição "bytes_read"
      if(strncmp(buf, TEMPO_INACTIVIDADE, strlen(TEMPO_INACTIVIDADE)) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos = atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_inactividade(segundos);
        my_printf2("Terminado tempo-inactividade (%d segundos).\n", segundos);
        char *msg = "Tempo máximo de inactividade: ";
        char *segundos_as_string = integer_to_string(segundos);
        char *s = " segundos\n";
        char *tempo_inactividade_msg = malloc(sizeof(msg) + sizeof(s) + sizeof(segundos_as_string));
        strcpy(tempo_inactividade_msg, msg);
        strcat(tempo_inactividade_msg, segundos_as_string);
        strcat(tempo_inactividade_msg, s);
        write(fifo_fd[1], tempo_inactividade_msg, strlen(tempo_inactividade_msg));
        close_fifo_server_client();
      }else if(strncmp(buf, TEMPO_EXECUCAO, strlen(TEMPO_EXECUCAO)) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int segundos = atoi(args[1]);
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        tempo_execucao(segundos);
        my_printf2("Terminado tempo-execucao (%d segundos).\n", segundos);
        char *msg = "Tempo máximo de execução: ";
        char *segundos_as_string = integer_to_string(segundos);
        char *s = " segundos\n";
        char *tempo_execucao_msg = malloc(sizeof(msg) + sizeof(s) + sizeof(segundos_as_string));
        strcpy(tempo_execucao_msg, msg);
        strcat(tempo_execucao_msg, segundos_as_string);
        strcat(tempo_execucao_msg, s);
        write(fifo_fd[1], tempo_execucao_msg, strlen(tempo_execucao_msg));
        close_fifo_server_client();
      }else if(strncmp(buf, EXECUTAR, strlen(EXECUTAR)) == 0){
        // open_fifo_server_client();
        char * comando = malloc(bytes_read);
        strncpy(comando, buf, strlen(buf)-9);
        setup_executar(buf + 9);
        // close_fifo_server_client();
      }else if(strncmp(buf, LISTAR, strlen(LISTAR)) == 0){
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        // executar comando
        listar();
        my_printf("Terminado listar.\n");
        // fechar fifo de escrita do servidor para o cliente
        close_fifo_server_client();
      }else if(strncmp(buf, TERMINAR, strlen(TERMINAR)) == 0){
        tokenize(args, buf, ARRAY_SIZE);
        int id_tarefa = atoi(args[1]);
        open_fifo_server_client();
        terminar(id_tarefa);
        close_fifo_server_client();
      }else if(strncmp(buf, HISTORICO, strlen(HISTORICO)) == 0){
        open_fifo_server_client();
        historico();
        my_printf("Terminado historico.\n");
        close_fifo_server_client();
      }else if(strncmp(buf, AJUDA, strlen(AJUDA)) == 0){
        // abrir fifo de escrita do servidor para o cliente
        open_fifo_server_client();
        // executar comando
        ajuda();
        my_printf("Terminado ajuda.\n");
        // fechar fifo de escrita do servidor para o cliente
        close_fifo_server_client();
      }
    }
    // fechar fifo de leitura cliente para o servidor
    close_fifo_client_server();
  }

  close(log_fd);
  close(historico_fd);
  my_printf("A terminar o Servidor...");
  return 0;
}
