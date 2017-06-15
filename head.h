#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>

void execute(char **argv,int newsize);
void comecd(char **new,char *curr);
void comecho(char **new,int newsize);
char **split_line(char *line);
char **tokensemi(char *command);
char **lsh_split_line(char *token);
void redirecting(char **new, int newsize);
void piping(char **new, int newsize);
void both(char **new, int newsize);

int idnum[1000];
int idnumf[1000];
char prof[1000][1000];
char pro[1000][1000];
int procnum;
int newsize,countfg;
int erofl,redir,redir2;
char pipcom[1000][1000];
char buf[1000];
