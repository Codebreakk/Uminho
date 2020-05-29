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

/** nome de cada funcionalidade por extenso */
#define TEMPO_INACTIVIDADE "tempo-inactividade"
#define TEMPO_EXECUCAO "tempo-execucao"
#define EXECUTAR "executar"
#define LISTAR "listar"
#define TERMINAR "terminar"
#define HISTORICO "historico"
#define AJUDA "ajuda"

/** Variável global com o tamanho dos buffers */
#define BUF_SIZE 1024

/** nomes dos FIFOS usados pelo cliente e servidor para comunicarem. O nome dos
* fifos indica a direção na qual a informação é enviada. Por exemplo, o fifo
* FIFO_CLIENT_SERVER é usado para escrita pelo cliente e leitura pelo servidor
*
*  CLIENT ===(escreve no)===> FIFO_CLIENT_SERVER ===(é lido pelo)===> SERVIDOR
*/
#define FIFO_CLIENT_SERVER "fifo_client_server"
#define FIFO_SERVER_CLIENT "fifo_server_client"

/** Nomes dos ficheiros usados pela função ajuda e funcionalidades de logging */
#define HELP "HELP"
#define LOG "log"
#define LOG_IDX "log.idx"

/** Os fifos foram colocados neste ficheiro de forma a não termos de passar
* repetidamente como argumento da main para cada uma das funções.
*/
int fifo_fd[2];

/** File Descriptor para o ficheiro de log */
int log_fd;

/** Lista de funções para manipulação dos fifos */
int open_fifo_server_client();

int close_fifo_server_client();

int open_fifo_client_server();

int close_fifo_client_server();

/** Lista de funcionalidades implementadas de acordo com o enunciado. */
int tempo_inactividade(int segundos);

int tempo_execucao(int segundos);

int executar(char* comandos);

int listar();

int terminar(int tarefa);

int historico();

int ajuda();
