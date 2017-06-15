#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "head.h"


char **split_line(char *line)     // Command for splitting the command with spaces
{
  int bufsize = 1000, position = 0,i;
  char **tokens = malloc(bufsize * sizeof(char*));    // Initialising buffer 1000 characters
  char *token;

  if (!tokens) {
    fprintf(stderr, "Token error\n");       // Display error
    return tokens;
  }

  token = strtok(line," ");           // Token with delimiter space
  while (token != NULL) 
  {
    tokens[position] = token;         // Fill one command with arguments in a 2d array
    position++;

    if (position >= bufsize) {
      bufsize += 1000;
      tokens = realloc(tokens, bufsize * sizeof(char*));    // Resizing the buffer
      if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        return tokens;
      }
    }

    token = strtok(NULL," ");
  }
 	tokens[position] = NULL;
 	newsize = position;               // Return the size of the 2 d array

 
  return tokens;
}