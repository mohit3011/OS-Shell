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


int i,in1,out1,j,cmderr;
int fd[2];
int stpipe[1000],pointpipe,indexl,indexr,indexdr;
int prev,previ,ini,outi,size1;


int func1(int in1,int out, int i1,char **new,char **comm1)
{

	//printf("%d %d %d\n",indexl,stpipe[i1],in1);
	pid_t pid;
	pid = fork();

	if(pid<0)
	{
		printf("Error id forking\n");
	}

	else if(pid==0)
	{
		if(i1==0 && in1!=0)
		{
			for(j=indexl+1;j<stpipe[0];j++)
			{
				comm1[j-indexl-1] = new[j];

			}
			comm1[stpipe[0]-indexl-1] = NULL;
			size1 = stpipe[0]-indexl-1;
		}

		if(i1==0 && in1==0)
		{
			for(j=0;j<stpipe[0];j++)
			{
				comm1[j] = new[j];

			}
			comm1[stpipe[0]] = NULL;
			size1 = stpipe[0];
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
			close(in1);
		}

		if(out!=1)
		{
			dup2(out,1);
			close(out);
		}

		if(strcmp(comm1[0],"echo")==0)
		{
			comecho(comm1,size1);

		}
		else
		{
			execute(comm1,size1);
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





void both(char **new, int newsize)
{
	int ind = 0;
	pointpipe = 0;
	cmderr = 0;
	pid_t pid;
	int count = 0,infd,size;
	indexl = -1;
	indexr = -1;
	indexdr = -1;
	char **comm1 = malloc(1000 * sizeof(char*));
	char filename1[1000];


	//printf("%d\n",newsize);
	for(i=0;i<newsize;i++)
	{
		if(strcmp(new[i],"<")==0)
		{
			indexl = i;
		}	

		if(strcmp(new[i],">")==0)
		{
			indexr = i;
		}

		if(strcmp(new[i],">>")==0)
		{
			indexdr = i;
		}


		if(strcmp(new[i],"|")==0)
		{	
			//printf("%d\n",pointpipe);
			stpipe[pointpipe]=i;
			pointpipe++;
		}
	}

	if(indexl > stpipe[0] || (indexr < stpipe[pointpipe-1] && indexr!=-1))
	{
		fprintf(stderr, "Use redirection and Pipe in correct order\n");
		cmderr = 1;	

	}



	outi=dup(1);
	ini=dup(0);

	if(indexl!=-1 && cmderr==0)
	{
		//printf("Hello\n");
		strcpy(filename1,new[indexl+1]);
		new[indexl] = NULL;
		size = indexl;
	    infd = open(filename1,O_RDONLY);
	    if(infd<0)
	    {
			fprintf(stderr, "Error in opening the file\n");	
	    }

        close(0);
    	dup2(infd,0);
    	close(infd);
    	pipe(fd);
    	dup2(fd[1],1);
    	close(fd[1]);
    	if(strcmp(new[0],"echo")==0)
    	{
    		comecho(new,size);

    	}
    	else
    	{
    		execute(new,size);
    	}
    	in1 = fd[0];
    	ind = 1;
    	dup2(outi,1);
    	
	}

	if(indexl==-1 && cmderr==0)
	{
		in1 = 0;
	}
	if(cmderr==0)
	{
		for(i=ind;i<pointpipe;i++)
		{

			pipe(fd);

			
			func1(in1,fd[1],i,new,comm1);

			close(fd[1]);
			in1 = fd[0];


		}
	}

	if(in1!=0 && cmderr==0)
	{
		dup2(in1,0);
		close(in1);
	}
	if(indexr!=-1 && cmderr==0)
	{
		//printf("Hi\n");
		strcpy(filename1,new[indexr+1]);
		//printf("%s\n",filename1);
	    infd = open(filename1,O_WRONLY | O_TRUNC | O_CREAT, 0644);
	    if(infd<0)
	    {
			fprintf(stderr, "Error in opening the file\n");	

	    }
	    newsize = indexr;
		//printf("%d\n",newsize);



	}


	if(indexdr!=-1 && cmderr==0)
	{
		//printf("Hi\n");
		strcpy(filename1,new[indexdr+1]);
		//printf("%s\n",filename1);
	    infd = open(filename1,O_WRONLY | O_APPEND);
	    if(infd<0)
	    {
			fprintf(stderr, "Error in opening the file\n");	

	    }
	    newsize = indexdr;
		//printf("%d\n",newsize);



	}

	if(indexr==-1 && cmderr==0)
	{
		//printf("Galti\n");
		infd = outi;
	}

	if(cmderr==0)
	{

		for(i=stpipe[pointpipe-1]+1;i<newsize;i++)
		{
			comm1[i-stpipe[pointpipe-1]-1] = new[i];
		}

		comm1[i-stpipe[pointpipe-1]-1] = NULL;
		size1 = i-stpipe[pointpipe-1]-1;

		/*for (int i = 0; i < size1; i++)
		{
			printf("%s\n",comm1[i]);
		}*/

		dup2(infd,1);
		close(infd);

		if(strcmp(comm1[0],"echo")==0)
		{
			comecho(comm1,size1);

		}
		else
		{
			execute(comm1,size1);
		}
		
		
		dup2(ini,0);
		dup2(outi,1);

	}
return;	
}