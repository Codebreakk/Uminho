#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <sys/wait.h>
//#include <fcntl.h>

int my_printf(char* string);

int my_printf2(char* string,...);

char * integer_to_string(int integer);

int tokenize(int ARRAY_SIZE, char* buf, char * args[]);

ssize_t my_readln(int fd, char *line, size_t size);

int my_readchar(int fd, char* buf);