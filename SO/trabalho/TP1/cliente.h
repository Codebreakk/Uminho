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

/** fifos de leitura e escrita do servidor */
int fifo_fd[2];

/** funções responsáveis por abrir e fechar os fifos com as respectivas
* permissões */
int open_fifo_client_server();

int close_fifo_client_server();

int open_fifo_server_client();

int close_fifo_server_client();

/** substitui todas as flags pelos respectivos nomes de cada funcionalidade */
void replace_flags_with_names(char * argv[]);

/** recebe os comandos já tratados pela main e envia pelo pipe de escrita para o
* servidor */
int run_argus(int argc, char * argv[]);
