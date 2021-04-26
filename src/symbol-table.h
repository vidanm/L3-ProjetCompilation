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

/* Symbol in table */
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

/* Definition of symbol table */
typedef struct symbol_table{
	Scope *current;
} SymbolTable;

/**
 * Creat a empty table.
 */
SymbolTable *makeEmptySymbolTable();

/**
 * Add a scope in the symbol table
 */
void pushScope(SymbolTable *table);

/** 
 * Remove current scope from the symbol table 
 */
void popScope(SymbolTable *table);

/**
 * Insert a symbol to the current scope of the symbole table.
 * 
 * @param table table to insert the symbol
 * @param identifier identifier of the symbol
 * @param type type of the symbol
 */
void insertSymbol(SymbolTable *table, char identifier[], int type);

/**
 * Check the type of the symbol indicated by its identifier.
 * 
 * @param table table where to check symbol
 * @param identifier identifier of the symbol
 */
int lookupSymbol(SymbolTable *table, char identifier[]);

/**
 * Print symbol table to standard out
 */
void printSymbolTable(SymbolTable *table);


#define STR_EQUAL(s1, s2) (strcmp(s1,s2) == 0)

#endif
