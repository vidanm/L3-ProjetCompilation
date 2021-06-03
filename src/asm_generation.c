#include <stdio.h>
#include "abstract-tree.h"
extern table /* Symbol Table */

void reserveGlobalVarAdress(FILE *file,char* name,int type){
	switch (type){
		case 2:
			fprintf(file,"%s:	resb 1\n",name);
			break;
		case 0:
			break;
		case 1:
			fprintf(file,"%s:	resb 32\n",name);
			break;
		case 3:
			fprintf(file,"%s:	resq 1\n",name);
			break;
		default:
			fprintf(file,"ERROR TOO BIG ALLOCATION\n");
	}
}

void pushLocalOnStack(Node *n){
	/* TODO: Need to add address references in symbol table ?*/
	switch (n->child->u.integer) /* type */ {
		case 1:
			fprintf(file,"push	%d",n->child->sibling->u.integer); /* Wrong definition in
										      syntactic parser */
			break;
		case 2:
			fprintf(file,"push	'%c'",n->child->sibling->u.character);
			break;
		default:
			break;
		
	}
}

void popLocalOnStack(Node *n){
	/* pop from the stack as many times as the size of the local symbol table */
	symbolTable = table;
	for (i = 0; i < symbolTable->current->size; i++){
		fprintf(file,"pop	eax");
	}
}
