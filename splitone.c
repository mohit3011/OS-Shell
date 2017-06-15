#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "head.h"

char **lsh_split_line(char *token)
{                                       // This function returns the token in the 2d array
  int bufsize = 1000;                   // initial buffer 
  int i;
  int position = 0;
  char **tokens = malloc(1000* sizeof(char*));
  

  if (!tokens) {
    fprintf(stderr, "Token error\n");     // output error
    tokens[0] = NULL;
    return tokens;
  }
 
  tokens[position] = token;
  position++;                                   // return 2 d array tokens
  return tokens;
}