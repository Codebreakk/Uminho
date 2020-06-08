#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, const char* argv[]){
  int pfd[2];
  int fd = open("fich.txt", O_RDONLY);
  pipe(pfd);
  if (fork()==0) {
    dup2(fd, 0);
    close(fd);
    close(pfd[0]);
    dup2(pfd[1], 1);
    close(pfd[1]);
    execlp("grep", "grep", "palavra", NULL);
    _exit(1);
  }
}
