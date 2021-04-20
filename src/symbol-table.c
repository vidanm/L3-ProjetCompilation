#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-tree.h"
#include "symbol-table.h"

static const char *StringFromType[] = {
	"int",
	"char",
	"struct"
};

STentry symbolTable[MAXSYMBOLS];
int STmax = MAXSYMBOLS;
int STsize = 0;
int current_type = 0;

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

void createTable(Node *node){

	while (node != NULL){
		switch (node -> kind){
			case VarDeclaration: addVar(node->u.identifier,current_type);break;
			case Type: current_type = node->u.integer;break;
			default:break;
		}
		
		if (node -> firstChild != NULL)
			createTable(node->firstChild);

		node = node->nextSibling;
	}
}

void isInTable(Node *node)
{
	int count;
	while (node != NULL){
		switch (node -> kind){
			case Identifier:
				for (count = 0; count < STsize; count++){
					if (!strcmp(symbolTable[count].name, node->u.identifier))
					{
						break;
					}
				}
				if (count > STsize -1)
					printf("La variable %s n'a pas été déclaré\n", node->u.identifier);
				break;
			default:
				break;
		}
		if (node -> firstChild != NULL)
			isInTable(node->firstChild);

		node = node->nextSibling;
	}
}

void printTable(){
	int count;
	printf("TABLE DES SYMBOLES\n------------\n");
	for (count = 0; count < STsize; count++)
		printf("%s : %s\n",
			StringFromType[symbolTable[count].type],
			symbolTable[count].name
		      );
}

