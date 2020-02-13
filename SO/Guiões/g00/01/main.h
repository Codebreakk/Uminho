#include <stdlib.h>
#include <stdio.h>

int size = 1024;
//int *vector = malloc(size * sizeof(int));

/** insere valor */
void fill(int* vector, int size, int value){
  int i;

  for(i = 0; i < size; i++){
    vector[i] = value;
  }
}

/** Procura value no vector e devolve o indice dele, ou -1 se não existir */
int find(int* vector, int size, int value){
  int i;

  for(i = 0; i < size; i++){
    if(vector[i] == value) return i;
  }

  return -1;
}
