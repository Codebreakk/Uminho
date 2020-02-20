#include "main.h"

// exemplo prof.
int example(const char* argv[]){
  // mycp
  int filedesc = -1;
  int i = 0;

  filedesc = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0640);

  if(filedesc < 0){
    perror("open");
    return 1;
  }

  char buf[1] = {'a'};
  while(i < 1024){
    write(filedesc, buf, 1);
    i++;
  }

  return 0;
}

// ex.1
int mycp(int argc, const char* argv[]){
  int fildesc = -1, fildescout = -1;
  char buf[1];

  if(argc < 3) {
    printf("faltam argumentos.\n");
    return 1;
  }
  // leitura do ficheiro de input
  fildesc = open(argv[1], O_RDONLY);
  if(fildesc < 0){
    printf("Erro ao abrir ficheiro.\n");
    return 1;
  }

  // criação do ficheiro de output
  fildescout = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0640);
  if(fildescout < 0){
    printf("Erro ao abrir/criar ficheiro de output.\n");
    return 1;
  }

  while(read(fildesc, buf, 1) != 0){
    write(fildescout, buf, 1);
  }

  close(fildesc);
  close(fildescout);

  return 0;
}

// ex.2
int mycat(){
  int nbyte = 128, nchars = -1;
  char *buf = malloc(nbyte * sizeof(char));

  while((nchars = read(0, buf, nbyte)) != 0){
    write(1, buf, nchars);
  }

  return 0;
}

// ex.3
int myreadln(int argc, const char* argv[]){
  int fildesc = -1;

  if(argc < 2){
    printf("faltam argumentos.\n");
    return 1;
  }

  fildesc = open(argv[1], O_RDONLY);
  if(fildesc < 0){
    printf("Erro ao abrir o ficheiro.\n");
    return 1;
  }

  char buf[1];
  while(read(fildesc, buf, 1) && buf[0] != '\n'){
    write(1, buf, 1);
  }

  return 0;
}

// ex.4
int myreadln2(int argc, const char* argv[]){
  int fildesc = -1, i, bufsize = 32;

  if(argc < 2){
    printf("faltam argumentos.\n");
    return 1;
  }

  fildesc = open(argv[1], O_RDONLY);
  if(fildesc < 0){
    printf("Erro ao abrir o ficheiro.\n");
    return 1;
  }

  char buf[bufsize];
  while(read(fildesc, buf, bufsize)){
    i = 0;
    while(i < bufsize){
      if(buf[0] != '\n'){
        write(1, &buf[i], 1);
        i++;
      }else{
        return 0;
      }
    }
  }

  return 0;
}
