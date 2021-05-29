// include guard
#ifndef ST_H
#define ST_H

/* max nombre of symbols in a table of a scope */
#define MAX_SYMBOL_NUM 256
/* max name length of a identifier */
#define MAX_NAME_LENGTH 64
/* max nomber of table */
#define MAX_TABLES 128
/* max type number in symbol table */
#define MAX_TYPE_NUMBER 256
/* max member length of struct */
#define MAX_MEMBER_NUMBER 256

#include "abstract-tree.h"
#include "constant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol;

typedef struct symbol_type {
	/* type of type -> void, int, char, struct */
	int type;
	/* name of type -> void, int, char, or name of struct */ 
	char name[MAX_NAME_LENGTH];
	/* in case of struct type */
	struct symbol *member[MAX_MEMBER_NUMBER];
	int memberSize;
} SymbolType;


/* Symbol in table */
typedef struct symbol {
	char identifier[MAX_NAME_LENGTH];
	int type_descriptor;
} Symbol;


/* limit temporary: Each scope has up to 256 symbols */
typedef struct scope {
	Symbol *symbols[MAX_SYMBOL_NUM];
	int size; 		// current size, AKA next index to insert
	struct scope *father;
} Scope;

/* Definition of symbol table */
typedef struct symbol_table{
	Scope *current;
	/* Types defined in the program compiled */
	SymbolType *typeDefined[MAX_TYPE_NUMBER];
	/* size of type defined */
	int type_size;
} SymbolTable;

/**
 * Creat a empty table with three predefined symbolType
 * in the array typeDefined : void, int, char at index 0, 1, 2.
 */ 
SymbolTable *makeEmptySymbolTable();


/**
 * Create a type definition.
 * In case of simple type, put NULL and 0 at last 2 arg.
 * 
 */
SymbolType *makeSymbolType(int type, char *name, Symbol *member[], int memberSize);

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
 * 
 * @return type descriptor or -1 if not found.
 */
int lookupSymbol(SymbolTable *table, char identifier[]);

/**
 * Insert a definition of type to symbol table.
 * 
 * @return type_descriptor (positive),
 * or -1 in case of duplicate definition.
 */
int insertType(SymbolTable *table, SymbolType *type);

/**
 * Print symbol table to standard out
 */
void printSymbolTable(SymbolTable *table);


#define STR_EQUAL(s1, s2) (strcmp(s1,s2) == 0)

#endif
