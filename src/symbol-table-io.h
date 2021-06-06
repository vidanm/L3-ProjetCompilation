#ifndef STI_H
#define STI_H

#include "symbol-table.h"

void printSymbolTable(SymbolTable *t);

void printCurrentScope(SymbolTable *t);

/**
 * Print semantic error message in stderr.
 */
void print_sem_error(char *msg);

/**
 * Print warning message to stdout.
 */
void warning(char *msg);

#endif

