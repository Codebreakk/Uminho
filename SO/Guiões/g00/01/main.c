#include "main.h"

int main(int argc, const char* argv[]){

  int vector[size];
  fill(vector, size, 1);
  printf("%d\n", find(vector, size, 0));

  return 0;
}
