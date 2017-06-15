compile: q.o execute.o command_cd.o command_echo.o token_space.o tokensemi.o splitone.o redirection.o pip.o pipred.o
	gcc q.o execute.o command_cd.o command_echo.o token_space.o tokensemi.o splitone.o redirection.o pip.o pipred.o

q.o: q.c
	gcc -c q.c

execute.o: execute.c
	gcc -c execute.c

command_cd.o: command_cd.c
	gcc -c command_cd.c

command_echo.o: command_echo.c
	gcc -c command_echo.c

token_space.o: token_space.c
	gcc -c token_space.c

tokensemi.o: tokensemi.c
	gcc -c tokensemi.c

splitone.o: splitone.c
	gcc -c splitone.c

redirection.o: redirection.c
	gcc -c redirection.c

pip.o: pip.c
	gcc -c pip.c

pipred.o: pipred.c
	gcc -c pipred.c

clean: q.o execute.o command_cd.o command_echo.o token_space.o tokensemi.o splitone.o redirection.o pip.o pipred.o
	rm *.o
