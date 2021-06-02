%{
/* tpc-2020-2021.y */
/* Syntaxe du TPC pour le projet d'analyse syntaxique de 2020-2021*/
#include <string.h>
#include <stdio.h>
#include "abstract-tree.h"
#include "symbol-table.h"
#include "build_table.h"
#include "asm_generation.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int yylex();
int yyerror(const char *);
extern int lineno;
extern char yytext[];
extern int charno;
extern char line[200];

Node *AST = NULL;
FILE *file;
%}

/* %define parse.error verbose */

%union value {
    int integer;
    char character;
    char identifier[64];
    Node *node;
};

%token <character>CHARACTER ADDSUB DIVSTAR
%token <integer>NUM
%token <identifier>IDENT
%token <identifier>ORDER EQ
%token <integer>STRUCT SIMPLETYPE
%token <integer>VOID
%token OR AND IF WHILE RETURN PRINT READC READE
%left ')'
%left ELSE
%type <node> SuiteInstr Instr Exp TB FB M E T F LValue
%type <node> Prog TypesVars DeclFoncts Declarateurs Type
%type <node> DeclChamps DeclFonct Arguments ListExp DeclVars
%type <node> Corps ListTypVar Parametres EnTeteFonct

%%
Prog:  TypesVars DeclFoncts { 
			        $$ = makeNode(Program);
    			    if ($1 != NULL) {
    			        addSibling($1,$2);
				        addChild($$,$1);
			        } else{
			      		addChild($$,$2);
                    }
                    AST = $$;
			      }
    ;

TypesVars:
        /* Declarations of global variable (simple and complex type)  */
        TypesVars Type Declarateurs ';' { 
            Node *n = makeNode(GlobeVar);
            addChild(n, $2);
            addChild(n, $3);
            if($1 == NULL){
                $$ = n;
            }else{
                $$ = $1;
                addSibling($$, n);
            } 
        }
        /* Definitions of structure */
    |   TypesVars STRUCT IDENT '{' DeclChamps '}' ';' {
            Node *n = makeNode(DefStruct);
            addChild(n, $5);
            set_identifier(n, $3);
			if($1 == NULL){
                $$ = n;
            }else{
                $$ = $1;
                addSibling($$, n);
            }
        }
    |  /* empty */ { $$ = NULL ; }
    ;

Type:
       SIMPLETYPE  {  $$ = makeNode(TypeSimp); $$->u.integer = $1; }
    | STRUCT IDENT {  $$ = makeNode(TypeStruct); set_identifier($$, $2); }
    ;
Declarateurs:
       Declarateurs ',' IDENT { 
                Node *id = makeNode(Identifier);
                set_identifier(id, $3);
		addSibling($1, id);
                $$ = $1;
		}
    |  IDENT { $$ = makeNode(Identifier);
		set_identifier($$, $1);
		}
    ;

DeclChamps :
       DeclChamps Type Declarateurs ';' {   
                            Node *champ = makeNode(DeclChamp);
                            addChild(champ, $2);
                            addChild(champ, $3); 
                            addSibling($1, champ);
                            $$ = $1;
						}
    
    |  Type Declarateurs ';' { 
            $$ = makeNode(DeclChamp);
            addChild($$, $1);
            addChild($$, $2);
            }
    ;
DeclFoncts:
       DeclFoncts DeclFonct {
            $$ = $1;
			addSibling($$,$2);
        }
    |  DeclFonct { $$ = $1; }
    ;
DeclFonct:
       EnTeteFonct Corps {
            $$ = makeNode(DefFunct);
            addChild($$, $1);
            addChild($$, $2);
        }
    ;

/* The first child is the return type of function (void or any possible Type)
    the second child is the AST node "Parametres",
    name of the function is in u.identifier
*/
EnTeteFonct:
       Type IDENT '(' Parametres ')' {
                    $$ = makeNode(DefFunctHead);
                    addChild($$, $1);
                    set_identifier($$, $2);
				    addChild($$, $4);
                }
    |  VOID IDENT '(' Parametres ')' {
                    $$ = makeNode(DefFunctHead);
                    addChild($$, makeNode(Void));
                    set_identifier($$, $2);
				    addChild($$, $4);
                }
    ;
/* Declaration of parametres of function, in case of void, it's a AST Void node, otherwise
    a AST ParaTypVar Node */
Parametres:
       VOID         { $$ = makeNode(Void); }
    |  ListTypVar   { $$ = $1; }
    |  /* empty */  {$$ = makeNode(Void);}
    ;
/* Parametres of function, non void, linked list of ParaTypVar, for each element, its
    child is a AST "Type" node, and its identifier is the name of parametre */
ListTypVar:
       ListTypVar ',' Type IDENT {
            Node *paraTypVar = makeNode(ParaTypVar);
            addChild(paraTypVar, $3);
            set_identifier(paraTypVar, $4);
			addSibling($1,paraTypVar);
            $$ = $1;
            }
    |  Type IDENT {
            $$ = makeNode(ParaTypVar);
            addChild($$, $1);
            set_identifier($$, $2);
        }
    ;
