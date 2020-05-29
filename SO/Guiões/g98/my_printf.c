#include "my_printf.h"

// TODO: talvez seja melhor renomear este ficheiro para "mysolib" visto que já
// existem algumas funções que são usadas para outros propósitos que não imprimir.

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

// TODO: Esta função ainda não foi devidamente testada.
/** my_printf2(char* string,...):
*   Versão própria da função printf que recebe uma string com flags  e uma lista
*   variável de argumentos e substitui as flags pelos devidos argumentos. Neste
*   momento apenas suporta 3 flags: %d, %s e %c.
*   @param String: string (com flags) a ser impressa para o standard output
*   @param ...   : lista variável de argumentos a serem impressos na String
*   @return        0 no caso de successo, -1 caso contrário //TODO: implementar este return!
*/
int my_printf2(char* string,...){
  int ARRAY_SIZE = 2048;
  va_list var_list;
  va_start(var_list, string);
  char delimiter[] = "%";
  int n = 0;
  char* fst_token = strdup(string);
  char* string_as_token[ARRAY_SIZE];

  char fst_char = string[0];
  // putchar(fst_char);

  // parte a string para um array por cada espaço (" ") que encontra.
  string_as_token[n] = strtok(fst_token, delimiter);
  while(string_as_token[n] && n < ARRAY_SIZE - 1){
    // printf("%s.", string_as_token[n]);
    string_as_token[++n] = strtok(NULL, delimiter);
  }

  // Se o primeiro char da primeira string não é '%', então avança para a próxima
  // string. Este passo tem de ser feito devido ao facto de strtok colocar na po-
  // sição 0 a substring que vai da posição 0 até à primeira ocurrência de '%'.
  int i = 0;
  if(fst_char != '%'){
    my_printf(string_as_token[0]);
    i = 1;
  }

  char * sub = NULL;
  for(; string_as_token[i] && i < ARRAY_SIZE; i++){
    char* token = strdup(string_as_token[i]);
    char flag = token[0];
    //putchar(flag);
    if(flag == 'c'){
      // char
      char character = va_arg(var_list, int);
      token[0] = character;
      my_printf(token);
    }else if(flag == 'd'){
      // signed decimal integer
      int integer = va_arg(var_list, int);
      char * integer_as_string = integer_to_string(integer);
      strcat(integer_as_string, token + 1);
      my_printf(integer_as_string);
    }else if(flag == 's'){
      // string
      sub = va_arg(var_list, char *);
      char * to_print = malloc(sizeof(char*) * (sizeof(sub) + sizeof(token)));
      strcpy(to_print, sub);
      strcat(to_print, token + 1);
      my_printf(to_print);
    }else{
      // imprime a string sem tratamento
      my_printf(token);
      continue;
    }
  }
  va_end(var_list);
  return 0;
}

char * integer_to_string(int integer){
  static char representation[] = "0123456789";
  static char buffer[50];
  char* string;
  int is_negative = 0;

  if(integer < 0){
    is_negative = 1;
  }

  string = &buffer[49];
  *string = '\0';

  while(integer != 0){
    *--string = representation[integer%10];
    integer = integer/10;
  }

  if(is_negative == 1){
    *--string = '-';
  }

  return (string);
}

// TODO: parece funcionar mas precisa ser testada.
/** Parte a string "buf" em várias strings separadas por espaço e escreve essas
* strings para o array "args". Retorna o número de elementos no array "args";
*/
int tokenize(int ARRAY_SIZE, char* buf, char * args[]){
  int n = 0;

  args[n] = strtok(buf, " ");
  while(args[n] && n < ARRAY_SIZE - 1){
    args[++n] = strtok(NULL, " ");
  }

  return n;
}
