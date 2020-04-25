#include "main.h"

/* chamadas ao sistema: defs e decls essenciais */
// int execl(const char *path, const char *arg0, ..., NULL);
// int execlp(const char *file, const char *arg0, ..., NULL);
// int execv(const char *path, char *const argv[]);
// int execvp(const char *file, char *const argv[]);

/** Exercício 1:
      Implemente um programa que execute o comando ls -l. Note que no caso da
      execução ser bem sucedida, mais nenhuma outra instrução é executada do
      programa original.
*/
int ex1(){

  //execl*
  int result = execl("/bin/ls", "ls", "-l", NULL);
  result = execlp("ls", "ls", "-l", NULL);

  //execv*
  char * args[] = {
    "ls",
    "-l",
    NULL
  };

  result = execv("/bin/ls", args);
  result = execvp("ls", args);
  // não imprime porque ao usar exec apenas o comando é executado.
  printf("result: %d\n", result);

  if(result == 0){
    _exit(0);
  }
  _exit(1);
}

/** Exercício 2:
      Implemente um programa semelhante ao anterior que execute o mesmo comando
      mas agora no contexto de um processo filho.
*/
int ex2(){
  int result = -1, status;
  pid_t pid;

  if((pid = fork()) == 0){
    result = execl("/bin/ls", "ls", "-l", NULL);
    if(result < 0){
      perror("execl");
    }

    // esta linha não imprime quando o filho executa o execl com sucesso.
    printf("result: %d.\n", result);
    _exit(1);
  }

  wait(&status);

  // esta linha executa porque o pai não executa o execl.
  printf("pai terminou.\n");

  _exit(0);
}

/** Exercício 3:
*     Implemente um programa que imprima a lista de argumentos recebidos na sua
*     linha de comando.
*/
int ex3(int argc, const char* argv[]){
  int i;
  for(i = 0; i < argc; i++){
    printf("%s ", argv[i]);
  }
  printf("\n");

  _exit(0);
}
/** Exercício 4:
*     Implemente um programa que execute o programa da questão anterior com uma
*     qualquer lista de argumentos. Mantendo o nome do ficheiro que corresponde
*     ao programa executável, experimente alterar o primeiro elemento da lista
*     de argumentos (índice zero do argv).
*/
int ex4(int argc, const char* argv[]){
  //int result = execl("/home/jc/Uminho/SO/Guiões/main", "./main", "1", "echo", NULL);
  argv[0] = "ola";
  int result = execl("/home/jc/Uminho/SO/Guiões/main", *argv, NULL);
  if(result == 0){
    _exit(0);
  }
  _exit(1);
}

/** Exercício 5:
*     Implemente um programa que execute concorrentemente uma lista de executá-
*     veis específicados como argumentos da linha de comando. Considere os exe-
*     cutáveis sem quaisquer argumentos próprios. O programa deverá esperar pelo
*     fim da execução de todos processos por si criados.
*/
int ex5(int argc, const char* argv[]){
  int i, status, result;
  pid_t pid;

  for(i = 1; i < argc; i++){
    if((pid = fork()) == 0){
      result = execlp(argv[i], argv[i], NULL);
      if(result == 0){
        _exit(i);
      }else{
        _exit(-1);
      }
    }
  }

  for(i = 1; i < argc; i++){
    if(pid != 0){
      pid_t terminated_pid = wait(&status);
      if(WIFEXITED(status)){
        printf("[%d] Filho com Pid %d terminou.\n", WEXITSTATUS(status), terminated_pid);
      }
    }
  }

  _exit(0);
}

/** Exercício 6:
*     Implemente uma versão simplificada da função system(). Ao contrário da
*     função original, não tente suportar qualquer tipo de redireccionamento, ou
*     composição/encadeamento de programas executáveis. O único argumento deverá
*     ser uma string que especifica um programa executável e uma eventual lista
*     de argumentos. Procure que o comportamento e valor de retorno da sua fun-
*     ção sejam compatíveis com a original.
*/
/** Indicações do professor:
* Dividir a string. (strtok/strsep)
* resultado: args[] = ["ls", "-l", "dir/", NULL];
* fork()
*      se for processo filho:
*                      execvp(args[0], args);
*                      se execvp retornou, _exit(código de saida de erro).
*      se for processo pai:
*                     wait(&status);
*                     return WEXITSTATUS(status);
*/
int ex6(char * command){
  char * args[128];
  int i = 0, status, result, pid;

  char * token = strtok(command, " ");

  args[i++] = token;

  while(token != NULL){
    token = strtok(NULL, " ");
    args[i++] = token;
  }
  args[i++] = NULL;
  args[i++] = '\0';

  if((pid = fork()) == 0){
    result = execvp(args[0], args);
    if(result >= 0){
      _exit(result);
    }else{
      _exit(-1);
    }
  }else{
    wait(&status);
    return WEXITSTATUS(status);
  }
  return 0;
}
