#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include "my_printf.h"

int tempo_inactividade(int segundos);

int tempo_execucao(int segundos);

int executar(char* comandos);

int listar();

int terminar(int tarefa);

int historico();

int ajuda();
