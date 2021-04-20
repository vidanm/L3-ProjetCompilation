%{
#include "abstract-tree.h"
#include "syntactic_parser.h"
#include "symbol-table.h"
#include <string.h>
#include <stdlib.h>
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) record_input(buf, &result, max_size)
#define TRUE 1
#define FALSE 0

void record_input(char buf[], int *result, int max_size);

/* line container */
char line[200];

/* flag of end of line, it's true by defaut to read the first line */
int is_EOL = TRUE;

/* current position of cursur */
unsigned int cursur=0;

/* Current line number */
unsigned int lineno=1;

/* number of characters in the current line */
unsigned int charno=0;

%}

%option noinput nounput noyywrap
%x SHORT_COMMENT LONG_COMMENT

%%

[ \t\r]+        { charno += yyleng;}
\n		    	{ charno=0; lineno++; is_EOL = TRUE;}
"/*"	    	{ charno += yyleng;BEGIN LONG_COMMENT; }
"//"	    	{ charno += yyleng;BEGIN SHORT_COMMENT; }
&&		    	{ charno += yyleng;return AND; }
"||"			{ charno += yyleng;return OR; }
"+"|-			{ charno += yyleng; return ADDSUB; }
"*"|"/"|"%"	        { charno += yyleng;return DIVSTAR; }
"<"|"<="|">"|>=		{ charno += yyleng;return ORDER; }
==|!=			{ charno += yyleng;return EQ; }
int			    { charno += yyleng;
			      yylval.integer = 0;	      
			      return SIMPLETYPE;
			      }
char		    { charno += yyleng;
			yylval.integer = 1;
			return SIMPLETYPE;}
print			{ charno += yyleng;return PRINT; }
readc			{ charno += yyleng;return READC; }
reade			{ charno += yyleng;return READE; }
void			{ charno += yyleng;return VOID; }
struct		    { charno += yyleng;yylval.integer = 2; return STRUCT;}
if			    { charno += yyleng;return IF; }
else			{ charno += yyleng;return ELSE; }
while			{ charno += yyleng;return WHILE; }
return		    { charno += yyleng;return RETURN; }
[a-zA-Z_][a-zA-Z0-9_]*  { charno += yyleng; 
			   strcpy(yylval.identifier,yytext); 
			   return IDENT; }
[0-9]+			        { charno += yyleng; 
				  yylval.integer = atoi(yytext);
				  return NUM;}
'\\?.'			        { charno += yyleng; yylval.character = yytext[0]; return CHARACTER; }
<<EOF>> 				return 0;
.			            { charno += yyleng; return yytext[0];}

<LONG_COMMENT>"*/"		{ BEGIN INITIAL; charno += yyleng; }
<LONG_COMMENT,SHORT_COMMENT>.		{charno += yyleng;}
<LONG_COMMENT>\n		{lineno++;charno=0; is_EOL = TRUE;}
<SHORT_COMMENT>\n		{BEGIN INITIAL; lineno++;charno=0;is_EOL = TRUE;}
%%

/**
 * Fill the char array with characters from stdin until "EOF" or "\n",
 * including the "EOF" or the '\n' in the end, terminated
 * the char array with '\0';
 */
void fill_line(char line[]){
	int i = 0;
	char c;
	while(TRUE){
		c = getchar();
		line[i] = c;
		i++;
		if(c == EOF || c == '\n'){
			line[i] = '\0';
			break;
		}
	}
}

/**
 * The function that replaces the yy_input(),
 * it will supply the lexical parser a character in
 * the current line at each call by put the character in the buf[0],
 * set (*result) the number of character read (1), and move the cursur
 * to next character.
 * 
 * When reaching the end of current line which is indicated
 * by a flag, it will reset the cursur to the 0 position, fill the line container
 * with the content of new line, reset the new line flag to false.
 * In case of end of file, it will set (*result) the YY_NULL.
 *  
 * Attention, this function uses multiple global variables,
 * is_EOF: the flag of reaching the end of line.
 * cursur: the cursur
 * line: the line container
 */
void record_input(char buf[], int *result, int max_size){
	char c;
	if (is_EOL){
		cursur = 0;
		fill_line(line);
		is_EOL = FALSE;
	}
	c = line[cursur];
	// printf("char position: %d, char now is %c, ansic code: %d\n",charno, c, c);
	*result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);
	cursur++;
}
