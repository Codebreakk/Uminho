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

  free(buf);
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

  int bufsize = 1024;
  char buf[bufsize];
  ssize_t result = 0;
  while((result = myreadln3(fildesc, buf, bufsize)) > 0){
    write(1, buf, result);
  }

  return 0;
}

// readln invocada no ex.3
// esta função lê do fd e escreve para a line, retornando o número de chars lidos.
ssize_t myreadln3(int fd, char *line, size_t size){
  int i = 0, n = 0;

  if(fd < 0){
    perror("File descriptor inválido.");
  }

  while((n = read(fd, &line[i], 1)) > 0 && i < size) {
    i++;
    if(line[i] == '\n') return i;
  }

  return i;
}

// ex.4
int myreadln2(int argc, const char* argv[]){
  int fildesc = -1, i, bufsize = 32, readsize = -1;

  if(argc < 2){
    printf("faltam argumentos.\n");
    return 1;
  }

  fildesc = open(argv[1], O_RDONLY);
  if(fildesc < 0){
    perror("Erro ao abrir o ficheiro");
    return 1;
  }

  char buf[bufsize];
  while((readsize = read(fildesc, buf, bufsize)) > 0){
    for(i = 0; i < readsize; i++){
      if(buf[i] != '\n'){
        write(1, &buf[i], 1);
      }else{
        return 0;
      }
    }
  }

  return 0;
}

// readln invocada no ex.4
// esta função lê do fd e escreve para a line, retornando o número de chars lidos.
ssize_t myreadln4(int fd, char *line, size_t size){
  int i = 0, n = 0;

  if(fd < 0){
    perror("File descriptor inválido.");
  }

  while((n = readchar(fd, &line[i])) > 0 && i < size) {
    i++;
    if(line[i] == '\n') return i;
  }

  return i;
}

//lê 1 char para o buf
int readchar(int fd, char* buf){
  static int pos = 0;
  static int read_bytes = 0;
  static char buf[1024];
  
  if(pos == read_bytes){
    read_bytes = read(fd, buf, sizeof(buf));
    pos = 0;
  }else{

  }
}

// ex.5
int mynl(int argc, const char* argv[]){
  int n_linha = 0, bufsize = 1024, n_lidos = 0;
  char buf[bufsize];

  while((n_lidos = myreadln3(0, buf, bufsize)) > 0){
    write(1, buf, n_lidos);
    n_linha++;
  }

  return 0;
}

// ex.6
/**
int pessoas(char* filename, char* flag, char* nome, int idade){
  int fdfile = -1;
  if(argc < 4){ // faltam argumentos;
    return -1;
  }

  // abrir ficheiro
  fdfile = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0640);

  if(fdfile < 0){
    return -1; // não foi possivel abrir o ficheiro;
  }

  if(argv[1] == "-i"){
    // insert

  }else if(argv[1] == "-u"){
    // update
  }

  return 0;
}
*/
