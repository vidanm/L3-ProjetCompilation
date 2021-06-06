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

/* exit value in case of normal exit */
#define NORMAL_EXIT 0
/* exit value in case of syntaxe error */
#define SYNERR_EXIT 1
/* exit value in case of semantic error */
#define SEMERR_EXIT 2
/* exit value in case of other failure, for example argument line error */
#define OTHERF_EXIT 3

extern char *type_string[];

#endif