Corps: '{' DeclVars SuiteInstr '}'      {   
                $$ = makeNode(DefFunctCorps);
                addChild($$, $2);
                addChild($$, $3);
			}
    ;
/* Declarations of local variable of function */
DeclVars:
        DeclVars Type Declarateurs ';' {
            Node *n = makeNode(DeclVar);
            addChild(n, $2);
            addChild(n, $3);
            if ($1 == NULL){
                $$ = n;
            }else{
                $$ = $1;
                addSibling($$, n);
            }
		}
    |  /* empty */ { $$ = NULL; }
    ;
/* May be NULL */
SuiteInstr:
        SuiteInstr Instr { 
            if ($1 != NULL){
                addSibling($1, $2);
                $$ = $1;
            }else{
                $$ = $2;
            }
        }
    |   /* empty */ { $$ = NULL; }
    ;
/* May be NULL */
Instr:
        LValue '=' Exp ';'       {$$ = makeNode(Move); addChild($$, $1); addSibling($1, $3);}
    |   READE '(' LValue ')' ';' {$$ = makeNode(InstrReadE); addChild($$, $3);}
    |   READC '(' LValue ')' ';' {$$ = makeNode(InstrReadC); addChild($$, $3);}
    |   PRINT '(' Exp ')' ';'    {$$ = makeNode(Print); addChild($$, $3);}
    |   IF '(' Exp ')' Instr     {
            $$ = makeNode(InstrIF);
            addChild($$, $3);
            addSibling($3, $5);
        }
    |   IF '(' Exp ')' Instr ELSE Instr  {
            $$ = makeNode(InstrIFELSE);
            addChild($$, $3);
            addSibling($3, $5);
            addSibling($5, $7);
        }
    |   WHILE '(' Exp ')' Instr  {
            $$ = makeNode(InstrWhile);
            addChild($$, $3);
            addSibling($3, $5);
        }
    |   Exp ';'             { $$ = $1; }
    |   RETURN Exp ';'      {
            $$ = makeNode(ReturnValue);
            addChild($$, $2);
        }
    |   RETURN ';'           {$$ = makeNode(ReturnVoid);}
    |   '{' SuiteInstr '}'   {$$ = $2;}
    |   ';'                  {$$ = NULL;}
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
        set_identifier($$, $2);
    	addChild($$,$1);
		addSibling($1,$3);
	}
    |  M

{ $$ = $1; }
    ;

M   :  M ORDER E    {   
        $$=makeNode(Order);
        set_identifier($$, $2);
    	addChild($$,$1);
		addSibling($1,$3);
	}

    |  E        { $$ = $1; }
    ;

E   :  E ADDSUB T   {  
        $$=makeNode(AddSub);
        $$->u.character = $2;
    	addChild($$,$1);
		addSibling($1,$3);
	}
    |  T        { $$ = $1; }
    ;

T   :  T DIVSTAR F  {
        $$=makeNode(DivStar);
        $$->u.character = $2;
    	addChild($$,$1);
		addSibling($1,$3);
		}

    |  F            { $$ = $1; }
    ;

F  :  ADDSUB F      {$$ = makeNode(UnaryAddSub);
                    addChild($$, $2); }

    |  '!' F        {$$ = makeNode(Negation);
                    addChild($$, $2); }

    |  '&' IDENT    { $$ = makeNode(Identifier);
		      strcpy($$->u.identifier,$2); /* pointer */}

    |  '(' Exp ')'  { $$ = $2; }

    |  NUM          {   $$ = makeNode(IntLiteral);
                        $$->u.integer = $1;
                    }

    |  CHARACTER    {
        $$ = makeNode(CharLiteral);
        $$->u.character = $1;
        }

    |  LValue       { $$ = $1; }

    |  IDENT '(' Arguments  ')' {
                $$ = makeNode(Call);
                set_identifier($$, $1);
                addChild($$, $3);
        }
    ;
LValue:
        IDENT            {
            $$ = makeNode(Identifier);
            set_identifier($$, $1);
        }

    |   IDENT '.' IDENT  {;
            $$ = makeNode(Identifier);
            set_identifier($$, $1);
            Node *member = makeNode(Identifier);
            set_identifier(member, $3);
            addChild($$, member);
        }
    ;
/* NULL possible */
Arguments:
       ListExp { $$ = $1; }
    |  /* empty */ { $$ = NULL; }
    ;
ListExp:
       ListExp ',' Exp  { $$ = $1; addSibling($$,$3); }
    |  Exp              { $$ = $1; }
    ;

%%

int yyerror(const char *s) {
	int j;
	printf("%s ", s);
	printf("%d - char %d:\n", lineno, charno);
	printf("%s\n",line);
	for (j=0;j<charno;j++){
		if (line[j] == '\t')
			printf("\t");
		else
			printf(" ");
	}
	printf("^\n");
	return 1;
}

int main(void){
	
	file =  fopen("bss.asm","w+");
	fprintf(file,"section .bss\n");
	if (yyparse() == 1){
	return 1;
	}
	printTree(AST);
	SymbolTable *table = makeTableFromAST(AST);

	ASTtoASM(file,AST); /* ASM File generation */ 
	printSymbolTable(table);

	return 0;
}	
