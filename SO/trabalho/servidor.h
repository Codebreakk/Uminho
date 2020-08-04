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

/** No geral, se alguma função retornar -1 consideramos erro */
#define ERRO -1

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
#define CONCLUIDA 0
#define A_EXECUTAR 1
#define INACTIVIDADE 2
#define EXECUCAO 3
#define TERMINADA 4

/** Lista de mensagens escritas para o historico. */
#define MSG_CONCLUIDA ", concluida: "
#define MSG_MAX_INACTIVIDADE ", max inactividade: "
#define MSG_MAX_EXECUCAO ", max execução: "
#define MSG_TERMINADA ", terminada: "
#define MSG_ERRO ", erro: "

/** Definições da Queue que guarda as tarefas que estão a ser executadas. Esta
* queue foi definida de acordo com os apontamentos da disciplina de Algoritmos
* e Complexidade relativos a "Estruturas de Dados", como se pode verificar no
* ponto 3.2.1 ("Queues usando listas ligadas"). Contudo foram feitas alterações
* para adaptar ao nosso caso.
*/

/** Estrutura que guarda o comando e estado de cada tarefa. */
typedef struct tarefa{
  char* comandos;
  int id;
  int pid;
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

int enqueue(Queue *q, char* comandos, int id, int pid);

int dequeue(Queue *q, int id);

int dequeue_by_pid(Queue *q, int pid, int return_val);

/** Lista de funções para manipulação dos fifos. */
int open_fifo_server_client();

int close_fifo_server_client();

int open_fifo_client_server();

int close_fifo_client_server();

int write_to_historico(int id, char *coms_string, int estado);

int parse_comandos(int array_size, char* buf, char * args[]);

/** Lista de funcionalidades implementadas de acordo com o enunciado. */
int tempo_inactividade(int segundos);

int tempo_execucao(int segundos);

void execution_handler(int signum);

void sigchild_handler(int signum);

int setup_executar(char* comandos);

void timeout_handler(int signum);

int executar(char* comandos, int id);

int listar();

int terminar(int tarefa);

int historico();

int ajuda();
