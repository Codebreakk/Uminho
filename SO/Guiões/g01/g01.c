#include "main.h"

// VARIÁVEIS GLOBAIS
int pos;
int read_bytes = 0;
char buf_aux[1024];

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
  int fildesc = -1, i, readsize = -1;
  size_t bufsize = 256;
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
  pos = 0;
  while((readsize = myreadln4(fildesc, buf, bufsize)) > 0){
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

  while((n = readcharprof(fd, &line[i])) > 0 && i < size) {
    i++;
    if(line[i] == '\n') return i;
  }

  return i;
}

//int pos;
//int read_bytes = 0;
//char buf_aux[1024];
//lê 1 char para o buf
int readchar(int fd, char* buf){ // não está terminada!!
  //static int pos = 0;
  //static int read_bytes = 0;
  //static char buf_aux[1024];

  // Se li tudo, tenho de pedir mais caracteres.
  if(pos == read_bytes){
    read_bytes = read(fd, buf_aux, sizeof(buf_aux));
    pos = 0;
  }else{
    // caso contrário, devo continuar a ler.
    buf[pos] = buf_aux[pos];
    pos++;
    return pos;
  }

  return pos;
}

// readchar do professor
int readcharprof(int fd, char* buf){
  static int pos = 0;
  static int read_bytes = 0;
  static char buf_aux[1024];

  if(pos == read_bytes){
    // EOF
    int bytes = 0; // onde é declarado isto???
    if((bytes = read(fd, buf_aux, sizeof(buf_aux))) < 1){
      return 0;
    }

    pos = 0;
    read_bytes = bytes;
  }
  *buf = buf_aux[pos];
  pos++;
  return 1;
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
typedef struct Person{
  char name[200];
  int age;
} Person;

//API
//int new_person(char* name, int age);
//int person_change_age(char* name, int age);
//int person_change_age_v2(long pos, int age);

int pessoas(int argc, const char* argv[]){

  if(argc < 5){
    return 1; // faltam argumentos.
  }
  char* nome = "Jose Mourinho";
  new_person(nome, 20);

  return 0;
}

int new_person(char *name, int age){
  Person person;// = malloc(sizeof(Person));
  int result = 0;
  int fd = open("pessoas", O_CREAT | O_APPEND | O_WRONLY, 0640);
  int offset = lseek(fd, 0, SEEK_END);

  if(fd < 0){
    perror("Erro ao abrir o ficheiro");
    return 1;
  }

  strcpy(person.name, name);
  person.age = age;

  result = write(fd, &person, sizeof(Person));

  if(result < 0){
    perror("Erro ao escrever para o ficheiro");
  }

  result = close(fd);

  if(result < 0){
    perror("Erro ao fechar o ficheiro");
  }

  return 0;
}


int person_change_age(char* name, int age){
  return 0;
}
