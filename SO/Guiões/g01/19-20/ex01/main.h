#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdlib.h>
#include <stdio.h>

// exemplo prof.
int example(const char* argv[]);

// ex.1
int mycp(int argc, const char* argv[]);

// ex.2
int mycat();

// ex.3
int myreadln(int argc, const char* argv[]);

// ex.4
int myreadln2(int argc, const char* argv[]);
