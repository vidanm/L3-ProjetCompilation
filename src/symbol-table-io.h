#ifndef STI_H
#define STI_H

#include "symbol-table.h"

void printSymbolTable(SymbolTable *t);

void printCurrentScope(SymbolTable *t);

void print_sem_error(char *msg);

#endif

