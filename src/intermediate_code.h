#include <stdio.h>
#include "symbol-table.h"

extern SymbolTable * table;

void imul(); /* write ASM code to multiply the last 2 values on the stack */

void idiv(); /* write ASM code to divide the last 2 values on the stack */

void iadd();  /* write ASM code to add the last 2 values on the stack */

void isub();  /* write ASM code to subtract the last 2 values on the stack */

void istore(SymbolTable * table, char* ident); /* write ASM code to store in ident the last value on the stack */

void iload(SymbolTable * table, char* ident); /* write ASM code to push ident value to the stack */

void ldc(int value); /* write ASM code to push constant value to the stack */

void asmif();

void asmor();

void asmand();

void write_main_section();

void write_bss_section();
