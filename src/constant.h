#ifndef _COMPILATION_CONS
#define _COMPILATION_CONS

#define TYPE_VOID 0
#define TYPE_INT 1
#define TYPE_CHAR 2
#define TYPE_STRUCT 3
#define TYPE_FUNC 4
#define typeToString(type) (type_string[type])

#define TRUE 1
#define FALSE 0

#define NORMAL_EXIT 0
#define SMEERR_EXIT 1
#define SYNERR_EXIT 2

extern char *type_string[];

#endif