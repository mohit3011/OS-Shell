#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "head.h"



void piping(char **new, int newsize)
{
	int i;
	input=-1;
	output=-1;
	int piping[1000][2];
	int stpipe[1000],pointpipe=0;
	int prev,previ,in,out,size1;
	char **comm1 = malloc(1000 * sizeof(char*));
	pid_t pid;

	for(i=0;i<newsize;i++)
	{	

		if(strcmp(new[i],"|")==0)
		{	

			pipe(piping[pointpipe]);
			stpipe[pointpipe]=i;
			pointpipe++;
			new[i]=NULL;
		}
	}
	int length=i;
	out=dup(1);
	in=dup(0);
	/*if(input==1)
	{

		int fd=open(arg[stpipe[0]-1],O_RDONLY);

		arg[stpipe[0]-2]=NULL;
		if(fd<0)
		{	
			puts("input file missing");
			return ;
		}
		dup2(fd,0);
		close(fd);
	}*/

	pid=fork();
	if(pid<0)
	{
		printf("Error creating process\n");
		_exit(1);
	}
	else if(pid==0)  //Child Process
	{
		int ret;
		dup2(piping[0][1],1);
		for(i=0;i<pointpipe;i++)
		{	
			close(piping[i][0]);
			close(piping[i][1]);
		}
		execute(new,stpipe[0]);
		_exit(0);
	}
	else
	{

		for(i=1;i<pointpipe;i++)
		{
			pid=fork();
			if(pid==0)
			{
				dup2(piping[i][1],1);
				dup2(piping[i-1][0],0);
				int yy=0;
				int j;

				for(j=0;j<pointpipe;j++)
				{	
					close(piping[j][0]);
					close(piping[j][1]);
				}

				for(j=stpipe[i-1]+1;j<stpipe[i];j++)
				{
					comm1[j-stpipe[i-1]-1] = new[j];
				}
				size1 = j - stpipe[i-1]-1;
				execute(comm1,size1);

				_exit(0);
			}
		}
	}
}