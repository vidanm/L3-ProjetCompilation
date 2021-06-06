/**
 * This file contains functions related to semantic check on all instructions
 */

#ifndef EC_H
#define EC_H

#include "abstract-tree.h"
#include "symbol-table.h"
#include "constant.h"

#define RED_BEGIN "\033[1;31m"
#define RED_END "\033[0m"


/**
 * Get the type of the AST node that is expression, result of logical
 * operation will be considered as integer.
 * 
 * @return type descriptor of the exp. 1 for int,
 * 2 for char, -1 for invalid expression.
 */
int typeOfExp(Node *exp, SymbolTable *t);

/**
 * Print error message to stderr and exit with SEMERR_EXIT
 */
void typeMismatchError(int left, int right, SymbolTable *t, char *operation,
                       int line);


int isExpNode(Node *n);



#endif