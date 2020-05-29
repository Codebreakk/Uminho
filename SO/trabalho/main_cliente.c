#include "cliente.h"

int main(int argc, char * argv[]){
  int ARRAY_SIZE = 256;
  char buf[BUF_SIZE];
  ssize_t total = 0;

  // recebeu apenas o comando "./argus" sem argumentos
    if(argc == 1 && strcmp(argv[0], "./argus") == 0){
      my_printf("\nargus$ ");
      // TODO: criar o nosso read para evitar problemas com o tamanho dos argumentos
      while((total = read(0, buf, BUF_SIZE)) != 0){
        buf[total] = '\0';
        char* args[ARRAY_SIZE];

        // TODO: testar e melhorar esta função (ficheiro mySOlib)
        tokenize(ARRAY_SIZE, buf, args);
        int length_args = 0;
        while(args[length_args] != NULL){
          length_args++;
        }

        run_argus(length_args, args);
        my_printf("\nargus$ ");
      }
    }else{
      replace_flags_with_names(argv);

      run_argus(argc - 1, argv + 1);
    }

  return 0;
}
