#include "main.h"

int run_argus(int argc, char * argv[]){

  if(strcmp(argv[0], "-i") == 0 && argc == 2){
    // tempo-inactividade: ./argus -i n
    my_printf(argv[0]);
  }else if(strcmp(argv[0], "-m") == 0 && argc == 2){
    // tempo-execução:     ./argus -m n
  }else if(strcmp(argv[0], "-e") == 0 && argc == 2){
    // executar:           ./argus -e "p1|p2...|pn"
  }else if(strcmp(argv[0], "-l") == 0 && argc == 1){
    // listar:             ./argus -l
  }else if(strcmp(argv[0], "-t") == 0 && argc == 2){
    // terminar:           ./argus -t n
  }else if(strcmp(argv[0], "-r") == 0 && argc == 1){
    // historico:          ./argus -r
  }else if(strcmp(argv[0], "-h") == 0 && argc == 1){
    // ajuda:              ./argus -h
  }

  return 0;
}

int main(int argc, char * argv[]){
  int BUF_SIZE = 256;
  int ARRAY_SIZE = 256;
  char buf[BUF_SIZE];
  int total = 0;

  if(argc == 1 && strcmp(argv[0], "./argus") == 0){
    my_printf("\nargus$ ");
    // TODO: criar o nosso read para evitar problemas com o tamanho dos argumentos
    while((total = read(0, buf, BUF_SIZE)) != 0){
      buf[total] = '\0';
      char* args[ARRAY_SIZE];

      // TODO: testar e melhorar esta função
      tokenize(ARRAY_SIZE, buf, args);
      int length_args = 0;
      while(args[length_args] != NULL){
        length_args++;
      }

      run_argus(length_args, args);
      my_printf("\nargus$ ");
    }
  }else{
    run_argus(argc-1, argv+1);
  }
  return 0;
}
