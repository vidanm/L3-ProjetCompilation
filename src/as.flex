%{
#include "as.tab.h"
#include <string.h>
#include <stdlib.h>
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
    {\
	char c;\
	if (strcmp(line,"\0") == 0){\
		c = getchar();\
		linesize = 0;\
		curseur = 0;\
		while ( c != EOF && c != '\n' ){\
			line[linesize] = c;\
			c = getchar();\
			linesize++;\
		}\
		line[linesize] = c;\
		line[linesize+1] = '\0';\
	}\
	c = line[curseur];\
	result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);\
	count++;\
	curseur++;\
}

unsigned int lineno=1;
char line[200];
unsigned int linesize=0;
unsigned int i;
unsigned int curseur=0;
unsigned int count = 0;

%}

%option nounput
%option noinput
%option noyywrap

space " |\t"
type "int"|"char"
ident [a-zA-Z_][a-zA-Z0-9_]*
or "||"
and "&&"
eq "=="
order "<="|"<"|">="|">"
addsub "+"|"-"
divstar "*"|"/"
void "void"
print "print"
num [0-9]+
character \'.\'
return "return"
if "if"
else "else"
while "while"
reade "reade"
readc "readc"
struct "struct"
%x comm
%%

"/*" { BEGIN(comm);}
<comm>"*/" { BEGIN(INITIAL);}
<comm>(.|[ \t\r\n]) { if (yytext[0] == '\n' || yytext[0] == '\r') { lineno++;count=0;linesize=0;strcpy(line,"\0");} }


{type} return TYPE;
{or} return OR;
{and} return AND;
{eq} return EQ;
{order} return ORDER;
{addsub} return ADDSUB;
{divstar} return DIVSTAR;
{void} return VOID;
{print} return PRINT;
{num} yylval.integer = atoi(yytext); return NUM;
{character} yylval.character = yytext[0]; return CHARACTER;
{return} return RETURN;
{if} return IF;
{else} return ELSE;
{while} return WHILE;
{reade} return READE;
{readc} return READC;
{struct} return STRUCT;
{ident} strcpy(yylval.identifier,yytext); return IDENT;
" "|\t|\r|\n { if(yytext[0] == '\n' || yytext[0] == '\r') {lineno++; strcpy(line,"\0");count=0;linesize=0;}
else if (yytext[0] == '\t') count+=4;}
. return yytext[0];
<<EOF>> return 0;
%%
