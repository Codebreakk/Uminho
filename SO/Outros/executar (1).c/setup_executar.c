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

int * pid_exec;
int execution = 0;

/** Handler do SIGALRM para o tempo máximo de execução. */
void execution_handler(int signum){
    if(*pid_exec > 0){
      kill(*pid_exec, SIGKILL);
      my_printf2("[EXECUTION HANLDER] Matou o processo: %d.\n", *pid_exec);
    }
  execution = 1;
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

  // 1º - COLOCAR TAREFA EM EXECUÇÃO
  if((pid = fork()) == 0){
    result = executar(comandos, new_id);
    if(result > 0){
      _exit(result);
    }else{
      _exit(ERRO);
    }
  }else{ // isto não precisa estar num else, visto que o filho faz _exit antes.
    // 2º - ADICIONAR TAREFA À QUEUE
    // o pai passa para uma var global o pid do filho, para o handler matar o processo
    pid_exec = &pid;

    // Definir tempo máximo de execução.
    if(max_execucao >= 0){
      if(signal(SIGALRM, execution_handler) == SIG_ERR){
        perror("SIGALRM Falhou para o Execution Handler.");
        _exit(ERRO);
      }
      alarm(max_execucao);
    }

    char *aux = malloc(sizeof(comandos));
    strcpy(aux, comandos);
    if(enqueue(queue_tarefas, aux, new_id, pid) < 0){
        return ERRO;
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

    // if((id = dequeue(queue_tarefas, new_id) < 0)){
    //   return ERRO; // Erro ao remover tarefa da queue.
    // }
  }

  if(execution){ // tempo de execução foi excedido.
    my_printf("Tempo de execução foi excedido.\n");
    return EXECUCAO;
  }

  return 0;
}

int * pids;
int pids_count = 0;
int timeout = 0;

void timeout_handler(int signum){
  my_printf("[TIMEOUT HANDLER] ");
  for(int i = 0; i < pids_count; i++){
    my_printf2("pid = %d;  ", pids[i]);
    if(pids[i] > 0){
      kill(pids[i], SIGKILL);
    }
  }
  my_printf("\n");
  timeout = 1;
}
