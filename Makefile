all: msh
msh: myShellPrgm.o lex.yy.o
	gcc -o msh myShellPrgm.o lex.yy.o -ll
myShellPrgm.o: myShellPrgm.c
	gcc -Wall -g -c myShellPrgm.c
lex.yy.o: lex.yy.c
	gcc -Wall -g -c lex.yy.c
lex.yy.c: lex.c
	flex lex.c
clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f msh
	