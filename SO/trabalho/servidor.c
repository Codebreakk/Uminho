#include "servidor.h"

int max_inactividade = -1;
int max_execucao = -1;
int tarefa_id = 1;

// Inicializa a queue.
void initQueue(Queue *q){
  q->inicio = q->fim = NULL;
}

// Verifica se a queue está vazia.
int isEmpty(Queue *q){
  return ((q->inicio) == NULL);
}

// Adiciona uma nova tarefa à queue. Retorna 0 se correr com sucesso, -1 c.c..
int enqueue(Queue *q, char* comandos){
  int r = 0; // Resultado da execução, começamos por assumir que corre com sucesso.
  Tarefa *new;
  new = malloc(sizeof(Tarefa));
  if(new == NULL){
    r = -1; // Falhou a alocação de memória, retorna erro;
  }
  else{
    new->comandos = comandos;
    // my_printf2("new->comandos = %s\n", comandos); // DEBUG
    new->id = tarefa_id++;
    new->estado = A_EXECUTAR;
    new->prox = NULL;
    if(!isEmpty(q)){
      // Se a queue estiver vazia, q->fim == NULL, logo ocorre SEGFAULT.
      q->fim->prox = new;
    }
    q->fim = new;
    if(q->inicio == NULL){
      q->inicio = new;
    }
  }
  return r;
}

// Obtem o próximo elemento da Queue a ser tratado.
int dequeue(Queue *q, char** comandos){
  int r = 0;
  Tarefa *tmp;
  if(q->inicio == NULL){
    r = -1; // Não existem elementos na Queue, logo devolve erro;
  }else{
    // Retorna o "comandos" a ser usado.
    *comandos = q->inicio->comandos; // TODO: testar!
    // my_printf2("comandos = %s\n", *comandos); // DEBUG
    r = q->inicio->id;
    tmp = q->inicio;
    q->inicio = q->inicio->prox;
    if(q->inicio == NULL){
      q->fim = NULL;
    }
    free(tmp);
  }
  return r; // retorna o id da tarefa.
}

