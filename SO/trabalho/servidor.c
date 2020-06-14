#include "servidor.h"

int max_inactividade = -1;
int max_execucao = -1;
int tarefa_id = 0;

// Inicializa a queue.
void initQueue(Queue *q){
  q->inicio = q->fim = NULL;
}

// Verifica se a queue está vazia.
int isEmpty(Queue *q){
  return (q->inicio == NULL);
}

// Adiciona uma nova tarefa à queue. Retorna 0 se correr com sucesso, -1 c.c..
int enqueue(Queue *q, char* comandos, int id, int pid){
  int r = 0; // Resultado da execução, começamos por assumir que corre com sucesso.
  Tarefa *new = malloc(sizeof(Tarefa));
  if(new == NULL){
    r = -1; // Falhou a alocação de memória, retorna erro;
  }
  else{
    new->id = id;
    new->pid = pid;
    new->comandos = comandos;
    new->estado = A_EXECUTAR;
    new->prox = NULL;
    if(!isEmpty(q)){ // LISTA NÃO VAZIA
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

// Remove o elemento com o id indicado da Queue.
int dequeue(Queue *q, int id){
  int r = 0;
  Tarefa *tmp;
  if(q->inicio == NULL){
    r = ERRO; // Não existem elementos na Queue, logo devolve erro;
  }else{
    if(q->inicio->id == id){
      q->inicio = q->inicio->prox;
    }else{
      tmp = q->inicio;
      while(tmp->prox != NULL){
        if(tmp->prox->id == id){
          tmp->prox = tmp->prox->prox;
        }
        tmp = tmp->prox;
      }
      free(tmp);
    }
  }
  return r; // retorna 0 em caso de sucesso, -1 caso contrário.
}

// Remove o elemento com o pid indicado da Queue.
int dequeue_by_pid(Queue *q, int pid, int return_val){
  int id = 0;
  Tarefa *tmp = malloc(sizeof(q->inicio));
  char *coms_string;
  if(q->inicio == NULL){
    id = ERRO; // Não existem elementos na Queue, logo devolve erro;
  }else{
    if(q->inicio->pid == pid){
      id = q->inicio->id;
      coms_string = malloc(sizeof(q->inicio->comandos));
      strcpy(coms_string, q->inicio->comandos);
      q->inicio = q->inicio->prox;
    }else{
      tmp = q->inicio;
      while(tmp->prox != NULL){
        if(tmp->prox->pid == pid){
          id = tmp->prox->id;
          coms_string = malloc(sizeof(tmp->prox->comandos));
          strcpy(coms_string, tmp->prox->comandos);
          tmp->prox = tmp->prox->prox;
        }
        tmp = tmp->prox;
      }
      free(tmp);
    }
  }

  if(return_val == INACTIVIDADE){
    write_to_historico(id, coms_string, INACTIVIDADE);
  }else if (return_val == CONCLUIDA){
    write_to_historico(id, coms_string, CONCLUIDA);
  }else if(return_val == ERRO){
    write_to_historico(id, coms_string, ERRO);
  }else if(return_val == EXECUCAO){
    write_to_historico(id, coms_string, EXECUCAO);
  }

  return id; // retorna o id do processo removido.
}

/** Abre o fifo do servidor para o cliente no modo de escrita */
int open_fifo_server_client(){
  if((fifo_fd[1] = open(FIFO_SERVER_CLIENT, O_CREAT | O_TRUNC | O_WRONLY)) < 0){
    perror("Erro ao abrir o FIFO servidor cliente.");
    return -1;
  }
  my_printf("FIFO servidor cliente foi aberto com sucesso.\n");

  return fifo_fd[1];
}

/** Fecha o fifo do servidor para o cliente */
int close_fifo_server_client(){
  close(fifo_fd[1]);
  my_printf("FIFO servidor cliente fechado.\n");

  return 0;
}

/** Abre o fifo do cliente para o servidor no modo de leitura */
int open_fifo_client_server(){
  if((fifo_fd[0] = open(FIFO_CLIENT_SERVER, O_RDONLY)) < 0){
    perror("Erro ao abrir o FIFO cliente servidor.");
    return -1;
  }
  my_printf("FIFO cliente servidor foi aberto com sucesso.\n");

  return fifo_fd[0];
}

/** Fecha o fifo do cliente para o servidor */
int close_fifo_client_server(){
  close(fifo_fd[0]);
  my_printf("FIFO cliente servidor fechado.\n");

  return 0;
}

/** Função que escreve para o histórico o resultado final da tarefa */
int write_to_historico(int id, char *coms_string, int estado){
  char *id_as_string = integer_to_string(id);
  int tamanho_estado = 0;
  char *msg_estado;

  if(estado == CONCLUIDA){
    tamanho_estado = sizeof(MSG_CONCLUIDA);
    msg_estado = malloc(tamanho_estado);
    strcpy(msg_estado, MSG_CONCLUIDA);
  }else if(estado == INACTIVIDADE){
    tamanho_estado = sizeof(MSG_MAX_INACTIVIDADE);
    msg_estado = malloc(tamanho_estado);
    strcpy(msg_estado, MSG_MAX_INACTIVIDADE);
  }else if(estado == EXECUCAO){
    tamanho_estado = sizeof(MSG_MAX_EXECUCAO);
    msg_estado = malloc(tamanho_estado);
    strcpy(msg_estado, MSG_MAX_EXECUCAO);
  }else if(estado == TERMINADA){
    tamanho_estado = sizeof(MSG_TERMINADA);
    msg_estado = malloc(tamanho_estado);
    strcpy(msg_estado, MSG_TERMINADA);
  }else if(estado == ERRO){
    tamanho_estado = sizeof(MSG_ERRO);
    msg_estado = malloc(tamanho_estado);
    strcpy(msg_estado, MSG_ERRO);
  }

  int tamanho = sizeof(coms_string)+sizeof(id_as_string)+tamanho_estado+sizeof("#\n");
  char *historico_msg = malloc(tamanho);
  strcpy(historico_msg, "#");
  strcat(historico_msg, id_as_string);
  strcat(historico_msg, msg_estado);
  strcat(historico_msg, coms_string);
  strcat(historico_msg, "\n");
  write(historico_fd, historico_msg, strlen(historico_msg));

  return 0;
}

int parse_comandos(int array_size, char* buf, char * args[]){
  int n = 0;

  args[n] = strtok(buf, "|");
  while(args[n] && n < array_size - 1){
    int len = strlen(args[0]);
    // remove o '\n' no final de algumas strings para evitar falhar comparações com o "strcmp".
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

int *pid_exec;
int execution = 0;

/** Handler do SIGALRM para o tempo máximo de execução. */
void execution_handler(int signum){
  // Se pid < 0, não podemos fazer kill.
  if(*pid_exec > 0){
    my_printf2("[EXECUTION HANLDER] terminou o processo: %d.\n", *pid_exec);
    kill(*pid_exec, SIGKILL);
  }
  execution = 1;
}

/** Este handler trata dos filhos criados pelo servidor. */
void sigchild_handler(int signum){
  int child_status;
  pid_t child_pid = wait(&child_status);

  if(WIFSIGNALED(child_status)){ // Se o processo filho terminou por signal:
    if(WTERMSIG(child_status) == SIGKILL){ // O processo filho foi morto por SIGKILL
      dequeue_by_pid(queue_tarefas, child_pid, EXECUCAO);
    }
  }else if(WIFEXITED(child_status)){// Se o processo terminou:
    int return_val = WEXITSTATUS(child_status); // valor de retorno do filho.

    if(child_pid >= 0){
      dequeue_by_pid(queue_tarefas, child_pid, return_val);
    }
  }
}

/** Este método é um passo intermédio executado antes da invocação da "executar".
* Tem como objectivo: 1) adicionar a tarefa à queue de forma a ser possível lis-
* tar todas as tarefas em execução e 2) controlar o tempo máximo de execução de
* cada tarefa.
*/
int setup_executar(char* comandos){
  int result = -1, pid = 0, new_id = ++tarefa_id;
  execution = 0; // Reset da flag que indica timeout da executar.

  // Safety-check para garantir que a queue_tarefas existe e foi inicializada.
  if(isEmpty(queue_tarefas)){
    if(queue_tarefas == NULL){
      queue_tarefas = malloc(sizeof(Queue));
    }
    initQueue(queue_tarefas);
  }

  if(signal(SIGCHLD, sigchild_handler) == SIG_ERR){
    perror("SIGCHLD falhou para o SIGCHLD Handler.");
    return (ERRO);
  }

  // 1º - COLOCAR TAREFA EM EXECUÇÃO
  if((pid = fork()) == 0){
    pid_t child_pid = getpid();
    pid_exec = &child_pid;

    // Definir tempo máximo de execução.
    if(max_execucao >= 0){
      if(signal(SIGALRM, execution_handler) == SIG_ERR){
        perror("SIGALRM falhou para o Execution Handler.");
        _exit(ERRO);
      }
      alarm(max_execucao);
    }

    result = executar(comandos, new_id);

    if(execution){ // tempo de execução foi excedido.
      my_printf("Tempo de execução foi excedido.\n");
      _exit(EXECUCAO);
    }else{
      _exit(result);
    }
  }else{ // isto não precisa estar num else, visto que o filho faz _exit antes.
    // 2º - ADICIONAR TAREFA À QUEUE
    char *aux = malloc(sizeof(comandos));
    strcpy(aux, comandos);
    if(enqueue(queue_tarefas, aux, new_id, pid) < 0){
        return ERRO;
    }

  }
  // 3º - INFORMAR O CLIENTE QUE A TAREFA FOI ADICIONADA
  open_fifo_server_client();
  char *msg_aux = "nova tarefa #";
  char *id_as_string = integer_to_string(new_id);
  char *nova_tarefa_msg = malloc(sizeof(msg_aux) + sizeof(id_as_string) + sizeof("\n"));
  strcpy(nova_tarefa_msg, msg_aux);
  strcat(nova_tarefa_msg, id_as_string);
  strcat(nova_tarefa_msg, "\n");
  write(fifo_fd[1], nova_tarefa_msg, strlen(nova_tarefa_msg));
  close_fifo_server_client();

  return 0;
}

int * pids;
int pids_count = 0;
int timeout = 0;

void timeout_handler(int signum){
  my_printf("[TIMEOUT HANDLER] terminou os seguintes processos: ");
  for(int i = 0; i < pids_count; i++){
    my_printf2("pid = %d;  ", pids[i]);
    if(pids[i] > 0){
      kill(pids[i], SIGKILL);
    }
  }
  my_printf("\n");
  timeout = 1;
}

/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos, int id){
  char * coms[ARRAY_SIZE];
  int pid = -1, i = 0, status;
  char *id_as_string = integer_to_string(id);
  char *coms_string = malloc(sizeof(comandos));
  strcpy(coms_string, comandos);

  timeout = 0; // Reset do timeout.
  // Divisão da tarefa nos seus vários comandos.
  parse_comandos(ARRAY_SIZE, comandos, coms);

  int n_coms = sizeof_string_array(coms);
  pids_count = n_coms - 1;
  if(n_coms <= 0) return ERRO; // Número inválido de comandos.

  // Lista de pids dos comandos.
  pids = (int *) malloc(sizeof(int) * n_coms);
  int n_pipes = n_coms - 1;
  if(n_pipes == 0){ // Não existem pipes no comando.
    // Partir o comando nos seus argumentos.
    char* args[ARRAY_SIZE];
    tokenize(args, coms[0], ARRAY_SIZE);

    // Executar comando num processo filho.
    if(fork() == 0){
      // Escrita para o ficheiro de log do output
      char *aux_tarefa_log = "\n##### OUTPUT TAREFA ";
      char *tarefa_log = malloc(sizeof(aux_tarefa_log)+sizeof(id_as_string)+sizeof("\n"));
      strcpy(tarefa_log, aux_tarefa_log);
      strcat(tarefa_log, id_as_string);
      strcat(tarefa_log, "\n");
      write(log_fd, tarefa_log, strlen(tarefa_log));

      dup2(log_fd, 1);
      close(log_fd);
      if(execvp(args[0], args) < 0){
        perror("ERRO execvp ");
        _exit(ERRO);
      }
    }else{
      close(log_fd);
      wait(&status);
      return (WEXITSTATUS(status));
    }
  }else if(n_pipes >= 1){
    // Lista de pipes para a tarefa.
    int pipe_array[n_pipes][2];

    // Cada iteração representa 1 comando com os seus argumentos.
    for(i = 0; coms[i] != NULL; i++){
      // Partir o comando nos seus argumentos
      char* args[ARRAY_SIZE];
      tokenize(args, coms[i], ARRAY_SIZE);

      // Criar pipe anónimo
      if(pipe(pipe_array[i]) < 0){
        perror("pipe_array");
        my_printf2("Falhou a criação do pipe_array[%d].\n", i);
        return (ERRO);
      }
      if((pid = fork()) == 0){ // PROCESSO FILHO
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

        // Definição do alarme max_inactividade (se existir).
        if(max_inactividade >= 0){
          if(signal(SIGALRM, timeout_handler) == SIG_ERR){
            perror("SIGALRM falhou para o Timeout Handler.");
            _exit(ERRO);
          }
          alarm(max_inactividade);
        }

        if(i == n_pipes){ // copia o fifo_fd no último comando para o STDOUT
          dup2(log_fd, 1);
          close(log_fd);
        }

        if(timeout == 1){
          my_printf2("Executar terminou por timeout.\n");
          write_to_historico(id, coms_string, INACTIVIDADE);
          _exit(INACTIVIDADE);
        }

        if(execvp(args[0], args) < 0){
          perror("ERRO execvp ");
          _exit(ERRO); // apenas executa esta linha se execvp falhou.
        }
      }else{
        // PROCESSO PAI
        // fecha os pipes que os seus filhos já usaram.
        if(i > 0) close(pipe_array[i-1][0]);
        if(i < n_pipes){
          close(pipe_array[i][0]);
          close(pipe_array[i][1]);
        }
        if(i == n_pipes){
          char *aux_tarefa_historico = "\n##### OUTPUT TAREFA ";
          char *id_as_string = integer_to_string(id);
          char *tarefa_historico = malloc(sizeof(aux_tarefa_historico)+sizeof(id_as_string)+sizeof("\n"));
          strcpy(tarefa_historico, aux_tarefa_historico);
          strcat(tarefa_historico, id_as_string);
          strcat(tarefa_historico, "\n");
          write(log_fd, tarefa_historico, strlen(tarefa_historico));
          close(log_fd);
        }
        pids[i] = pid;

        wait(&status);
        if(WIFEXITED(status)){
          my_printf2("O processo %d terminou normalmente.\n", pid);
        }else{
          if(WIFSIGNALED(status)){
            if(WTERMSIG(status) == SIGALRM){
              // O processo filho for morto por SIGALRM.
              _exit(INACTIVIDADE);
            }
          }
          my_printf2("O processo %d foi morto.\n", pid);
        }
      }
    }
  }

  alarm(0);
  my_printf("Executar terminou com sucesso.\n");
  write_to_historico(id, coms_string, CONCLUIDA);
  _exit(CONCLUIDA);
}

/** Listar tarefas em execução (opção "-l" da linha de comando). */
int listar(){
  Tarefa *iterator;
  if(!isEmpty(queue_tarefas)){
    my_printf("A listar...\n");
    iterator = queue_tarefas->inicio;

    while(iterator != NULL){
      if(iterator->estado == A_EXECUTAR){
        char *string_id = integer_to_string(iterator->id);
        write(fifo_fd[1], "#", strlen("#"));
        write(fifo_fd[1], string_id, strlen(string_id));
        write(fifo_fd[1], ": ", strlen(": "));
        write(fifo_fd[1], iterator->comandos, strlen(iterator->comandos));
        write(fifo_fd[1], "\n", strlen("\n"));
      }
      iterator = iterator->prox;
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
    my_printf("A procurar a Tarefa a terminar...\n");
    for(iterator = queue_tarefas->inicio; iterator != NULL; iterator = iterator->prox){
      if(iterator->id == tarefa && iterator->pid > 0){
        if(iterator->pid > 0){
          kill(iterator->pid, SIGKILL);
          write_to_historico(tarefa, iterator->comandos, TERMINADA);
          dequeue(queue_tarefas, tarefa);
          char *message = "Tarefa terminada.\n";
          write(fifo_fd[1], message, strlen(message));
          break;
        }else{
          my_printf2("Terminar: pid inválido %d\n", iterator->pid);
        }
      }
    }
  }else{
    char *message = "Tarefa não encontrada.\n";
    write(fifo_fd[1], message, strlen(message));
  }

  return 0;
}

/** Listar registo histórico de tarefas terminadas (opção "-r"). */
int historico(){
  char buf[BUF_SIZE];

  // Abrir ficheiro com a informação a imprimir.
  int historico_file = open(HISTORICO, O_RDONLY);
  if(historico_file < 0){
    perror("Erro ao abrir o ficheiro historico");
    return ERRO;
  }

  // ler do ficheiro para o buf e escrever para o fifo_client_server.
  ssize_t bytes = 0;
  while((bytes = read(historico_file, buf, BUF_SIZE)) != 0){
    write(fifo_fd[1], buf, bytes);
  }

  close(historico_file);
  return 0;
}

/** Apresentar ajuda à sua utilização (opção "-h"). */
int ajuda(){
  char buf[BUF_SIZE];

  // Abrir ficheiro com a informação a imprimir.
  int file_help = open(HELP, O_RDONLY);
  if(file_help < 0){
    perror("Erro ao abrir o ficheiro help");
    return ERRO;
  }

  // ler do ficheiro para o buf e escrever para o fifo_client_server.
  ssize_t bytes = 0;
  while((bytes = read(file_help, buf, BUF_SIZE)) != 0){
    write(fifo_fd[1], buf, bytes);
  }

  // Fechar o ficheiro de ajuda.
  close(file_help);
  return 0;
}
