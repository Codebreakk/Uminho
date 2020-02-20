#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h>    /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
int     open(const char *path, int oflag [, mode]);
ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);
int     close(int fildes);
*/

// ex.01
/** File descriptors
* Standard In:    STDIN_FILENO  = 0
* Standard Out:   STDOUT_FILENO = 1
* Standard Error: STDERR_FILENO = 2
*/
void mycat(){
  int nbyte = 128;
  char *buf = malloc(nbyte*sizeof(char));

  while(read(0, buf, nbyte) != 0){
    write(1, buf, nbyte);
    fflush(0);
  }
}

int main(int argc, const char* argv[]){
  int nbyte = 8;
  char *buf = malloc(nbyte*sizeof(char));

  while(read(0, buf, nbyte) != 0){
    write(1, "escolher exercício:\n", 21);
    write(1, " 1) mycat\n", 11);
    write(1, " 2) 10mb\n", 10);
    write(1, " 1) mycat2\n", 12);

    if(*buf == '1'){
      mycat();
    }
  }

  return 0;
}
