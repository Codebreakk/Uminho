#include "cliente.h"

int main(int argc, char * argv[]){
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE); // limpar buf.
  ssize_t total = 0;
  // recebemos apenas o comando "./argus" (sem argumentos)
  if(argc == 1 && strcmp(argv[0], "./argus") == 0){
    my_printf("\nargus$ ");
    // TODO: criar o nosso read para evitar problemas com o tamanho dos argumentos
    while((total = read(0, buf, BUF_SIZE)) != 0){
      // my_printf2("buf = %s\n", buf); // DEBUG
      char* args[ARRAY_SIZE];
      if(strlen(buf) > 0){
        int length_args = 0;
        if(strncmp(buf, EXECUTAR, strlen(EXECUTAR)) == 0){
          args[0] = EXECUTAR;
          args[1] = malloc(total - 11);
          if(strlen(buf) > 10) {
            char* executar = buf + 10;
            char* pointer = strchr(executar, '"');
            *pointer = '\0'; // apaga a partir do char '"' para a frente
            // my_printf2("%s\n", executar); // DEBUG
            strcpy(args[1], executar);
            length_args = 2;
          }else{
            length_args = 1;
            args[1] = "";
          }
        }else{
          // TODO: testar e melhorar esta função (ficheiro mySOlib)
          tokenize(args, buf, ARRAY_SIZE);
          // my_printf2("args0 = %s\nargs1 = %s\n", args[0], args[1]); // DEBUG
          while(args[length_args] != NULL){
            length_args++;
          }
        }
        // Passar a lista dos argumentos para a função auxiliar que comunica com o servidor
        run_argus(length_args, args);
      }
      // Limpar o buf
      memset(buf, 0, BUF_SIZE);
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
