#include <stdio.h>
#include "abstract-tree.h"

void declareASMVar(FILE *file,char* name,int type){
	
	switch (type){
		case 2:
			fprintf(file,"%s:	resb 1\n",name);
			break;
		case 0:
			fprintf(file,"%s:	resw 1\n",name);
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

void handleAffectation(FILE *file,char* name,Node *value){
	
	if (value->kind == IntLiteral)
		 fprintf(file,"mov word [%s], %d\n",name,value->u.integer);
}

void ASTtoASM(FILE *file,Node *n){
	if (n->kind == Move){
		handleAffectation(file,n->firstChild->u.identifier,n->firstChild->nextSibling);
	}
	for (Node *child = n->firstChild; child != NULL;
			child = child->nextSibling){
		ASTtoASM(file,child);
	}
}
