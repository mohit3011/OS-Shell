#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "head.h"


void comecho(char **new,int newsize)		// Echo builtin command
{
	int flage = 0;
	int i,j;

	if(new[1][0]=='"')						// If the echo command "" the flage=1
	{
		flage = 1;
	}
	
	if(flage==1)						// If the flage==1
	{											
		for(i=1;i<newsize;i++)
		{
			for(j=0;j<strlen(new[i]);j++)															// for printing all the characters between ""
			{
				if((i==1 && j==0) || (i==newsize-1 && j==strlen(new[newsize-1])-1))
				{
						;
				}

				else
				{
					printf("%c",new[i][j]);
				}
			}
			printf(" ");

		}
		printf("\n");
	}

	else
	{
		for(i=1;i<newsize;i++)
		{
			for(j=0;j<strlen(new[i]);j++)				// Else print all the characters
			{
				printf("%c",new[i][j]);
			}
			printf(" ");

		}
		printf("\n");
	}

return;
}
