#include <stdio.h>
#include "intermediate_code.h"

extern file;

void imul(){} /* write ASM code to multiply the last 2 values on the stack */

void idiv(){} /* write ASM code to divide the last 2 values on the stack */

void iadd(){}  /* write ASM code to add the last 2 values on the stack */

void isub(){}  /* write ASM code to subtract the last 2 values on the stack */

void istore(char* ident){} /* write ASM code to store in ident the last value on the stack */

void iload(char* ident){} /* write ASM code to push ident value to the stack */

void ldc(int value){} /* write ASM code to push constant value to the stack */
