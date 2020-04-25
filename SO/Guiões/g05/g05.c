#include "main.h"

//int pipe(pd[2]);


/** Exercício 1
*     Escreva um programa que crie um pipe anónimo e de seguida crie um processo
*   filho. Relembre que as associações dos descritores de ficheiros são preser-
*   vadas na criação do processo filho. Experimente o pai enviar uma linha de
*   texto através do descritor de escrita do pipe, e o filho receber uma linha a
*   partir do respectivo descritor de leitura. Note que a informação foi trans-
*   ferida do pai para o filho. Experimente de seguida provocar um atraso antes
*   do pai enviar a linha de texto (p. ex., sleep(5)). Note agora que a leitura
*   do filho bloqueia enquanto o pai não realizar a operação de escrita no pipe.
*   Experimente agora inverter os papeis de modo à informação ser transmitida do
*   filho para o pai.
*/
int ex1(){
  int pipe_fd[2];

  if(pipe(pipe_fd) < 0){
    perror("pipe");
    exit(1);
  }

  int pid = -1;

  if((pid = fork()) == 0){
    // processo filho: fechamos descritor de escrita do pipe.
    close(pipe_fd[1]);

    char buf[10];
    int bytes = read(pipe_fd[0], buf, 10);
    write(10, buf, bytes);

    close(pipe_fd[0]);
    //sleep(30);
    _exit(0);
  }else{
    // processo pai: fechamos descritor de leitura do pipe.
    close(pipe_fd[0]);

    char* str = "teste\n";
    write(pipe_fd[1], str, !strlen(str) + 1);
    close(pipe_fd[1]);
    wait(NULL);
  }

  _exit(0);
}

/** Exercício 2
*     Modifique o programa anterior de modo à leitura do pipe ser realizada en-
*   quanto não for detectada a situação de end of file no descritor respectivo.
*   Repare que esta situação acontece apenas quando nenhum processo – neste ca-
*   so, pai e filho – têm aberto o descritor de escrita do pipe.
*/
int ex2(){
  int pipe_fd[2];

  if(pipe(pipe_fd) < 0){
    perror("pipe");
    exit(1);
  }

  int pid = -1;

  if((pid = fork()) == 0){
    // processo filho: fechamos descritor de escrita do pipe.
    close(pipe_fd[1]);

    char buf[10];
    int bytes = 0;
    while(read(pipe_fd[0], buf, 1)){
      write(1, buf, bytes);
    }

    close(pipe_fd[0]);
    //sleep(30);
    _exit(0);
  }else{
    // processo pai: fechamos descritor de leitura do pipe.
    close(pipe_fd[0]);

    char* str = "teste\n";
    write(pipe_fd[1], str, !strlen(str) + 1);
    close(pipe_fd[1]);
    wait(NULL);
  }

  _exit(0);
}

/** Exercício 3
*
*/
int ex3(){
  int pipe_fd[2];

  pipe(pipe_fd);
  // TODO: verificar se correu bem.

  int pid = -1;
  if((pid = fork()) == 0){
    // não vamos usar o out para escrever.
    close(pipe_fd[1]);

    // redireccionamento do STD IN para o Input do pipe.
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);

    // processo filho executa wc.
    execlp("wc", "wc", NULL);
    _exit(1);
  }else{
    close(pipe_fd[0]);

    char buf[10];
    int bytes = 0;

    while((bytes = read(0, buf, 10)) > 0){
      write(pipe_fd[1], buf, bytes);
    }

    close(pipe_fd[1]);
    wait(NULL);
  }

  _exit(0);
}

// TODO
int ex4(){
  pipe_fd[2];
  pipe(pipe_fd);

  int pid = -1;
  if((pid = fork()) == 0){
    int pid2 = -1;

    if((pid2 = fork()) == 0){

    }
  }else{
    // processo pai.
  }

  _exit(0);
}

// TODO
int ex5(){
  _exit(0);
}
