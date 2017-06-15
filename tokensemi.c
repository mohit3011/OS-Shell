#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <sys/wait.h>

int sizearr;

char **tokensemi(char *command)			// Command for tokensing with semicolon as delimiter
{
	int buffersize = 1000;				// Initialising the buffer size of 1000 characters
	char **tokens = malloc(1000* sizeof(char*));
	int position=0;
	char *token;
	token  = strtok(command,";\n");			// Strtok for tokenising
	while(token!=NULL)
	{
		tokens[position] = token;
		position++;							// Fill all the commands in the 2 d array and return the no of rows in the array

		if (position >= buffersize) 
		{
      		buffersize += 1000;
      		tokens = realloc(tokens, buffersize * sizeof(char*)); // If the buffer is short then increase the buffer
      		if (!tokens) 
      		{
        		fprintf(stderr, "Allocation error\n");
        	return tokens;
      		}
    	}
		sizearr++;
		token = strtok(NULL,";\n");

	}
	return tokens;					// return tokens
}
