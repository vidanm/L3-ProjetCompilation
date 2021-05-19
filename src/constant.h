#ifndef _COMPILATION_CONS
#define _COMPILATION_CONS

#define TYPE_VOID 0
#define TYPE_INT 1
#define TYPE_CHAR 2
#define TYPE_STRUCT 3
#define typeToString(type) (type_string[type])

#define TRUE 1
#define FALSE 0

extern char *type_string[];

#endif