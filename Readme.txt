This shell in C has been made by Mohit Chandra (201502199). 

I have implemented following features:
1. Shell Prompt which shows username and current directory
2. Execute builtin commands
3. Background process with &
4. Custum made commands for echo, pwd, cd
5. System commands with and without arguments 
6. pinfo :​  prints the process related info of your shell program (Bonus).
7. Printing appropriate errors to stderr.
8. User can type the command anywhere in the command line i.e. by giving spaces, tabs etc
9. Modularized code with a make file
10. When any background process ends a message displays (Bonus).
11. Redirection with ">", "<" & ">>" has been implemented.
12. Pipe has been implemented and user can give multiple pipes.
13. Pipe with redirection has been implemented.
14. jobs list all the background process.
15. kjob <jobnumber> <flag> kills the background job with given job number.
16. killallbg kills all background process.
17. fg <jobnumber> foregrounds the process.
18. Ctrl + D kills the shell.
19. Ctrl + C kills the process.
20. Ctrl + Z sends the process in background.

File included are:
1. q.c
2. command_echo.c
3. command_cd.c
4. splitone.c
5. token_space.c
6. tokensemi.c
7. head.h
8. execute.c
9. pip.c
10. pipred.c
11. redirection.c
12. Makefile

To run the shell: 1) Type make clean
				  2) Type ./a.out
