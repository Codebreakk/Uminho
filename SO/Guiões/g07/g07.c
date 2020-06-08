#include "main.h"

/** Chamadas ao sistema
* sighandler_t signal(int signum, sighandler_t handler);
* int kill(pid_t pid, int sig);
* unsigned int alarm(unsigned int seconds);
* int pause(void);
*/

/** Exercício 1
Usando SIGINT, SIGQUIT e SIGALRM, escreva um programa que vá contando o tempo em
segundos desde que começou. Se, entretanto, o utilizador carregar em Ctrl+C, o
programa deverá imprimir o tempo passado. Se carregar em Ctrl+\ o programa deve-
rá indicar quantas vezes o utilizador carregou em Ctrl+C e terminar de seguida.
*/
int ctrl_c_counter = 0;
int seconds = 0;

void sigint_handler(int signum){
  printf("sigint received: code %d\n", signum);
  printf("%d\n", seconds);
  ctrl_c_counter++;
}

void sigquit_handler(int signum){
  printf("sigquit received: code %d\n", signum);
  printf("%d\n", ctrl_c_counter);
  exit(0);
}

void sigalrm_handler(int signum){
  //printf("sigalrm received: code %d\n", signum);
  seconds++;
  alarm(1);
}

int ex1(){
  alarm(1);
  if(signal(SIGINT, sigint_handler) == SIG_ERR){
    perror("SIGINT Failed.");
    exit(1);
  }

  if(signal(SIGQUIT, sigquit_handler) == SIG_ERR){
    perror("SIGQUIT Failed.");
    exit(1);
  }

  if(signal(SIGALRM, sigalrm_handler) == SIG_ERR){
    perror("SIGALRM Failed.");
    exit(1);
  }

  while(1){
    pause();
    printf("interrupted\n");
  }

  return 0;
}

int ex2(int argc, char * argv[]){

  if(argc < 3){
    my_printf("Usage: multigrep <palavra> <file>...\n");
    return 1;
  }

  // nº de ficheiros;
  int files_count =  argc - 2;
  char ** files = argv + 2; // files[0] = argv[2]
  int pids[files_count];

  //criar múltiplos processos grep por ficheiro.
  for(int i = 0; i < files_count; i++){
    int pid;
    if((pid = fork()) == 0){
      //printf("grep %d with pid %d for file %s\n",i,getpid(),files[i]);

      if(execlp("grep", "grep", argv[1], files[i], NULL) < 0){
        perror("grep exec");
        _exit(1); //_exit(10);
      }
    }

    // TODO: check return value.
    pids[i] = pid;
  }

  // Programas grep a executar...
  int status = 0;
  int found = 0;
  int pid = -1;

  // Enquanto não for encontrada a palavra e existirem processos para esperar...
  while(!found && (pid = wait(&status)) > 0){
    if(WIFEXITED(status)){
      switch (WEXITSTATUS(status)){
        case 0: // correu com sucesso.
          printf("grep %d found the word.\n", pid);
          found = 1;
          break;
        case 1: // ocorreu sem sucesso.
          printf("grep %d did not find the word.\n", pid);
          break;
        default: // ocorreu algum erro.
          my_printf("something went wrong...\n");
      }
    }
  }

  // Se sair pelo que não encontrou, então também diz que não encontrou.
  if(!found){
    return 1;
  }

  // Terminar todos os processos restantes...
  for(int i = 0; i < files_count; i++){
    kill(pids[i], SIGKILL);

    if(waitpid(pids[i], &status, 0) > 0){
      if(WIFEXITED(status)){
        printf("grep %d finished.\n", pids[i]);
      }else{
        printf("grep %d killed.\n", pids[i]);
      }
    }
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

int ex3(int argc, char * argv[]){

  if(argc < 3){
    my_printf("Usage: multigrep <palavra> <file>...\n");
    return 1;
  }

  // nº de ficheiros;
  int files_count =  argc - 2;
  pids_count = files_count;
  char ** files = argv + 2; // files[0] = argv[2]
  pids = (int *) malloc(sizeof(int)*pids_count);

  //criar múltiplos processos grep por ficheiro.
  for(int i = 0; i < files_count; i++){
    int pid;
    if((pid = fork()) == 0){
      //printf("grep %d with pid %d for file %s\n",i,getpid(),files[i]);

      if(execlp("grep", "grep", argv[1], files[i], NULL) < 0){
        perror("grep exec");
        _exit(1);
      }
    }

    // check return value
    pids[i] = pid;
  }

  // Programas grep a executar...
  if(signal(SIGALRM, timeout_handler) == SIG_ERR){
    perror("SIGALRM Failed.");
    return 10;
  }
  alarm(10);

  int status = 0;
  int found = 0;
  int pid = -1;
  // Enquanto não for encontrado o valor, executamos esta espera.
  while(!found && (pid = wait(&status)) > 0){
    if(WIFEXITED(status)){
      switch (WEXITSTATUS(status)){
        case 0:
          printf("grep %d found the word.\n", pid);
          found = 1;
          alarm(0);
          break;
        case 1:
          printf("grep %d did not find the word.\n", pid);
          break;
        default:
          my_printf("something went wrong...\n");
      }
    }
  }

  if(timeout){
    return 2;
  }

  if(!found){
    return 1;
  }

  // Terminar todos os processos restantes... Este ciclo é executado apenas após
  // o valor ter sido encontrado.
  for(int i = 0; i < files_count; i++){
    kill(pids[i], SIGKILL);

    if(waitpid(pids[i], &status, 0) > 0){
      if(WIFEXITED(status)){ // Estes terminaram normalmente.
        printf("grep %d finished.\n", pids[i]);
      }else{ // Estes ainda não tinham terminado.
        printf("grep %d killed.\n", pids[i]);
      }
    }
  }

  return 0;
}
