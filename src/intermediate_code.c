#include <stdio.h>
#include "intermediate_code.h"

extern file;

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

void istore(char* ident){
	fprintf(file,"pop qword[%s]\n",ident);
}

void iload(char* ident){
	fprintf(file,"push qword[%s]\n",ident);
}

void ldc(int value){
	/* push constant to stack */
	fprintf(file,"push qword %d",value);
}
