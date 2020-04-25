#include "my_printf.h"

/** my_printf(char* string):
*   Versão própria da função printf que recebe uma string e escreve para o stan-
*   dard output.
*   @param String: string a ser impressa para o standard output
*   @return        0 no caso de successo, -1 caso contrário
*/
int my_printf(char* string){
  int size = strlen(string);

  int result = write(1, string, size);

  if(result < 0){
    perror("write");
    return -1;
  }

  return 0;
}

//int main(int argc, char const *argv[]) {
//
//  return 0;
//}
