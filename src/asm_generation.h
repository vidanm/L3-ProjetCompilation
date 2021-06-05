#include <stdio.h>
#include "abstract-tree.h"
#include "symbol-table.h"

void reserveGlobalVarAdress(FILE *file,char* name,int type); /* Reserve the space in BSS Section 
								for a global variable */
void pushLocalOnStack(Node *n); /* Push all the local variables onto the stack */
void popLocalOnStack(SymbolTable * table,Node *n); /* Pop all the local variables off the stack */
