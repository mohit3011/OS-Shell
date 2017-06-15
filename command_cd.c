#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "head.h"


char *homedir;

void comecd(char **new,char *curr)		// Inbuilt cd command
{
	int i;
	if(new[1]==NULL)				// If no path is given after cd then jump to the directory where the q.c is located
	{
		chdir(homedir);
		strcpy(curr,":~");

	}
	else if(strcmp(new[1],"..")==0)		// If cd .. command then
	{											
		for(i=strlen(curr)-1;i>=0;i--)
		{
			if(curr[i]=='/')			// Delecting the last directory name from the path in the command prompt
			{
				curr[i] = '\0';
				break;
			}
			else
			{
				curr[i] = '\0';
			}
		}
	chdir(".."); // change the directory
	}
return;
}
