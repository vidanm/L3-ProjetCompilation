#include <stdio.h>

void declareASMVar(FILE *file,char* name,int type){
	
	switch (type){
		case 2:
			fprintf(file,"%s:	RESB 1\n",name);
			break;
		case 0:
			fprintf(file,"%s:	RESW 1\n",name);
			break;
		case 1:
			fprintf(file,"%s:	RESD 1\n",name);
			break;
		case 3:
			fprintf(file,"%s:	RESQ 1\n",name);
			break;
		default:
			fprintf(file,"ERROR TOO BIG ALLOCATION\n");
	}
}
