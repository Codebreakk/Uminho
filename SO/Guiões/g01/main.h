#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

ssize_t myreadln3(int fd, char *line, size_t size);

int mynl(int argc, const char* argv[]);

int readchar(int fd, char* buf);

ssize_t myreadln4(int fd, char *line, size_t size);

int pessoas(int argc, const char* argv[]);

//ex.6
int new_person(char* name, int age);
int person_change_age(char* name, int age);
int person_change_age_v2(long pos, int age);
