// include guard
#ifndef ST_H
#define ST_H

/* max nombre of symbols in a table of a scope */
#define MAX_SYMBOL_NUM 256
/* max name length of a identifier */
#define MAX_NAME_LENGTH 64
/* max nomber of table */
#define MAX_TABLES 128

#define TYPE_VOID 0
#define TYPE_INT 1
#define TYPE_CHAR 2
#define TYPE_STRUCT 3

#include "abstract-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* symbol in table */
typedef struct symbol {
	char identifier[MAX_NAME_LENGTH];
	int type;
} Symbol;

/* limit temporary: Each scope has up to 256 symbols */
typedef struct scope {
	Symbol *symbols[MAX_SYMBOL_NUM];
	int size; 		// AKA next index to insert
	struct scope *father;
} Scope;

typedef struct symbol_table{
	Scope *current;
} SymbolTable;

void pushScope(SymbolTable *table);

void popScope(SymbolTable *table);

void insertSymbol(SymbolTable *table, char identifier[], int type);

int lookupSymbol(SymbolTable *table, char identifier[]);

void printSymbolTable(SymbolTable *table);

SymbolTable *makeTableFromAST(Node *tree);

#define STR_EQUAL(s1, s2) (strcmp(s1,s2) == 0)



// /**
//  * Symbole table is global variable, which is list of pointer
//  * that indicates to sub-tables in each scope.
//  */
// STentry *symbolTable[MAX_TABLES];

// /* add an entry in the table of current scope */
// void addVar(const char name[],int type);

// /* print table */
// void printTable();

// /* create a symbole table from a AST */
// void createTable(Node *node);

// /* check if the name is already defined in the symbole table */
// void isInTable(const char name[]);

#endif
