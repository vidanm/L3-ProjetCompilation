#ifndef BT_H
#define BT_H

/**
 * This module contains processes involving multiple modules at compile time,
 * for example, building symbole needs defitions of AST and of symbole table
 * itself, which would increase dependency if put it inside of symbole table
 * module.
 */

#include "abstract-tree.h"
#include "symbol-table.h"
#include "symbol-table-io.h"
#include "asm_generation.h"
#include "intermediate_code.h"
#include "parse_arg.h"

SymbolTable *makeTableFromAST(Node *tree);

#endif