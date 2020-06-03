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

int parse_comandos(int array_size, char* buf, char * args[]){
  int n = 0;

  args[n] = strtok(buf, "|");
  while(args[n] && n < array_size - 1){
    int len = strlen(args[0]);
    // remove o '\n' no final de algumas strings para evitar falhar comparações
    // com o "strcmp".
    if ((args[n])[len - 1] == '\n') {
      (args[n])[len -1] = '\0';
    }
    args[++n] = strtok(NULL, "|");
  }

  return 0;
}

/** Definir o tempo máximo (em segundos) de inactividade de comunicação num pipe
* anónimo (opção "-i n" da linha de comando).
*/
int tempo_inactividade(int segundos){
  if(segundos < 0){
    perror("tempo-inactividade");
    return 1;
  }

  max_inactividade = segundos;

  return 0;
}

/** Definir o tempo máximo de execução de uma tarefa (opção "-m n" da linha de
* comandos).
*/
int tempo_execucao(int segundos){
  if(segundos < 0){
    perror("tempo-execucao");
    return 1;
  }

  max_execucao = segundos;

  return 0;
}

int * pids;
int pids_count = 0;
int timeout = 0;

/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos){
  char * coms[ARRAY_SIZE];
  int result = -1;
  int pid = -1, status = -1;
  parse_comandos(ARRAY_SIZE, comandos, coms);

  int sizeof_coms = sizeof_string_array(coms);
  pids = (int *) malloc(sizeof(int)*sizeof_coms);
  // my_printf2("sizeof_coms %d\n", sizeof_coms); // DEBUG

  int n_pipes = 1;
  if(sizeof_coms == 1){
    // Partir o comando nos seus argumentos
    char* args[ARRAY_SIZE];
    tokenize(args, coms[0], ARRAY_SIZE);

    if(fork() == 0){
      result = execvp(args[0], args);
      // sleep(10); //DEBUG
    }else{
      wait(&status);
      return WEXITSTATUS(status);
    }
  }else if(sizeof_coms > 1){
    n_pipes = sizeof_coms - 1;

    int pipe_array[n_pipes][2];

    // Cada iteração representa 1 comando com os seus argumentos
    for(int i = 0; coms[i] != NULL; i++){
      // Partir o comando nos seus argumentos
      char* args[ARRAY_SIZE];
      tokenize(args, coms[i], ARRAY_SIZE);

      // Criar pipe anónimo
      if(pipe(pipe_array[i]) < 0){
        perror("pipe_array");
        my_printf2("Failed to create pipe_array[%d]\n", i);
        return 1;
      }

      if((pid = fork()) == 0){
        // Se estivermos no último comando, não precisamos de fechar o STDIN do
        // próximo pipe porque esse pipe não existe.
        if(i < n_pipes){
          close(pipe_array[i][0]);
        }

        // Se estivermos no primeiro comando não recebemos os dados de um pipe,
        // logo não copiamos nenhum pipe para o STDIN.
        if(i > 0){
          dup2(pipe_array[i-1][0], 0);
          close(pipe_array[i-1][0]);
        }

        // Se estivermos no último comando escrevemos para o STDOUT normal, logo
        // não copiamos nenhum pipe para o STDOUT.
        if(i < n_pipes){
          dup2(pipe_array[i][1], 1);
          close(pipe_array[i][1]);
        }

        // if(i == n_pipes){ // copia o fifo_fd no último comando para o STDOUT
        //   dup2(fifo_fd[1], 1);
        //   close_fifo_server_client();
        // }

        result = execvp(args[0], args);
        // if(result >= 0){
        //   exit(result);
        // }else{
        //   exit(-1);
        // }
      }
      // Processo pai fecha os pipes que os seus filhos já usaram.
      if(i > 0) close(pipe_array[i-1][0]);
      if(i < n_pipes){
        close(pipe_array[i][0]);
        close(pipe_array[i][1]);
      }
      // if(i == n_pipes){
      //   dup2(fifo_fd[1], 1);
      //   close_fifo_server_client();
      // }

      // check return value
      pids[i] = pid;
      // my_printf2("%s \n", coms[i]);
    }

    // tempo máximo de inactividade.
    if(max_inactividade >= 0){
      // alarm(0) cancela os alarmes activos.
      alarm(max_inactividade);
    }

    for (int i = 0; i < sizeof_coms; i++) {
      wait(NULL);
    }
  }

  return result;
  // return 0;
}

/** Listar tarefas em execução (opção "-l" da linha de comando). */
int listar(){
  return 0;
}

/** Terminar uma tarefa em execução (opção "-t n"). */
int terminar(int tarefa){
  return 0;
}

/** Listar registo histórico de tarefas terminadas (opção "-r"). */
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
