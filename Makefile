# Makefile

OBJS	= bison.o lex.o main.o print.o evaluator.o enum_com.o builder.o inserter.o

CC	= g++
CFLAGS	= -g -Wall -Wno-write-strings -Wno-unused-function -ansi -pedantic

#default: all

sevia:			$(OBJS) 
			$(CC) $(CFLAGS) $(OBJS) -o sevia -ll

lex.o:			lex.c 
			$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:			stic.lex 
			flex -l stic.lex
			cp lex.yy.c lex.c

bison.o:		bison.c 
			$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:		stic.y heading.h
			bison -d -v stic.y
			cp stic.tab.c bison.c
			cmp -s stic.tab.h tok.h || cp stic.tab.h tok.h

main.o:			main.cc 
			$(CC) $(CFLAGS) -c main.cc -o main.o

print.o:		print.cc 
			$(CC) $(CFLAGS) -c print.cc -o print.o

evaluator.o:	evaluator.cc 
			$(CC) $(CFLAGS) -c evaluator.cc -o evaluator.o

enum_com.o:	enum_com.cc 
			$(CC) $(CFLAGS) -c enum_com.cc -o enum_com.o

builder.o:		builder.cc 
			$(CC) $(CFLAGS) -c builder.cc -o builder.o

inserter.o:	inserter.cc 
			$(CC) $(CFLAGS) -c inserter.cc -o inserter.o

lex.o yac.o main.o print.o evaluator.o enum_com.o builder.o inserter.o	: heading.h
lex.o main.o		: tok.h

clean:
	rm -f *.o *~ lex.c lex.yy.c bison.c tok.h stic.tab.c stic.tab.h stic.output sevia

clean-out:
	rm -f output/*.ispl