/** Abre o fifo do servidor para o cliente no modo de escrita */
int open_fifo_server_client(){
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, O_CREAT | O_TRUNC | O_WRONLY)) < 0){
    perror("fifo server client open");
    // não faz sentido terminar o servidor quando falha a abertura dos pipes,
    // mas temos de fazer alguma coisa quando falha (ex.: escrever para o termi-
    // nal a informar que falhou).
    //_exit(1);
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

int * pids_exec;
int pids_count_exec = 0;
int execution = 0;

// TODO: Ainda só foi copiada da "timeout_handler" e não foi adaptada correctamente.
void execution_handler(int signum){
  for(int i = 0; i < pids_count_exec; i++){
    if(pids_exec[i] > 0){
      kill(pids_exec[i], SIGKILL);
    }
  }
  execution = 1;
}

/** Este método é um passo intermédio executado antes da invocação da "executar".
* Tem como objectivo: 1) adicionar a tarefa à queue de forma a ser possível lis-
* tar todas as tarefas em execução e 2) controlar o tempo máximo de execução de
* cada tarefa.
*/
int setup_executar(char* comandos){
  int id = -1, result = -1, status = 0, pid = -1;

  // 1º - Adicionar tarefa à queue.
  if(isEmpty(queue_tarefas)){
    if(queue_tarefas == NULL){
      queue_tarefas = malloc(sizeof(Queue));
    }
    initQueue(queue_tarefas);
  }
  if(enqueue(queue_tarefas, comandos) < 0){
      return -1; // Erro ao adicionar tarefa à queue.
  }
  char** comandos_aux = malloc(sizeof(comandos));
  if((id = dequeue(queue_tarefas, comandos_aux) < 0)){
    return -1; // Erro ao remover tarefa da queue.
  }
  // my_printf2("comandos_aux = %s\n", *comandos_aux); // DEBUG

  // 2º - Chamar "executar" para executar o comando.
  if((pid = fork()) == 0){
    result = executar(*comandos_aux);
    if(result > 0){
      _exit(result);
    }else{
      _exit(-1);
    }
  }

  // 3º - Definir tempo máximo de execução.
  if(max_execucao >= 0){
    if(signal(SIGALRM, execution_handler) == SIG_ERR){
      perror("SIGALRM Failed for Execution Handler.");
      _exit(1);
    }
    alarm(max_execucao);
  }

  // 4º - esperar pelo fim da execução.
  while((pid = wait(&status)) > 0){
    if(WIFEXITED(status)){
      // TODO: o que faço aqui??
    }
  }

  // 5º - se terminou com sucesso, adicionar ao histórico, c.c. ainda não sei...
  if(result >= 0){
    my_printf2("result = %d\n", result);
    // write(historico_fd, )
  }
  return 0;
}

int * pids;
int pids_count = 0;
int timeout = 0;

void timeout_handler(int signum){
  for(int i = 0; i < pids_count; i++){
    if(pids[i] > 0){
      kill(pids[i], SIGKILL);
    }
  }
  timeout = 1;
}

/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos){
  char * coms[ARRAY_SIZE];
  int pid = -1, status = 0;
  parse_comandos(ARRAY_SIZE, comandos, coms);

  int sizeof_coms = sizeof_string_array(coms);
  if(sizeof_coms <= 0) return 1;

  pids = (int *) malloc(sizeof(int) * sizeof_coms);
  // my_printf2("sizeof_coms %d\n", sizeof_coms); // DEBUG
  // my_printf2("comandos = %s\n", *comandos); // DEBUG
  int n_pipes = 1;
  if(sizeof_coms == 1){ // Não existem pipes.
    // Partir o comando nos seus argumentos
    char* args[ARRAY_SIZE];
    tokenize(args, coms[0], ARRAY_SIZE);

    if(fork() == 0){
      // my_printf2("%s\n", coms[0]); // DEBUG
      // dup2(fifo_fd[1], 1);
      // close_fifo_server_client();
      execvp(args[0], args);
      exit(1);
    }else{
      wait(&status);
      close_fifo_server_client();
      return WEXITSTATUS(status);
    }
  }else if(sizeof_coms > 1){
    n_pipes = sizeof_coms - 1;
    int pipe_array[n_pipes][2];

    // Cada iteração representa 1 comando com os seus argumentos
    for(int i = 0; coms[i] != NULL; i++){
      // Partir o comando nos seus argumentos
      char* args[ARRAY_SIZE];
      // my_printf2("coms[%d] = %s\n", i, coms[i]); //DEBUG
      // my_printf2("coms = %s\n", i, *coms); //DEBUG
      // char * pointer = *coms[i];
      tokenize(args, coms[i], ARRAY_SIZE);
      // my_printf2("args[%d] = %s\n", i, args[i]); // DEBUG
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

        execvp(args[0], args);
        _exit(1); // execvp falhou.
      }
      // Processo pai fecha os pipes que os seus filhos já usaram.
      if(i > 0) close(pipe_array[i-1][0]);
      if(i < n_pipes){
        close(pipe_array[i][0]);
        close(pipe_array[i][1]);
      }
      // if(i == n_pipes){
      //   close_fifo_server_client();
      // }

      // TODO: check return value, não podem ser negativos.
      pids[i] = pid;
      // my_printf2("%s \n", coms[i]);
    }
  }

  // Definição do alarme max_inactividade se existir.
  if(max_inactividade > 0){
    my_printf("We're in.\n"); // DEBUG
    if(signal(SIGALRM, timeout_handler) == SIG_ERR){
      perror("SIGALRM Failed for Timeout Handler.");
      _exit(1);
    }
    alarm(max_inactividade);
  }

  while((pid = wait(&status)) > 0){
    if(WIFEXITED(status)){
      // TODO: o que faço aqui??
    }
  }

  if(timeout != 0){
    my_printf("TIMEOUT\n");
    return 2;
  }

  // TODO: verificar estado, etc.
  //while(!timeout && wait(&status) > 0){}
  for (int i = 0; i < sizeof_coms; i++) {
    kill(pids[i], SIGKILL);
    // Status check.
    pid = wait(&status);
    if(WIFEXITED(status)){ // Estes terminaram normalmente.
      printf("Executar %d finished.\n", pids[i]);
    }else{ // Estes ainda não tinham terminado.
      printf("Executar %d killed.\n", pids[i]);
    }
  }

  return 0;
}

/** Listar tarefas em execução (opção "-l" da linha de comando). */
int listar(){
  Tarefa *iterator = malloc(sizeof(Tarefa));
  if(!isEmpty(queue_tarefas)){
    my_printf("Listing...\n");
    for(iterator = queue_tarefas->inicio; iterator != NULL; iterator = iterator->prox){
      if(iterator->estado == A_EXECUTAR){
        my_printf2("#%d: %s\n", iterator->id, iterator->comandos);
      }
    }
  }else{
    char * message = "Não existem tarefas a executar.\n";
    write(fifo_fd[1], message, strlen(message));
  }

  return 0;
}

/** Terminar uma tarefa em execução (opção "-t n"). */
int terminar(int tarefa){

  Tarefa *iterator = malloc(sizeof(Tarefa));
  if(!isEmpty(queue_tarefas)){
    my_printf("Searching for tarefa to terminate...\n");
    for(iterator = queue_tarefas->inicio; iterator != NULL; iterator = iterator->prox){
      if(iterator->id == tarefa){
        // TODO: Terminar tarefa, mas como se não temos o pid aqui?
        //kill(pid_tarefa, SIGKILL);
        //break;
      }
    }
  }else{
    char * message = "Tarefa não encontrada.\n";
    write(fifo_fd[1], message, strlen(message));
  }

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
