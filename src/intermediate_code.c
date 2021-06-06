#include <stdio.h>
#include "intermediate_code.h"
#include "symbol-table.h"

extern FILE* file;

void imul(){
	fprintf(file,";imul()\n");
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"mul rbx, rcx\n");
	fprintf(file,"push rbx\n\n");
}

void idiv(){
	fprintf(file,";idiv()\n");
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"div rbx, rcx\n");
	fprintf(file,"push rbx\n\n");
}

void iadd(){
	fprintf(file,";iadd()\n");
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"add rbx, rcx\n");
	fprintf(file,"push rbx\n\n");
}

void isub(){
	fprintf(file,";isub()\n");
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"sub rbx, rcx\n");
	fprintf(file,"push rbx\n\n");
}

void istoreglob(char* ident){
	fprintf(file,";istoreglob()\n");
	fprintf(file,"pop qword[%s]\n\n",ident); 
}

void iloadglob(char* ident){
	fprintf(file,";iloadglob()\n");
	fprintf(file,"push qword[%s]\n\n",ident);
}

void ldc(int value){
	/* push constant to stack */
	fprintf(file,";ldc()\n");
	fprintf(file,"push qword %d\n\n",value);
}

void istore(SymbolTable* table,char *ident){
	int address = getSymbolAddress(table,ident);
	fprintf(file,";istore()\n");
	fprintf(file,"mov rax, [rsp+%d]\n\n",address); 
}

void iload(SymbolTable* table,char *ident){
	int address = getSymbolAddress(table,ident);
	fprintf(file,";iload()\n");
	fprintf(file,"pop rax\n");
	fprintf(file,"mov [rsp+%d], rax\n\n",address);
}

void write_main_section(){
	fprintf(file,"\nsection .text\n");
	fprintf(file,"\t global _start\n");
	fprintf(file,"_start:\n");
}

void write_bss_section(){
	fprintf(file,"section .bss\n");
}

void asmif(){;}

void asmor(){;}

void asmand(){;}

void asmeq(){;}

void iprint(SymbolTable* table,char * ident){
	int address = getSymbolAddress(table,ident);
	fprintf(file,";iprint()\n");
	fprintf(file,"push [rsp+%d]\n",address);
	fprintf(file,"call printf\n");
	fprintf(file,"add esp, 8\n\n");
}


