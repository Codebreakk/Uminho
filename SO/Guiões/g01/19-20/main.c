#include "main.h"

/**
* int     open(const char *path, int oflag [, mode]);
* ssize_t read(int fildes, void *buf, size_t nbyte);
* ssize_t write(int fildes, const void *buf, size_t nbyte);
* off_t   lseek(int fd, off_t offset, int whence);
* int     close(int fildes);
*/

/** File descriptors
* Standard In:    STDIN_FILENO  = 0
* Standard Out:   STDOUT_FILENO = 1
* Standard Error: STDERR_FILENO = 2
*/

void print_menu(){
  char firstline[] = "Escolher exercicio:\n";
  char options[] = "  0) exemplo\n  1) mycp\n  2) mycat\n  3) readln\n\
  4) readln2\n  5) myln\n  6) pessoas\n";
  char exit[] = "(Para sair, usar Control + D)\n";
  write(1, firstline, sizeof(firstline));
  write(1, options, sizeof(options));
  write(1, exit, sizeof(exit));
}

int main(int argc, const char* argv[]){
  int result = -1;
  char buf[1];

  print_menu();
  while(read(0, buf, 1) != 0){
    if(buf[0] == '0'){
      // exemplo prof.
      result = example(argv);
    }else if(buf[0] == '1'){
      // ex.1
      result = mycp(argc, argv);
      return result;
    }else if(buf[0] == '2'){
      // ex.2
      result = mycat();
    }else if(buf[0] == '3'){
      // ex.3
      result = myreadln(argc, argv);
      printf("\n");
    }else if(buf[0] == '4'){
      result = myreadln2(argc, argv);
      printf("\n");
    }else if(buf[0] == '5'){
      result = mynl(argc, argv);
      printf("\n");
    }/**else if(buf[0] == '6'){
      result = pessoas(argc, argv);
      printf("\n");
    }*/else{
      print_menu();
    }
  }

  return result;
}
