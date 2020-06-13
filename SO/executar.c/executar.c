/** Executar uma tarefa (opção «-e "p1|p2...|pn"» da linha de comando). */
int executar(char* comandos){
  char * coms[ARRAY_SIZE];
  int pid = -1, status = 0, i = 0;
  char buf1[512];
  int n;

  timeout = 0; // Reset do timeout.
  parse_comandos(ARRAY_SIZE, comandos, coms);

  int sizeof_coms = sizeof_string_array(coms);
  pids_count = sizeof_coms;
  if(sizeof_coms <= 0) return 1;

  pids = (int *) malloc(sizeof(int) * sizeof_coms);
  int n_pipes = 1;
  if(sizeof_coms == 1){ // Não existem pipes.
    // Partir o comando nos seus argumentos
    char* args[ARRAY_SIZE];
    tokenize(args, coms[0], ARRAY_SIZE);

    if(fork() == 0){
      execvp(args[0], args);
      exit(1);
    }else{
      wait(&status);
      close_fifo_server_client();
      return WEXITSTATUS(status);
    }
  }else if(sizeof_coms > 1){
    n_pipes = sizeof_coms - 2 ;
    int pipe_array[n_pipes][2];
    int pipe_mon[2][2];
    if(pipe(pipe_mon[0]) < 0){
      perror("pipe_array");
      my_printf2("Falhou a criação do pipe_array[%d].\n", i);
      return 1;
    }
    if(pipe(pipe_mon[1]) < 0){
      perror("pipe_array");
      my_printf2("Falhou a criação do pipe_array[%d].\n", i);
      return 1;
    }
    // Cada iteração representa 1 comando com os seus argumentos
    for(i = 0; coms[i] != NULL; i++){

      // Partir o comando nos seus argumentos
      char* args[ARRAY_SIZE];
      tokenize(args, coms[i], ARRAY_SIZE);

      // Criar pipe anónimo
      if(i>0 && i <= n_pipes){
        if(pipe(pipe_array[i]) < 0){
          perror("pipe_array");
          my_printf2("Falhou a criação do pipe_array[%d].\n", i);
          return 1;
        }
      }

      // PROCESSO de monotorizaçao
      if(i==0){
        if((pid = fork()) == 0){
          if(max_inactividade >= 0){
            if(signal(SIGALRM, timeout_handler) == SIG_ERR){
              perror("SIGALRM Falhou para o Timeout Handler.");
              _exit(1);
            }
          }
          close(pipe_mon[0][1]);
          close(pipe_mon[1][0]);

          //ALARM
          alarm(max_inactividade);
          while((n=read(pipe_mon[0][0],buf1,512))>0){
            alarm(max_inactividade);
            write(pipe_mon[1][1],buf1,n);
          }
          _exit(1);
        }
        close(pipe_mon[1][1]);
        close(pipe_mon[0][0]);
      }
      //Processo filho
      if((pid = fork()) == 0){
        //1 processo
        if(i==0){
          close(pipe_mon[0][0]);
          dup2(pipe_mon[0][1],1);
          close(pipe_mon[0][1]);
          close(pipe_mon[1][0]);
          close(pipe_mon[1][1]);
        }
        //2 processo e ultimo
        if(i==1 && n_pipes == 0){
          dup2(pipe_mon[1][0],0);
          close(pipe_mon[1][0]);
          close(pipe_mon[0][0]);
          close(pipe_mon[1][1]);

        }
        //2 processo e nao ultimo
        if(i==1 && n_pipes > 0){
          dup2(pipe_mon[1][0],0);
          close(pipe_mon[1][0]);
          close(pipe_mon[0][0]);
          close(pipe_mon[1][1]);
          close(pipe_array[1][0]);
          dup2(pipe_array[1][1],1);
          close(pipe_array[1][1]);
        }
        //Processos do meio
        if(i>1 && i-1 < n_pipes){
          close(pipe_array[i][0]);
          dup2(pipe_array[i-1][0],0);
          close(pipe_array[i-1][0]);
          dup2(pipe_array[i][1],1);
          close(pipe_array[i][1]);
        }
        //Ultimo processo
        if(i>1 && i-1==n_pipes){
          dup2(pipe_array[i][0],0);
          close(pipe_array[i][0]);
        }
      }
      execvp(args[0], args);
      _exit(-1);

      if(i==0){
        close(pipe_mon[0][1]);
      }
      if(i==1){
        close(pipe_mon[0][0]);
        close(pipe_mon[1][1]);
        close(pipe_mon[1][0]);
      }
      if(i>1 && i-1 < n_pipes){
        close(pipe_array[i-1][0]);
        close(pipe_array[i][1]);
      }
      if(i>1 && i-1==n_pipes){
        close(pipe_array[i][0]);
      }
      // if(i == n_pipes){
      //   close_fifo_server_client();
      // }


      // TODO: check return value, não podem ser negativos.
      pids[i] = pid;
      // my_printf2("%s \n", coms[i]); // DEBUG
    }
  }

  while((pid = wait(&status)) > 0){
    if(WIFEXITED(status)){
      // TODO: o que faço aqui?
      // O filho deste processo já terminou, logo podemos desactivar o alarme.
      alarm(0);
    }
  }

  if(timeout){
    my_printf2("Timeout na função Executar. COMANDO: %s\n", coms[i]);
    return INACTIVIDADE;
  }

  // TODO: verificar estado, etc.
  while(!timeout && wait(&status) > 0){}
  for (i = 0; i < sizeof_coms; i++) {
    kill(pids[i], SIGKILL);
    // Status check.
    pid = wait(&status);
    if(WIFEXITED(status)){ // Estes processos terminaram normalmente.
      printf("Executar (%d) terminou normalmente.\n", pids[i]);
    }else{ // Estes processos ainda não tinham terminado.
      printf("Executar (%d) foi morta.\n", pids[i]);
    }
  }

  return 0;
}
