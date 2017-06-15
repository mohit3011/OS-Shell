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


int i,in1,out1,j;
int fd[2];				// file descriptor
int stpipe[1000],pointpipe;
int prev,previ,ini,outi,size1;


int func(int in1,int out, int i1,char **new,char **comm1)
{

	pid_t pid;
	pid = fork();

	if(pid<0)
	{
		printf("Error id forking\n");
	}

	else if(pid==0)
	{
		if(i1==0)
		{
			for(j=0;j<stpipe[0];j++)				// Function for piping 
			{
				comm1[j] = new[j];

			}
			comm1[stpipe[0]] = NULL;
			size1 = stpipe[0];						// If the first pipe
		}

		if(i1!=0)
		{
			for(j=stpipe[i1-1]+1;j<stpipe[i1];j++)
			{
				comm1[j-stpipe[i1-1]-1] = new[j];
			}
			comm1[j-stpipe[i1-1]-1] = NULL;
			size1 = j - stpipe[i1-1] - 1;
		}
		if(in1!=0)
		{
			dup2(in1,0);
			close(in1);							// Replacing the stdin
		}

		if(out!=1)
		{
			dup2(out,1);
			close(out);						// Replacing the stdout
		}
		if(strcmp(comm1[0],"echo")==0)
		{
			comecho(comm1,size1);

		}
		else
		{
			execute(comm1,size1);			// Executing the command
		}
		exit(0);
	}

	else
	{
		int returnStatus;    
		waitpid(pid, &returnStatus, 0);
	}
return pid;
}





void piping(char **new, int newsize)
{
	pointpipe = 0;				//pointer
	pid_t pid;
	int count = 0;
	char **comm1 = malloc(1000 * sizeof(char*));


	for(i=0;i<newsize;i++)
	{	

		if(strcmp(new[i],"|")==0)
		{	

			stpipe[pointpipe]=i;			// noting down all the indexes with "|"
			pointpipe++;
		}
	}
	outi=dup(1);							// duplicating the stdout
	ini=dup(0);								// duplicating the stdin

	in1 = 0;
	for(i=0;i<pointpipe;i++)
	{
		pipe(fd);

		
		func(in1,fd[1],i,new,comm1);

		close(fd[1]);
		in1 = fd[0];


	}

	if(in1!=0)
	{
		dup2(in1,0);
	}
	for(i=stpipe[pointpipe-1]+1;i<newsize;i++)
	{
		comm1[i-stpipe[pointpipe-1]-1] = new[i];
	}
	if(count==0)
	{

		comm1[i-stpipe[pointpipe-1]-1] = NULL;			// For the last command after pipe
		size1 = i-stpipe[pointpipe-1]-1;

		dup2(outi,1);
		if(strcmp(comm1[0],"echo")==0)
		{
			comecho(comm1,size1);

		}
		else
		{
			execute(comm1,size1);
		}
		count++;
	}
	dup2(ini,0);
	dup2(outi,1);

return;	
}