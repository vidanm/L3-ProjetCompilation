#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-tree.h"
#include "symbol-table.h"

STentry *symbolTable;
int STmax = MAXSYMBOLS;
int STsize = 0;

void addVar(const char name[], int type)
{
	int count;
	for (count = 0; count < STsize; count++){
		if (!strcmp(symbolTable[count].name, name))
		{
			printf("redefinition de la variable %s pres de la ligne", name);
			return;
		}
	}
	if (++STsize > STmax)
	{
		printf("too many variables near line \n");
		exit(1);
	}
	strcpy(symbolTable[STsize-1].name, name);
	symbolTable[STsize-1].type = type;
}

void printTable(){
	int count;
	printf("TABLE DES SYMBOLES\n------------\n");
	for (count = 0; count < STsize; count++)
		printf("%s : %d\n",
			symbolTable[count].name,
			symbolTable[count].type
		      );
}

void createTable(Node *node){
	switch(node -> kind){
		case IntLiteral: addVar(node->u.identifier,
					node->kind);
				 break;
		case CharLiteral: addVar(node->u.identifier,
					 node->kind);
				  break;
		case Identifier: addVar(node->u.identifier,
					node->kind);
				 break;
		default: break;
	}
	for (Node *child = node->firstChild; child != NULL; child = child-> nextSibling) {
		createTable(child);
	}
}
