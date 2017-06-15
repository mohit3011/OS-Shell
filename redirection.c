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





void redirecting(char **new, int newsize)
{

	int flfor=0, flrev=0 , fldfor=0,indexfor,indexrev,indexdfor,in,out,terout,terin,i,cmderr=0;
	char filename1[1000], filename2[1000];

	for(i=0;i<newsize;i++)
	{
		if(strcmp(new[i],">")==0)
		{
			flfor = 1;					// Checking for >
			indexfor = i;
		}

		else if(strcmp(new[i],"<")==0)
		{
			flrev = 1;					// checking for  < 
			indexrev = i;
		}

		else if(strcmp(new[i],">>")==0)
		{
			fldfor = 1;
			indexdfor = i;				// checking >>
		}

	}

	if((flfor==1 && flrev==1 && indexfor<indexrev) || (flfor==1 && indexfor==newsize-1) || (flfor==1 && indexfor==0) || (flrev==1 && indexrev==newsize-1) || (fldfor==1 && indexdfor==newsize-1) || (flrev==1 && indexrev==0))
	{
        fprintf(stderr, "Please use the command in correct manner\n");		

		cmderr = 1;											// Error Printing
	}

	if(flfor==1 && flrev==0 && cmderr==0)
	{
		if(indexfor >= newsize-1)
		{
          fprintf(stderr, "Please Provide A filename\n");		
		}
		else
		{
			strcpy(filename1,new[indexfor+1]);
			new[indexfor] = NULL;
			newsize = indexfor;
		    in = open(filename1,O_WRONLY | O_TRUNC | O_CREAT, 0644);			// Opening the file for ">"
		    if(in<0)
		    {
				fprintf(stderr, "Error in opening the file\n");	
		    }
		    terin = dup(1);
		    close(1);												// Closing stdout
			dup2(in,1);
			close(in);												// duplicating
			if(strcmp(new[0],"echo")==0)
			{
				comecho(new,newsize);							// If the command is echo	
			}

			else
			{
				execute(new,newsize);						// For other commands
			}
			dup2(terin,1);
		}
	}

	if(flrev==1 && flfor==0 && fldfor==0 && cmderr==0)
	{
		if(indexrev >= newsize-1)
		{
          fprintf(stderr, "Please Provide A filename\n");		
		}
		else
		{
			strcpy(filename1,new[indexrev+1]);
			new[indexrev] = NULL;
			newsize = indexrev;
		    out = open(filename1,O_RDONLY);						// Opening the file in <
		    if(out<0)
		    {
				fprintf(stderr, "Error in opening the file\n");	
		    }
		    terout = dup(0);
		    close(0);											// closing stdin
			dup2(out,0);
			close(out);
			if(strcmp(new[0],"echo")==0)
			{
				comecho(new,newsize);							// executing echo
			}
			else
			{
				execute(new,newsize);							// executing other commands
			}
			dup2(terout,0);										// restoring stdin
		}
	}

	if(fldfor==1 && flrev==0 && cmderr==0)
	{
		if(indexdfor >= newsize-1)
		{
          fprintf(stderr, "Please Provide A filename\n");		
		}
		else
		{
			strcpy(filename1,new[indexdfor+1]);
			new[indexdfor] = NULL;
			newsize = indexdfor;
		    in = open(filename1,O_WRONLY | O_APPEND);		// Opening the file for append
		    if(in<0)
		    {
				fprintf(stderr, "Error in opening the file\n");
		    }
		    terin = dup(1);
		    close(1);
			dup2(in,1);
			close(in);
			if(strcmp(new[0],"echo")==0)
			{
				comecho(new,newsize);	
			}
			else
			{
				execute(new,newsize);				// execute the command
			}
			dup2(terin,1);
		}
	}

	if(flfor==1 && flrev==1 && cmderr==0)
	{
		if(indexfor >= newsize-1 || indexrev >= newsize-1)
		{
          fprintf(stderr, "Error\n");		
		}

		else
		{
			strcpy(filename2,new[indexfor+1]);
			strcpy(filename1,new[indexrev+1]);
			new[indexrev] = NULL;
			newsize = indexrev;
	        out = open(filename1,O_RDONLY);
		    in = open(filename2,O_WRONLY | O_TRUNC | O_CREAT, 0644);
	        if(in<0 || out<0)
	        {
	    		fprintf(stderr, "Error in opening the file\n");	
	        }
	        terout = dup(0);
	        terin = dup(1);
	        close(0);
	        close(1);
	    	dup2(out,0);
	    	dup2(in,1);
	    	close(out);
	    	close(in);
	    	if(strcmp(new[0],"echo")==0)
	    	{
	    		comecho(new,newsize);	
	    	}
	    	else
	    	{
	    		execute(new,newsize);
	    	}
	    	dup2(terout,0);
	    	dup2(terin,1);
		}
	}



	if(fldfor==1 && flrev==1 && cmderr==0)
	{
		if(indexdfor >= newsize-1 || indexrev >= newsize-1)
		{
          fprintf(stderr, "Error\n");		
		}

		else
		{
			strcpy(filename2,new[indexdfor+1]);
			strcpy(filename1,new[indexrev+1]);
			new[indexrev] = NULL;
			newsize = indexrev;
	        out = open(filename1,O_RDONLY);
		    in = open(filename2,O_WRONLY | O_APPEND);
	        if(in<0)
	        {
	    		fprintf(stderr, "Error in opening the file\n");	
	        }
	        terout = dup(0);
	        terin = dup(1);
	        close(0);
	        close(1);
	    	dup2(out,0);
	    	dup2(in,1);
	    	close(out);
	    	close(in);
	    	if(strcmp(new[0],"echo")==0)
	    	{
	    		comecho(new,newsize);	
	    	}
	    	else
	    	{
	    		execute(new,newsize);
	    	}
	    	dup2(terout,0);
	    	dup2(terin,1);
		}
	}
		
}
