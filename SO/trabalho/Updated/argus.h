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

/** Variável global com o tamanho dos buffers */
#define BUF_SIZE 1024
#define ARRAY_SIZE 256

/** nomes dos FIFOS usados pelo cliente e servidor para comunicarem. */
#define FIFO_CLIENT_SERVER "fifo_client_server"
#define FIFO_SERVER_CLIENT "fifo_server_client"

/** nome de cada funcionalidade por extenso */
#define TEMPO_INACTIVIDADE "tempo-inactividade"
#define TEMPO_EXECUCAO "tempo-execucao"
#define EXECUTAR "executar"
#define LISTAR "listar"
#define TERMINAR "terminar"
#define HISTORICO "historico"
#define AJUDA "ajuda"

/** flag de cada funcionalidade */
#define FLAG_TEMPO_INACTIVIDADE "-i"
#define FLAG_TEMPO_EXECUCAO "-m"
#define FLAG_EXECUTAR "-e"
#define FLAG_LISTAR "-l"
#define FLAG_TERMINAR "-t"
#define FLAG_HISTORICO "-r"
#define FLAG_AJUDA "-h"

/**  */
#define WHITESPACE " "

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
