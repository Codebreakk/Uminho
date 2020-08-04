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
