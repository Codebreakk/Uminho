#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "my_printf.h"

int my_mkfifo(int argc, char * argv[]);

int wfifo(int argc, char * argv[]);

int rfifo(int argc, char * argv[]);

//int ex4();

//int ex5();
