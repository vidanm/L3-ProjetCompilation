// include guard
#ifndef ST_H
#define ST_H

/* max nombre of symbols in a table of a scope */
#define MAX_SYMBOLS 256
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

#define STR_EQUAL(s1, s2) (strcpy(s1,s2) == 0)

/* entry in table */
typedef struct {
	char name[MAX_NAME_LENGTH];
	int type;
} STentry;

/**
 * Symbole table is global variable, which is list of pointer
 * that indicates to sub-tables in each scope.
 */
STentry *symbolTable[MAX_TABLES];

/* add an entry in the table of current scope */
void addVar(const char name[],int type);

/* print table */
void printTable();

/* create a symbole table from a AST */
void createTable(Node *node);

/* check if the name is already defined in the symbole table */
void isInTable(const char name[]);

#endif
