#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int func1(){
  printf("FUNC1\n");
  return 0;
}

int main(int argc, const char* argv[]){
  for (int i = 0; i < 3; i++) {
    if (fork() == 0) {
      printf("processo %d\n", i);
      func1();
    } else {
      wait(NULL);
      printf("processo %d terminou\n", i);
      _exit(0);
    }
  }
}
