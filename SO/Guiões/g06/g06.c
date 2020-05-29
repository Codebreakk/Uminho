#include "main.h"

//int mkfifo(const char *pathname, mode_t mode);

/** Exercício 1
*       Escreva três programas. O primeiro cria apenas um pipe com nome "fifo".
* O segundo repete para este pipe todas as linhas de texto lidas a partir do seu
* standard input. O terceiro programa deverá repetir para o seu standard output
* todas as linhas de texto lidas a partir deste pipe. Repare que ao contrário
* dos pipes anónimos, o pipe corresponde a uma entrada no sistema de ficheiros,
* sujeito ao mesmo controlo de acesso dos ficheiros normais, e não obriga à
* criação do pipe por um processo ascendente dos processos em comunicação.
* Aliás, a comunicação pode mesmo realizar-se entre processos de utilizadores
* distintos. Note ainda, que tal como nos pipes anónimos, as operações de lei-
* tura e escrita no pipe oferecem um canal unidireccional sob uma política FIFO
* e diluição da eventual fronteira das escritas. No entanto, ao contrário dos
* pipes anónimos, a abertura para escrita de um pipe com nome bloqueia até que
* um processo o abra para leitura, e vice-cersa.
*/

int my_mkfifo(int argc, char * argv[]){
  if(mkfifo(argv[1], 0666) < 0){
    perror("mkfifo");
    exit(1);
  }

  return 0;
}

int wfifo(int argc, char * argv[]){

  int fifo_fd;
    if((fifo_fd = open(argv[1], O_WRONLY)) < 0){
      perror("fifo open");
      exit(1);
    }

  my_printf("fifo is now open.\n");

  char buf[100];
  int bytes_read;
  while((bytes_read = read(0, buf, 100)) > 0){
    write(fifo_fd, buf, bytes_read);
  }

  close(fifo_fd);

  return 0;
}


int rfifo(int argc, char * argv[]){

  int fifo_fd;
  if((fifo_fd = open(argv[1], O_RDONLY)) < 0){
    perror("fifo open");
    exit(1);
  }

  printf("fifo is now open.\n");

  char buf[100];
  int bytes_read;
  while((bytes_read = read(fifo_fd, buf, 100)) > 0){
    write(1, buf, bytes_read);
  }

  close(fifo_fd);

  return 0;
}

/** Exercício 2
*       Escreva um programa "servidor", que fique a correr em background, e
* acrescente a um ficheiro de "log" todas as mensagens que sejam enviadas por
* "clientes". Escreva um programa cliente que envia para o servidor o seu argu-
* mento. Cliente e servidor devem comunicar via pipes com nome.
*/
int servidor(int argc, char * argv[]){

  int file_fd;
  if((file_fd = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY)) < 0){
    perror("file open");
    exit(1);
  }

  int fifo_fd;

  while((fifo_fd = open(argv[1], O_RDONLY)) > 0){
    printf("fifo is now open.\n");

    char buf[100];
    int bytes_read;
    while((bytes_read = read(fifo_fd, buf, 100)) > 0){
      write(file_fd, buf, bytes_read);
    }

    close(fifo_fd);
  }

  close(file_fd);

  return 0;
}

//int cliente(int argc, char * argv[]){
//}
