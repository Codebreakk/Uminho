#include "cliente.h"

int main(int argc, char * argv[]){
  char buf[BUF_SIZE];
  ssize_t total = 0;
  // recebemos apenas o comando "./argus" (sem argumentos)
  if(argc == 1 && strcmp(argv[0], "./argus") == 0){
    my_printf("\nargus$ ");
    // TODO: criar o nosso read para evitar problemas com o tamanho dos argumentos
    while((total = read(0, buf, BUF_SIZE)) != 0){
      buf[--total] = '\0';
      char* args[ARRAY_SIZE];
      my_printf2("buf = %s\n", buf);
      int length_args = 0;
      if(strncmp(buf, EXECUTAR, 8) == 0){
        my_printf2("9 = %s\n", buf+9);
        args[0] = EXECUTAR;
        args[1] = malloc(total - 11);
        // TODO: (este strncpy está a copiar mal.) Já parece funcionar.
        strncpy(args[1], buf + 10, strlen(buf) - 11); // executar "wc | ls | ls -l"
        my_printf2("After strncpy: %s\n", args[1]);
        length_args = 2;
      }else{
        // TODO: testar e melhorar esta função (ficheiro mySOlib)
        tokenize(ARRAY_SIZE, buf, args);
        while(args[length_args] != NULL){
          length_args++;
        }
      }
      // Passar a lista dos argumentos para a função auxiliar que comunica com o servidor
      run_argus(length_args, args);
      my_printf("\nargus$ ");
    }
  }else{
    // recebemos um comando com argumentos (ex.: ./argus -h)
    // traduzir as flags para comandos
    replace_flags_with_names(argv);
    // Passar os argumentos do comando para a função auxiliar que comunica com o servidor
    run_argus(argc - 1, argv + 1);
  }
  return 0;
}
