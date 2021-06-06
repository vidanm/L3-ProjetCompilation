#ifndef IC_H
#define IC_H

#include "abstract-tree.h"
#include "symbol-table.h"
#include "check_expression.h"
/**
 * This file contains functions related to semantic check on all instructions
 */
void checkInstr(Node *instr, SymbolTable *t);


#endif

