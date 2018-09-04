/* RML to ISPL Compiler */
/* */

%{
#include "heading.h"
#include "tok.h"
int yyerror(char *s);
extern FILE *coba;
extern struct goals goals;
//int yylineno;
%}

digit		[0-9]
int_const	{digit}+
id              [a-zA-Z][a-zA-Z_0-9]*

%%
"--".*			; /* comment */
"module"        { return MODULE; }
"controls"      { return CONTROL; }
"init"          { return INIT; }
"update"        { return UPDATE; }
"end"           { return END; }
","             { return COMMA; }
"::"            { return DCOL; }
"~>"            { return ASSIGN; }
"true"          { return TRUE; }
"false"         { return FALSE; }
":="            { return EQUALS; }
"'"             { return NEXT; }
";"             { return SEMICOLON; }
"goal"		{ return GOAL; }
"and"		{ return AND; }
"or"		{ return OR; }
"->"		{ return IMPLIES; }
"<->"		{ return IFF; }
"!"		{ return NOT; }
"("		{ return LB; }
"G"		{ return G; }
"F"		{ return F; }
"X"		{ return X; }
"U"		{ return U; }
")"		{ return RB; }
"property"	{ return PROPF; }
{id}            { yylval.sval = strdup(yytext); return ISI;}


[ \t]*		{}
[\n]		{ yylineno++;}

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);}

