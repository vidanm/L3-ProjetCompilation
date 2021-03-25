%{
/* tpc-2020-2021.y */
/* Syntaxe du TPC pour le projet d'analyse syntaxique de 2020-2021*/
#include <string.h>
#include <stdio.h>
#include "abstract-tree.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
int yylex();
int yyerror(char *);
extern int lineno;
extern char yytext[];
extern int count;
extern int linesize;
extern char line[200];
int err=0;
%}

%union value {
	Node *node;
	char character;
	int integer;
	char identifier[64];
}

%token TYPE
%token <character> CHARACTER
%token <integer> NUM
%token <identifier> IDENT
%token ORDER
%token DIVSTAR
%token ADDSUB
%token EQ
%token AND
%token OR
%token RETURN
%token WHILE
%token ELSE
%token IF
%token PRINT
%token VOID
%token READE
%token READC
%token STRUCT
%start Prog

%type <node> F
%type <node> Exp
%type <node> LValue
%type <node> TB
%type <node> FB
%type <node> M
%type <node> E
%type <node> T

%%
Prog:  DeclGlob DeclFoncts
    |
    ;

DeclGlob: DeclGlob DeclStruct
	   | DeclGlob Declarateurs ';'
	   | error ';' { yyerrok; err = 1; }
	   |
	  ;

DeclStruct: STRUCT IDENT '{' StructBody '}' ';'
	  ;

StructBody: DeclVars
	  ;

DeclVars:
       DeclVars Declarateurs ';' 
    |  Declarateurs ';'
    ;

Declarateurs:
       Declarateurs ',' IDENT 
    | Type IDENT 
    ;

DeclFoncts:
       DeclFoncts DeclFonct 
    |  DeclFonct 
    ;

DeclFonct:
       EnTeteFonct Corps
    ;
EnTeteFonct:
       Type IDENT Parametres
    |  VOID IDENT Parametres
    ;
Parametres:
       '(' VOID ')' 
    |  '(' ListTypVar ')'
    |	'(' error ')' { yyerrok; err=1; }
    | '('')'
    ;
ListTypVar:
       ListTypVar ',' Type IDENT 
    |  Type IDENT 
    ;
Corps: '{' DeclVars SuiteInstr '}'
     | '{' SuiteInstr '}'
    ;
Type: TYPE
    | STRUCT IDENT
    ;
SuiteInstr:
       SuiteInstr Instr 
    |
    ;
Instr:
       LValue '=' Exp ';'
    |  READE '(' IDENT ')' ';'
    |  READC '(' IDENT ')' ';'
    |  PRINT '(' Exp ')' ';'
    |  IF '(' Exp ')' Instr ';'
    |  IF '(' Exp ')' Instr ELSE Instr ';'
    |  WHILE '(' Exp ')' Instr ';'
    |  IDENT '(' Arguments  ')' ';'
    |  RETURN Exp ';' 
    |  RETURN ';' 
    |  '{' SuiteInstr '}' 
    |  ';' 
    ;
Exp :  Exp OR TB 
    |  TB 
    ;
TB  :  TB AND FB 
    |  FB 
    ;
FB  :  FB EQ M
    |  M
    ;
M   :  M ORDER E 
    |  E 
    ;
E   :  E ADDSUB T 
    |  T 
    ;    
T   :  T DIVSTAR F 
    |  F 
    ;
F   :  ADDSUB F { $$=makeNode(UnaryAddSub);
    		  addChild($$,$2); }

    |  '!' F { Node *n = makeNode(F);
	       $$ = makeNode(F);
	       addChild($$,n); }

    |  '(' Exp ')' { $$ = $2; }

    |  NUM { $$ = makeNode(IntLiteral);
	     $$->u.integer = $1;}

    |  CHARACTER { $$ = makeNode(CharLiteral);
		   $$->u.character = $1;}

    |  LValue { $$ = $1 ;}

    |  IDENT '(' Arguments  ')' 
    ;
LValue:
       IDENT 
    ;
Arguments:
       ListExp 
    |
    ;
ListExp:
       ListExp ',' Exp 
    |  Exp 
    ;
%%

int yyerror(char *s) {
	int j;
	printf("Syntax Error line ");
	printf("%d - char %d:\n",lineno,count);
	printf("%s\n",line);
	for (j=0;j<count;j++)
		printf(" ");
	printf("^\n");
	return 1;
}

int main(void){
	strcpy(line,"\0");
	linesize = 0;
	if (yyparse() == 1){
		return 1;
	}
	if (err)
		return 1;
	return 0;
}	
