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

#define EVAL_STR(node, str) strcpy(node->u.identifier, str)

int yylex();
int yyerror(const char *);
extern int lineno;
extern char yytext[];
extern int charno;
extern char line[200];
%}

/* %define parse.error verbose */

%union{
    int integer;
    char character;
    char identifier[64];
    Node *node;
}

%token <character>CHARACTER
%token <integer>NUM
%token <identifier>IDENT
%token SIMPLETYPE
%token ORDER EQ
%token ADDSUB
%token DIVSTAR
%token OR AND STRUCT IF WHILE RETURN VOID PRINT READC READE
%left ')'
%left ELSE
%type <node> SuiteInstr Instr Exp TB FB M E T F LValue
%type <node> Prog TypesVars DeclFoncts Declarateurs Type
%type <node> DeclChamps DeclFonct Arguments ListExp DeclVars
%type <node> Corps ListTypVar Parametres EnTeteFonct

%%
Prog:  TypesVars DeclFoncts { 
    			      $$ = $2;
    			      if ($1 != NULL) { 
    			      addSibling($$,$1);
			      } printTree($$); 
			      }
    |  /* empty */ { $$ = NULL ;}
    ;

TypesVars:
       TypesVars Type Declarateurs ';' { $$ = $3;
					 addSibling($$,$1); }
    |  TypesVars STRUCT IDENT '{' DeclChamps '}' ';' { $$ = makeNode(Struct);
						       addSibling($$,$5); }
    |  /* empty */ { $$ = NULL ; }
    ;
Type:
       SIMPLETYPE  {  $$ = makeNode(Type);  }
    | STRUCT IDENT {  $$ = makeNode(Type); }
    ;
Declarateurs:
       Declarateurs ',' IDENT { $$ = makeNode(Identifier);
				addSibling($$,$1); } 
    |  IDENT { $$ = makeNode(Identifier); }
    ;

DeclChamps :
       DeclChamps SIMPLETYPE Declarateurs ';' { $$ = $3;
						addSibling($$,$1); }
    |  SIMPLETYPE Declarateurs ';' { $$ = $2; }
    |  DeclChamps STRUCT IDENT Declarateurs ';' { $$ = $4;
						  addSibling($$,$1); }
    |  STRUCT IDENT Declarateurs ';' { $$ = $3; }
    ;
DeclFoncts:
       DeclFoncts DeclFonct { $$ = $2;
			      addSibling($$,$1); }
    |  DeclFonct { $$ = $1; }
    ;
DeclFonct:
       EnTeteFonct Corps { $$ = $1;
			   addChild($$,$2); }
    ;
EnTeteFonct:
       Type IDENT '(' Parametres ')' { $$ = makeNode(Identifier);
				       addChild($$,$4);}
    |  VOID IDENT '(' Parametres ')' { $$ = makeNode(Identifier);
				       addChild($$,$4); }
    ;
Parametres:
       VOID { $$ = makeNode(Void); }
    |  ListTypVar { $$ = $1; }
    |  /* empty */ {$$ = makeNode(Void);}
    ;
ListTypVar:
       ListTypVar ',' Type IDENT { $$ = makeNode(Identifier);
				   addSibling($$,$1); }
    |  Type IDENT { $$ = makeNode(Identifier); }
    ;
Corps: '{' DeclVars SuiteInstr '}' { $$ = $3;
                                     if ($2 != NULL)
					addSibling($$,$2); 
				     /* l'erreur de seg vient d'ici  */ } 
    ;
DeclVars:
       DeclVars Type Declarateurs ';' { $$ = $3;
				        addChild($$,$1);}
    |  /* empty */ { $$ = NULL; }
    ;
SuiteInstr:
       SuiteInstr Instr { $$=$2;
			if ($1 != NULL)  
			addSibling($$,$1);}
    |  /* empty */ { $$ = NULL; }
    ;
