#include "main.h"

int main(int argc, const char* argv[]){

  // Exercício 1
  //ex1();

  // exercício 2
  //ex2();

  // exercício 3
  //ex3(argc, argv);

  // exercício 4
  /**if(strcmp(argv[1], "1") == 0){
    ex3(argc, argv);
  }else{
    ex4(argc, argv);
  }*/
  // exercício 5
  //ex5(argc, argv);

  // exercicio 6
  if(argc < 2){
    return 1;
  }
  int i = 1;
  char* args = malloc(sizeof(argv[0]) * (argc-1));
  strcat(args, argv[i]);
  for(i = 2; i < argc; i++){
    strcat(args, " ");
    strcat(args, argv[i]);
  }

  ex6(args);

  _exit(0);
}
