#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include "mySOlib.h"

/** nomes dos FIFOS usados pelo cliente e servidor para comunicarem. O nome dos
* fifos indica a direção na qual a informação é enviada. Por exemplo, o fifo
* FIFO_CLIENT_SERVER é usado para escrita pelo cliente e leitura pelo servidor
*
*  CLIENT ===(escreve no)===> FIFO_CLIENT_SERVER ===(é lido pelo)===> SERVIDOR
*/
#define FIFO_CLIENT_SERVER "fifo_client_server"
#define FIFO_SERVER_CLIENT "fifo_server_client"

/** Nome do ficheiro lido pela função ajuda */
#define HELP "help"

/** Os fifos foram colocados neste ficheiro de forma a não termos de passar
* repetidamente como argumento da main para cada uma das funções.
*/
int fifo_fd[2];

/** Lista de funcionalidades implementadas de acordo com o enunciado. */
int tempo_inactividade(int segundos);

int tempo_execucao(int segundos);

int executar(char* comandos);

int listar();

int terminar(int tarefa);

int historico();

int ajuda();