Instr:
       LValue '=' Exp ';'       {$$ = makeNode(Move); addChild($$, $1); addSibling($1, $3);}
    |  READE '(' LValue ')' ';' {$$ = makeNode(InstrReadE); addChild($$, $3);}
    |  READC '(' LValue ')' ';' {$$ = makeNode(InstrReadC); addChild($$, $3);}
    |  PRINT '(' Exp ')' ';'    {$$ = makeNode(Print); addChild($$, $3);}
    |  IF '(' Exp ')' Instr {   $$ = makeNode(InstrIF);
                                        addChild($$, $3);
                                        addSibling($3, $5);
                            }
    |  IF '(' Exp ')' Instr ELSE Instr  {
            $$ = makeNode(InstrIFELSE);
            addChild($$, $3);
            addSibling($3, $5);
            addSibling($5, $7);
        }
    |  WHILE '(' Exp ')' Instr  {
        $$ = makeNode(InstrWhile);
        addChild($$, $3);
        addSibling($3, $5);
        }
    |  Exp ';'          {$$ = $1; }
    |  RETURN Exp ';'       {
            $$ = makeNode(ReturnValue);
            addChild($$, $2);
        }
    |  RETURN ';'           {$$ = makeNode(ReturnVoid);}
    |  '{' SuiteInstr '}'   {$$ = $2;}
    |  ';'                  {$$ = makeNode(EmptyInstr);}
    ;
Exp :  Exp OR TB    {   
        $$=makeNode(Or);
        addChild($$,$1);
	    addSibling($1,$3);
	}
    |  TB           { $$ = $1; }
    ;

TB  :  TB AND FB    {   
        $$=makeNode(And);
    	addChild($$,$1);
        addSibling($1,$3);
		}
    |  FB       { $$ = $1; }
    ;
FB  :  FB EQ M      {   
        $$=makeNode(Eq);
    	addChild($$,$1);
		addSibling($1,$3);
	}
    |  M        { $$ = $1; }
    ;

M   :  M ORDER E    {   
        $$=makeNode(Order);
    	addChild($$,$1);
		addSibling($1,$3);
	}

    |  E        { $$ = $1; }
    ;

E   :  E ADDSUB T   {  
        $$=makeNode(AddSub);
    	addChild($$,$1);
		addSibling($1,$3);
	}
    |  T        { $$ = $1; }
    ;

T   :  T DIVSTAR F  {
        $$=makeNode(DivStar);
    	addChild($$,$1);
		addSibling($1,$3);
		}

    |  F            { $$ = $1; }
    ;

F  :  ADDSUB F      {$$ = makeNode(UnaryAddSub);
                    addChild($$, $2); }

    |  '!' F        {$$ = makeNode(Negation);
                    addChild($$, $2); }

    |  '&' IDENT    { /* pointer */}

    |  '(' Exp ')'  { $$ = $2; }

    |  NUM          {   $$ = makeNode(IntLiteral);
                        $$->u.integer = $1;
                    }

    |  CHARACTER    {
        $$ = makeNode(CharLiteral);
        $$->u.character = $1;
        }

    |  LValue       { $$ = $1; }

    |  IDENT '(' Arguments  ')' { $$ = makeNode(Identifier);
				  strcpy($$->u.identifier,$1);}
    ;
LValue:
       IDENT            {
           $$ = makeNode(Identifier);
           strcpy($$->u.identifier,$1);
        }

    |  IDENT '.' IDENT  {
        /*$$ = makeNode(StructIdentifier);
        Node left = makeNode(Identifier);
        EVAL_STR(left, $1);
        Node right = makeNode(Identifier);
        EVAL_STR(right, $3);*/
        }
    ;
Arguments:
       ListExp { $$ = $1; }
    |  /* empty */ { $$ = NULL; }
    ;
ListExp:
       ListExp ',' Exp {$$ = $3; addSibling($$,$1);}
    |  Exp {$$ = $1;}
    ;

%%

int yyerror(const char *s) {
	int j;
	printf("%s ", s);
	printf("%d - char %d:\n", lineno, charno);
	printf("%s\n",line);
	for (j=0;j<charno;j++)
		printf(" ");
	printf("^\n");
	return 1;
}

int main(void){
	if (yyparse() == 1){
		return 1;
	}
	return 0;
}	