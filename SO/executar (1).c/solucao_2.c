/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos, int id){
  char * coms[ARRAY_SIZE];
  int pid = -1, status = 0, i = 0;

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
      char *aux_tarefa_historico = "\n##### OUTPUT TAREFA ";
      char *id_as_string = integer_to_string(id);
      char *tarefa_historico = malloc(sizeof(aux_tarefa_historico)+sizeof(id_as_string)+sizeof("\n"));
      strcpy(tarefa_historico, aux_tarefa_historico);
      strcat(tarefa_historico, id_as_string);
      strcat(tarefa_historico, "\n");
      write(log_fd, tarefa_historico, strlen(tarefa_historico));
      dup2(log_fd, 1);
      close(log_fd);
      execvp(args[0], args);
      exit(ERRO);
    }else{
      close(log_fd);
      return WEXITSTATUS(status); // TODO: rever isto
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
        return ERRO;
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

        if(i == n_pipes){ // copia o fifo_fd no último comando para o STDOUT
          dup2(log_fd, 1);
          close(log_fd);
        }

        execvp(args[0], args);
        _exit(ERRO); // apenas executa esta linha se execvp falhou.
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

        // TODO: check return value, não podem ser negativos.
        pids[i] = pid;

        // Definição do alarme max_inactividade (se existir).
        if(max_inactividade >= 0){
          if(signal(SIGALRM, timeout_handler) == SIG_ERR){
            perror("SIGALRM Falhou para o Timeout Handler.");
            _exit(ERRO);
          }
          my_printf2("ALARME CONFIGURADO: %d.\n", max_inactividade);
          alarm(max_inactividade);
        }

        if(timeout){
          my_printf2("Timeout na função Executar.\n");
          return INACTIVIDADE;
          break;
        }

        pid_t terminated_pid = wait(&status);
        if(WIFEXITED(status)){
          my_printf2("O processo %d terminou normalmente.\n", terminated_pid);
        }else{
          my_printf2("O processo %d foi morto.\n", terminated_pid);
        }
      }
    }
  }

  my_printf("Executar terminou com sucesso.\n");
  return 0;
}
