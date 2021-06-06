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
extern int actual_stack_size;

typedef struct symbol_type {
	/* type of type -> void 0, int 1, char 2, struct 3, function 4*/
	int type;
	/* name of type -> void, int, char, name of struct, name of function */ 
	char name[MAX_NAME_LENGTH];
	/**
	 * in case of struct type, the members will be here
	 * in case of function,first element will be return type, the rest are parametres
	 *  */
	struct symbol *member[MAX_MEMBER_NUMBER];
	/** 
	 * in case of struct, member size
	 * in case of function, parametre size, with out return type
	 */
	int memberSize;
} SymbolType;


/* Symbol in table */
typedef struct symbol {
	char identifier[MAX_NAME_LENGTH];
	int type_descriptor;
	int stack_address;
} Symbol;


/* limit temporary: Each scope has up to 256 symbols */
typedef struct scope {
	Symbol *symbols[MAX_SYMBOL_NUM];
	int size; 		// current size, AKA next index to insert
	struct scope *father;
	char name[256];
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
 * the name will be copied by strcpy
 * In case of simple type, put NULL and 0 in the last 2 arg.
 * 
 */
SymbolType *makeSymbolType(int type, char *name, Symbol *member[], int memberSize);

/**
 * Insert a definition of type to symbol table.
 * 
 * @return type_descriptor (positive),
 * or -1 in case of duplicate definition.
 */
int insertType(SymbolTable *table, SymbolType *type);

int hasType(SymbolTable *table, SymbolType *type);


/** make a symbol */
Symbol *makeSymbol(char *identifier, int type_descriptor);

/**
 * Insert a symbol to the current scope of the symbole table.
 * 
 * @param table table to insert the symbol
 * @param identifier identifier of the symbol
 * @param type_descriptor type descriptor of the symbol
 */
void insertSymbol(SymbolTable *table, char identifier[], int type_descriptor);

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
 * Add a scope in the symbol table
 */
void pushScope(SymbolTable *table, char *name);

/** 
 * Remove current scope from the symbol table 
 */
void popScope(SymbolTable *table);

int equalSymbolType(SymbolType *st1, SymbolType *st2);

int getSymbolAddress(SymbolTable *table, char identifier[]);

#define STR_EQUAL(s1, s2) (strcmp(s1,s2) == 0)

#endif
