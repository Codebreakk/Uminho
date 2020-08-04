/** Os fifos foram colocados neste ficheiro de forma a não termos de passar
* repetidamente como argumento da main para cada uma das funções.
*/
int fifo_fd[2];

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
