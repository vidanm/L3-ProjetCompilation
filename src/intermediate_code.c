#include <stdio.h>
#include "intermediate_code.h"
#include "symbol-table.h"

extern FILE* file;

void imul(){
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"mul rbx, rcx\n");
	fprintf(file,"push rbx\n");
}

void idiv(){
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"div rbx, rcx\n");
	fprintf(file,"push rbx\n");
}

void iadd(){
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"add rbx, rcx\n");
	fprintf(file,"push rbx\n");
}

void isub(){
	fprintf(file,"pop rbx\n");
	fprintf(file,"pop rcx\n");
	fprintf(file,"sub rbx, rcx\n");
	fprintf(file,"push rbx\n");
}

void istoreglob(char* ident){
	fprintf(file,"pop qword[%s]\n",ident); 
}

void iloadglob(char* ident){
	fprintf(file,"push qword[%s]\n",ident);
}

void ldc(int value){
	/* push constant to stack */
	fprintf(file,"push qword %d",value);
}

void istore(SymbolTable* table,char *ident){
	int address = getSymbolAddress(table,ident);
	fprintf(file,"mov rax, [rsp+%d]",address); 
}

void iload(SymbolTable* table,char *ident){
	int address = getSymbolAddress(table,ident);
	fprintf(file,"mov [rsp+%d], rax",address);
}

void asmif(){;}

void asmor(){;}

void asmand(){;}
