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


extern int errno ;
char s[2] = ";";
char del[2] = " ";
int andp = 0,inf=0,ret;
int sizearr,procnum;
char *command,*login,*homedir;
char home[100];
char curr[1000]=":~";
char tempcurr[1000];
char onecom[1000];
char mainco[100];
char pathco[100];
char cwdi[1000];
char filename[1000];
char redbuf[10000];
char **oneco,**new;
struct utsname unm;


void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	printf("\n<%s@%s%s> ",login,unm.nodename,curr);
	//printprompt(hostname, checker, 1);
	fflush(stdout);
}

void sigstopHandler(int sig_num)
{
	int i,index,currpid;
	kill(currpid,20);
	kill(currpid,18);

	for(i=0;i<countfg;i++)
	{
		if(idnumf[i]==currpid)
		{
			index = i;
			break;
		}
	}

	idnum[procnum] = currpid;
	strcpy(pro[procnum],prof[index]);
	procnum++;

}

char *get_command()
{
	char *line = NULL;
	ssize_t bufsize = 0;				// Function for getting the command
	getline(&line, &bufsize, stdin);
	return line;
}

int main(int argc, char *argv[])
{
	int ret,flag=1,len,sp,k,point,fl1,dash,fl2,fl3,m,pid,terin,terout,countjobs=0;
	countfg=0;
	procnum = 0;
	pid_t childpid,pid1;
	int in, out,redflag,pipflag;
	long int i;
	int errnum;

	const char *token, *token1;
	char s[2] = ";";
	homedir = getcwd(cwdi, sizeof(cwdi));

	while(flag==1)
	{
		strcpy(mainco,"./");							// maico array has current directory
		sizearr = 0;
		newsize = 0;
		redir = 0;
		redir2 = 0;
		point = 0;
		redflag = 0;
		pipflag = 0;
		inf = 0;
		erofl = 0;
		uname(&unm);
		login = getenv("USER");							// getting the username
		if(andp==0)
		{
			printf("<%s@%s%s> ",login,unm.nodename,curr);    // To display the username and system name
		}

		andp = 0;
		command = get_command();							// Function for getting the command;

		signal(SIGINT, sigintHandler);				// for ctrcl +c
		
		signal(SIGTSTP, sigstopHandler);			// for ctrl + z

		//signal(SIGSTART, sigstopHandler);


		//signal(SIGTSTP, signalhandle2);


		if(strcmp(command,"\0")==0)
		{
			flag = 0;
			printf("\n");
		}
		else if(strcmp(command,"quit")==10)
		{
			flag = 0;										// If the command entered is quit then quit the terminal

		}


		else
		{

			oneco = tokensemi(command);						// calling the function to tokenise with semicolon as delimiter 
			while(point<sizearr)
			{
				new = split_line(oneco[point]);					//  taking one by one command and calling the function for tokenising with space
				//printf("%d\n",newsize);
				for(i=0;i<newsize;i++)
				{
					if(strcmp(new[i],">")==0 || strcmp(new[i],"<")==0 || strcmp(new[i],">>")==0)
					{
						redflag = 1;
					}

					if(strcmp(new[i],"|")==0)
					{
						pipflag = 1;
					}
				}

				if(strcmp(new[0],"pinfo")==0)
				{

					if(!new[1])
					{
						char spid[1000];
						char *pid = spid;
						char ssnum[1000];
						char *snum = ssnum;
						printf("pid -- %d\n", getpid());
						printf("Process Status -- "); 
						fflush(stdout);
						strcpy(new[0],"ps");
						new[1] = "-o";
						newsize++;
						new[2]="stat=";
						newsize++;
						new[3]="-p";
						newsize++;
						sprintf(snum, "%d", getpid());
						new[4]=snum;
						new[5]=NULL;
						execute(new,newsize);
						printf("Memory --");
						new[2] = "vsz=";
						sprintf(snum, "%d", getpid());
						new[4] = snum;
						execute(new,newsize);
						fflush(stdout);
						new[0]="ps";
						new[1] = "-o";
						new[2]="vsz=";
						new[3]="-p";
						new[4]=pid;
						new[5]=NULL;
						inf = 1;
						execute(new,newsize);
						char commexe[1000], buf[10000];
						strcpy(commexe,"/proc/");
						strcat(commexe, pid);
						strcat(commexe, "/exe");
						readlink(commexe, buf, sizeof(buf)-1);
						printf("Executable Path -- %s\n", buf);
					}
					else if(new[1] && !new[2])
					{
						char spid[1000];
						char *pid = spid;
						strcpy(pid, new[1]);
						printf("pid -- %s\n", pid);
						printf("Process Status -- "); 
						fflush(stdout);
						new[0]="ps";
						new[1] = "-o";
						new[2]="stat=";
						newsize++;
						new[3]="-p";
						newsize++;
						new[4]=pid;
						new[5]=NULL;
						execute(new,newsize);
						printf("Memory -- "); 
						fflush(stdout);
						new[0]="ps";
						new[1] = "-o";
						new[2]="vsz=";
						new[3]="-p";
						new[4]=pid;
						new[5]=NULL;
						inf = 1;
						execute(new,newsize);
						char commexe[1000], buf[10000];
						strcpy(commexe,"/proc/");
						strcat(commexe, pid);
						strcat(commexe, "/exe");
						readlink(commexe, buf, sizeof(buf)-1);
						printf("Executable Path -- %s\n", buf);

					}
				}

				else if(strcmp(new[0],"cd")==0)
				{
					if(new[1]!=NULL && strcmp(new[1],"..")!=0)		
					{

						strcpy(tempcurr,curr);
						strcpy(pathco,new[1]);	
						strcat(mainco,pathco);
						strcat(curr,"/");						// For updating command prompt
						strcat(curr,new[1]);
						chdir(mainco);

					}
					else
					{
						comecd(new,curr);					// calling the cd function 
					}
				}


				else if(strcmp(new[0],"echo")==0 && redflag==0 && pipflag==0)			// if the command is echo then call echo inbuilt command
				{
					comecho(new,newsize);
				}

				else if(strcmp(new[0],"firefox")==0 || strcmp(new[0],"vi")==0)
				{
					if(strcmp(new[0],"firefox")==0)
					{
						strcpy(prof[countfg],new[0]);
						execute(new,newsize);

					}

					else
					{
						strcpy(prof[countfg],new[1]);
						execute(new,newsize);
					}
				}

				else if(strcmp(new[0],"jobs")==0)			// for the jobs command
				{
					countjobs = 1;
					for(i=0;i<procnum;i++)
					{
						if(idnum[i]!=-1)					// checking which background process are active
						{
							printf("[%d] %s [%d]\n",countjobs,pro[i],idnum[i]);
							countjobs++;
						}
					}
				}


				else if(strcmp(new[0],"kjob")==0)			// kjob 								
				{
					int flkj = 0,counting=1;
					if(newsize == 3)									//must have three arguments
					{
						for(i=0;i<procnum;i++)
						{
							//printf("%d\n",idnum[i]);
							if(idnum[i]!=-1)
							{
				
								if(counting==atoi(new[1]))
								{
									kill(idnum[i],atoi(new[2]));
									flkj = 1;
									//idnum[i] = -1;
									break;
								}
								counting++;
								
							}
						}
						if(flkj==0)
						{
							fprintf(stderr,"No such job number\n");
						}
					}
					else
					{
						fprintf(stderr,"Takes Two Arguments!!\n");
					}
				}
				
				else if(strcmp(new[0],"killallbg")==0)							//kill all processses
				{
					for(i=0;i<procnum;i++)
					{
						if(idnum[i]!=-1)
						{
							kill(idnum[i],9);
						}
					}
				}

				else if(strcmp(new[0],"fg")==0)
				{
						int fgfl = 0;
						int counting = 1;
						for(i=0;i<procnum;i++)
						{
							if(counting==atoi(new[1]))
							{
								kill(idnum[i],SIGCONT);
								waitpid(idnum[1],&i,0);
								fgfl = 1;
								break;
							}
							counting++;
						}
							
						if(fgfl==0)
						{
							printf("invalid job no.\n");
						}
				}

				else if(redflag==1 && pipflag==0)
				{
					redirecting(new,newsize);
				}

				else if(pipflag==1 && redflag==0)
				{
					//printf("%d\n",newsize);
					piping(new,newsize);
				}

				else if(pipflag==1 && redflag==1)
				{
					both(new,newsize);
				}


				else
				{
					execute(new,newsize);					// else execute the command
				}
				point++;
			} 
		}

	}


	return 0;
}
