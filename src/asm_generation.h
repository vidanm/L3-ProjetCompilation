#include <stdio.h>
#include "abstract-tree.h"
#include "symbol-table.h"

void declareASMVar(FILE *file,char* name,int type);
void handleAffectation(FILE *file,char* name,int num);
void ASTtoASM(FILE *file,Node *n);
