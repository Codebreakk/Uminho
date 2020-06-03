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
  int pipe_fd[2];

  // Criar pipe.
  if(pipe(pipe_fd) < 0){
    perror("pipe");
    _exit(1);
  }

  if(fork() == 0){
    // 0 -> stdin
    // 1 -> stdout
    // pipe_fd[0] -> leitura pipe
    // pipe_fd[1] -> leitura pipe
    close(pipe_fd[0]);


    // TODO: Verificar se close, dup2 e execlp correram com sucesso
    dup2(pipe_fd[1],1);
    close(pipe_fd[1]);

    execlp("ls", "ls", "/etc", NULL);

    _exit(1);
  }else{
    // processo pai.
    if(fork() == 0){
      close(pipe_fd[1]);

      // TODO: Verificar se close, dup2 e execlp correram com sucesso
      dup2(pipe_fd[0], 0);
      close(pipe_fd[0]);

      execlp("wc", "wc", "-l", NULL);

      _exit(1);
    }else{
      close(pipe_fd[0]);
      close(pipe_fd[1]);
      wait(NULL);
      wait(NULL);
    }
  }

  _exit(0);
}

// TODO
/** Exercício 5:
*      Escreva um programa que emule o funcionamento do interpretador de coman-
*      dos na execução encadeada de:
*            grep -v ^# /etc/passwd | cut -f7 d: | uniq | wc -l
*/
int ex5(){
  int commands = 4; // número de comandos executados.
  int pipe_array[3][2]; // N = 3

  // Criar pipe.
  if(pipe(pipe_array[0]) < 0){
    perror("pipe_array[0]");
    _exit(1);
  }

  // 0 - Criar processo "grep".
  if(fork() == 0){
    // pipe_array[0][0]
    // pipe_array[0][1]
    close(pipe_array[0][0]); // Vamos ler do STDIN e não de um pipe.

    dup2(pipe_array[0][1], 1); // Copiamos o pipe para o STDOUT.
    close(pipe_array[0][1]); // Fechamos o pipe (pq já copiamos).

    // 1 -> pipe_array[0][1]
    execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);
    _exit(1);
  }
  // processo pai fecha o pipe do if acima.
  close(pipe_array[0][1]);

  // Criar pipe.
  if(pipe(pipe_array[1]) < 0){
    perror("pipe_array[1]");
    _exit(1);
  }

  // 1 - Criar processo "cut".
  if(fork() == 0){
    // pipe_array[0][0]
    // pipe_array[1][0]
    // pipe_array[1][1]
    close(pipe_array[1][0]); // O filho actual fecha o STDIN do próximo pipe

    dup2(pipe_array[0][0], 0); // O pipe[0][0] é o seu STDIN
    close(pipe_array[0][0]);

    dup2(pipe_array[1][1], 1); // pipe[1][1] é o seu STDOUT
    close(pipe_array[1][1]);

    execlp("cut", "cut", "-f7", "-d:", NULL);

    _exit(1);
  }
  // processo pai fecha os pipes do if acima.
  close(pipe_array[0][0]);
  close(pipe_array[1][1]);

  // Criar pipe_array[2]
  if(pipe(pipe_array[2]) < 0){
    perror("pipe_array[2]");
    _exit(1);
  }

  // 2 - Criar processo uniq
  if(fork() == 0){
    /** pipe_array[1][0]
    *   pipe_array[2][0]
    *   pipe_array[2][1]
    */
    close(pipe_array[2][0]); // O filho actual fecha o STDIN do próximo pipe

    dup2(pipe_array[1][0], 0); // O pipe[1][0] é o seu STDIN
    close(pipe_array[1][0]);

    dup2(pipe_array[2][1], 1); // O pipe[2][1] é o seu STDOUT
    close(pipe_array[2][1]);

    execlp("uniq", "uniq", NULL);
    _exit(1);
  }
  // processo pai fecha os pipes do if acima.
  close(pipe_array[1][0]);
  close(pipe_array[2][1]);

  // 3 - Criar processo wc -l
  if(fork() == 0){
    // pipe_array[2][0]

    // Ultimo processo, apenas tem de copiar o seu pipe para STDIN
    dup2(pipe_array[2][0], 0);// O pipe[2][0] é o seu STDIN
    close(pipe_array[2][0]);

    // O seu STDOUT é o 1.

    execlp("wc", "wc", "-l", NULL);
    _exit(1);
  }
  // processo pai fecha os pipes do if acima.
  close(pipe_array[2][0]);

  for (int i = 0; i < commands; i++) {
    wait(NULL);
  }

  _exit(0);
}
