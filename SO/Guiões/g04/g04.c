#include "main.h"

// int dup(int fd);
// int dup2(int fd1, int fd2);

/** exercício 1:
*   Escreva um programa que redireccione o descritor associado ao seu standard
*   input para o ficheiro /etc/passwd, e o standard output e error respectiva-
*   mente para saida.txt e erros.txt.
*/
int ex1(){
  // Verificar se open() executou correctamente.
  int input_fd = open("/etc/passwd", O_RDONLY);
  int output_fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  int error_fd = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // fd -> recurso
  // 0 (stdin) -> teclado
  // 1 (stdout) -> ecra
  // 2 (stderror) -> ecra
  // input_fd -> /etc/passwd
  // output_fd -> saida.txt
  // error_fd ->erros.txt

  // redireccionamento de ficheiros
  dup2(input_fd, 0);
  dup2(output_fd, 1);
  dup2(error_fd, 2);

  // fd -> recurso
  // input_fd, 0 -> /etc/passwd
  // output_fd, 1 -> saida.txt
  // error_fd, 2 ->erros.txt

  close(input_fd);
  close(output_fd);
  close(error_fd);

  // fd -> recurso
  // 0 -> /etc/passwd
  // 1 -> saida.txt
  // 2 ->erros.txt

  _exit(0);
}

/** exercício 2:
*   Modifique o programa anterior de modo a que, depois de realizar os redirec-
*   cionamentos, seja criado um novo processo que realize operações de leitura e
*   escrita. Observe o conteúdo dos ficheiros. Repare que o processo filho "nas-
*   ce" com as mesmas associações de descritores de ficheiros do processo pai.
*/
int ex2(){
  // Verificar se open() executou correctamente.
  int input_fd = open("/etc/passwd", O_RDONLY);
  int output_fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  int error_fd = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // fd -> recurso
  // 0 (stdin) -> teclado
  // 1 (stdout) -> ecra
  // 2 (stderror) -> ecra
  // input_fd -> /etc/passwd
  // output_fd -> saida.txt
  // error_fd ->erros.txt

  // redireccionamento de ficheiros
  dup2(input_fd, 0);
  dup2(output_fd, 1);
  dup2(error_fd, 2);

  // fd -> recurso
  // input_fd, 0 -> /etc/passwd
  // output_fd, 1 -> saida.txt
  // error_fd, 2 ->erros.txt

  close(input_fd);
  close(output_fd);
  close(error_fd);

  // fd -> recurso
  // 0 -> /etc/passwd
  // 1 -> saida.txt
  // 2 ->erros.txt

  int pid = -1;
  int status = -1;
  if((pid = fork()) == 0){
    char buffer[10];
    int bytes = 0;

    while((bytes = read(0, &buffer, 10)) > 0){
      write(1, &buffer, bytes);
      write(2, &buffer, bytes);
    }
    _exit(0);
  }else{
    wait(&status);
  }

  _exit(0);
}

/** exercício 3:
*   Modifique novamente o programa inicial de modo a que seja executado o coman-
*   do wc, sem argumentos, depois do redireccionamento dos descritores de entra-
*   da e saída. Note que, mais uma vez, as associações - e redireccionamentos -
*   de descritores de ficheiros são preservados pela primitiva exec().
*/
int ex3(){
  // Verificar se open() executou correctamente.
  int input_fd = open("/etc/passwd", O_RDONLY);
  int output_fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  int error_fd = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // fd -> recurso
  // 0 (stdin) -> teclado
  // 1 (stdout) -> ecra
  // 2 (stderror) -> ecra
  // input_fd -> /etc/passwd
  // output_fd -> saida.txt
  // error_fd ->erros.txt

  // redireccionamento de ficheiros
  dup2(input_fd, 0);
  dup2(output_fd, 1);
  dup2(error_fd, 2);

  // fd -> recurso
  // input_fd, 0 -> /etc/passwd
  // output_fd, 1 -> saida.txt
  // error_fd, 2 ->erros.txt

  close(input_fd);
  close(output_fd);
  close(error_fd);

  // fd -> recurso
  // 0 -> /etc/passwd
  // 1 -> saida.txt
  // 2 ->erros.txt

  execlp("wc", "wc", NULL);

  _exit(0);
}

/** Exercício 4:
*   Escreva um programa redir que permita executar um comando, opcionalmente re-
*   direccionando a entrada e/ou saída. O programa poderá ser invocado, com:
*        redir [-i fich_entrada] [-o fich_saida] comando arg1 arg2 ...
*/
int ex4(int argc, const char* argv[]){

  int input_fd = 0;
  int output_fd = 1;

  if(argc < 2){
    _exit(0);
  }

  if(strcmp(argv[1], "-i") == 0){
    input_fd = open(argv[2], O_RDONLY);
    dup2(input_fd, 0);
  }

  if(strcmp(argv[3], "-o") == 0){
    output_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(output_fd, 1);
  }

  if(strcmp(argv[1], "-o") == 0){
    output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(output_fd, 1);
  }

  if(strcmp(argv[3], "-i") == 0){
    input_fd = open(argv[4], O_RDONLY);
    dup2(input_fd, 0);
  }

  close(input_fd);
  close(output_fd);

  char* args[argc - 1];
  memcpy(args, argv + 1, sizeof(char) * argc - 1);
  execvp(argv[1], args);

  _exit(0);
}
