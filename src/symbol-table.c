#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-tree.h"
#include "symbol-table.h"

static const char *StringFromType[] = {
	"void",
	"int",
	"char",
	"struct"
};

STentry* symbolTable[MAXTABLES];

int STmax = MAXSYMBOLS; 
int STSize[MAXTABLES]; /* Nombres de symboles dans les tables */
int STNumber = 0; /* Nombre de tables */
int current_type = 0;

void appendST(){
	STentry *table;
	table = (STentry *) malloc(MAXSYMBOLS*sizeof(STentry));
	symbolTable[STNumber] = table;
	STSize[STNumber] = 0;
	STNumber += 1;
}

void popST(){
	symbolTable[STNumber] = NULL;
	STSize[STNumber] = 0;
	STNumber -= 1;
}

void addVar(const char name[], int type)
{

	int i,j;
	for (i = 0; i < STNumber; i++){
		for (j = 0;j < STSize[STNumber]; j++){
			
			if (!strcmp(symbolTable[i][j].name, name))
			{
				printf("redefinition de la variable %s pres de la ligne\n", name);
				return;
			}
		}
	}

	if (STNumber == 0){
		appendST();
	}

	if (++(STSize[STNumber-1]) > STmax)
	{
		printf("too many variables near line \n");
		exit(1);
	}

	strcpy(symbolTable[STNumber-1][(STSize[STNumber-1])-1].name,name);
	symbolTable[STNumber-1][(STSize[STNumber-1])-1].type = type;
}

void createTable(Node *node){
	while (node != NULL){
		switch (node -> kind){
			case VarDeclaration: addVar(node->u.identifier,current_type);break;
			case Type: current_type = node->u.integer;break;
			case ReturnType: current_type = node->u.integer;break;
			case Parameter: appendST(); break;
			case EndFunc: printTable(); popST(); break;
			default:break;
		}
		
		if (node -> firstChild != NULL)
			createTable(node->firstChild);

		node = node->nextSibling;
	}
}

void isInTable(Node *node)
{
	/* Ne vérifie que la présence des variables dans la table de symboles de var globales (0)*/
	int i,j,declared=0;
	while (node != NULL){
		switch (node -> kind){
			case Identifier:
				for (i = 0; i < STNumber; i++){
					for (j = 0;j < STSize[i]; j++){
						if (!strcmp(symbolTable[i][j].name, node->u.identifier))
						{
							declared = 1;
							break;
						}
					}
				}
				
				if (!declared)
					printf("\x1b[31mUtilisation de la variable %s sans l'avoir déclaré\x1b[0m \n", node->u.identifier);
				break;
			default:
				break;
		}
		declared = 0;
		if (node -> firstChild != NULL)
			isInTable(node->firstChild);

		node = node->nextSibling;
	}
}

void printTable(){
	int i,j;
	printf("\x1b[34mTABLE DES SYMBOLES\n=============\x1b[0m\n");
	for (i = 0;i < STNumber; i++){
		if (i == 0)
			printf("TABLE GLOBALE \n");
		else
			printf("TABLE LOCALE %d \n",i);
		for (j = 0; j < STSize[i]; j++){
			printf("%s : %s\n",
				StringFromType[symbolTable[i][j].type],
				symbolTable[i][j].name
			      );
		}
		printf("\n");
	}
	printf("\n\n");
}

