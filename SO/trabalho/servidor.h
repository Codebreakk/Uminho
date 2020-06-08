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
#define ARRAY_SIZE 256

/** nomes dos FIFOS usados pelo cliente e servidor para comunicarem. O nome dos
* fifos indica a direção na qual a informação é enviada. Por exemplo, o fifo
* FIFO_CLIENT_SERVER é usado para escrita pelo cliente e leitura pelo servidor
*
*  CLIENT ===(escreve no)===> FIFO_CLIENT_SERVER ===(é lido pelo)===> SERVIDOR
*  SERVIDOR ===(escreve no)===> FIFO_SERVER_CLIENT ===(é lido pelo)===> CLIENT
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

/** File Descriptor para o ficheiro de log. */
int log_fd;

/** File Descriptor para o ficheiro com o histórico de execução. */
int historico_fd;

/** Lista de estados possíveis para cada tarefa. */
#define A_EXECUTAR 0
#define CONCLUIDA 1
#define INACTIVIDADE 2
#define EXECUCAO 3
#define TERMINADA 4

/** Definições da Queue que guarda as tarefas que estão a ser executadas. Esta
* queue foi definida de acordo com os apontamentos da disciplina de Algoritmos
* e Complexidade relativos a "Estruturas de Dados", como se pode verificar no
* ponto 3.2.1 ("Queues usando listas ligadas"). Contudo foram feitas alterações
* mínimas e algumas correcções.
*/

/** Estrutura que guarda o comando e estado de cada tarefa. */
typedef struct tarefa{
  char* comandos;
  int id;
  int estado; // 0: a executar; 1: concluida; 2: inactividade; 3: execucao; 4: terminada.
  struct tarefa *prox;
} Tarefa;

/** Queue com a lista de tarefas. */
typedef struct queue{
  struct tarefa *inicio, *fim;
}Queue;

/** Apontador para a Queue com as tarefas a executar. */
Queue *queue_tarefas;

/** Lista de funções da Queue. */
void initQueue(Queue *q);

int isEmpty(Queue *q);

int enqueue(Queue *q, char* comandos);

int dequeue(Queue *q, char** comandos);

/** Lista de funções para manipulação dos fifos. */
int open_fifo_server_client();

int close_fifo_server_client();

int open_fifo_client_server();

int close_fifo_client_server();

int parse_comandos(int array_size, char* buf, char * args[]);

/** Lista de funcionalidades implementadas de acordo com o enunciado. */
int tempo_inactividade(int segundos);

int tempo_execucao(int segundos);

void execution_handler(int signum);

int setup_executar(char* comandos);

void timeout_handler(int signum);

int executar(char* comandos);

int listar();

int terminar(int tarefa);

int historico();

int ajuda();
