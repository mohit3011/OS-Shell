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

int andp,inf;
char name[1000];


void sigchld_handler (int sig) {					// function for exiting the background process 
    int status,i,index;
    pid_t child;

    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
       
        for(i=0;i<procnum;i++)
        {
          if(idnum[i]==child)
          {
            index = i;
            idnum[i] = -1;
          }
        }
        printf("\n%s with pid %d exited normally\n",pro[index],child);	//Printing the message

    }
}

void  execute(char **argv,int newsize)		// function for executing the commands
{
	int i,index,waitpidi;
	int flag = 0;
    pid_t  pid;							// pid
    int    status;

    

     for(i=0;i<newsize;i++)
     {
     		if (argv[i][0]=='&')
     		{
     			index = i;  			// For finding out whether it is a background process
     			flag = 1;
     			break;
     		}
     }

     if(flag==1)
     {
     	argv[index]='\0'; 				// if the process is a background process
     }

     if ((pid = fork()) < 0) {     // fork a child process           
         
          fprintf(stderr, "*** ERROR: forking child process failed\n");
          return;
     }
     else if (pid == 0) 
     {          									//for the child process:  
          if (execvp(*argv, argv) < 0) 
          {     									// execute the command  
              
               fprintf(stderr, "*** ERROR: exec failed\n");
               erofl = 1;
               return;
          }

     }
     else 
     {
     	  if(flag==0)
     	  { 
          strcpy(prof[countfg],argv[0]);         // getting the name of background process

          idnumf[countfg] = pid;
          countfg++;                             		// for the parent:     
          //while (wait(&status) != pid)       		// wait for completion  
          waitpidi = waitpid(pid, &status, WCONTINUED);
       	}

       	   else
       	   {										// if the process is background process
       	   	signal(SIGCHLD, sigchld_handler);		// Signal function
       	   	strcpy(pro[procnum],argv[0]);					// getting the name of background process

            idnum[procnum] = pid;
            procnum++;
       	    printf("%s\n",pro[procnum-1]);
       	   }
     }

     if(inf==0)
     {
     	free(argv);
     }
}