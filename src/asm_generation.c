#include <stdio.h>
#include "abstract-tree.h"
#include "symbol-table.h"
extern FILE * file;

void reserveGlobalVarAdress(FILE *file,char* name,int type){
	switch (type){
		case 2:
			fprintf(file,"%s:	resq 1\n",name);
			break;
		case 0:
			break;
		case 1:
			fprintf(file,"%s:	resq 1\n",name);
			break;
		case 3:
			fprintf(file,"%s:	resq 1\n",name);
			break;
		default:
			fprintf(file,"ERROR TOO BIG ALLOCATION\n");
	}
}

void popLocalOnStack(SymbolTable * table,Node *n){
	/* pop from the stack as many times as the size of the local symbol table */
	SymbolTable * symbolTable = table;
	int i;
	for (i = 0; i < symbolTable->current->size; i++){
		fprintf(file,"pop	eax");
	}
}
