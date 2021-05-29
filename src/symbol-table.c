#include "symbol-table.h"

SymbolType *makeSymbolType(int type, char *name, Symbol *member[], int memberSize){
    SymbolType *st = (SymbolType*)malloc(sizeof(SymbolType));
    st->type = type;
    strcpy(st->name, name);
    for(int i = 0; i < memberSize; i++){
        st->member[i] = member[i];
    }
    st->memberSize = memberSize;
    return st;
}

/**
 * If two symbol types have the same type and name, they are equal.
 */
int equalSymbolType(SymbolType *st1, SymbolType *st2){
    return st1->type == st2->type && (strcmp(st1->name, st1->name) == 0);
}

Symbol *makeSymbol(char *identifier, int type_descriptor) {
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    strcpy(s->identifier, identifier);
    s->type_descriptor = type_descriptor;
    return s;
}

Scope *makeScope(Scope *father) {
    Scope *s = (Scope *)malloc(sizeof(Scope));
    s->size = 0;
    s->father = father;
    return s;
}

SymbolTable *makeEmptySymbolTable() {
    SymbolTable *t = (SymbolTable *)malloc(sizeof(SymbolTable));
    t->current = NULL;

    SymbolType *the_void = makeSymbolType(TYPE_VOID, typeToString(TYPE_VOID), NULL, 0);
    SymbolType *the_int = makeSymbolType(TYPE_INT, typeToString(TYPE_INT), NULL, 0);
    SymbolType *the_char = makeSymbolType(TYPE_CHAR, typeToString(TYPE_CHAR), NULL, 0);
    
    t->typeDefined[0] = the_void;
    t->typeDefined[1] = the_int;
    t->typeDefined[2] = the_char;
    t->type_size = 3;
    return t;
}

// TODO finish it
/* not implemented, wait OS to do its job */
void freeScope(Scope *s) { ; }

void pushScope(SymbolTable *table) {
    Scope *cur = makeScope(table->current);
    table->current = cur;
}

void popScope(SymbolTable *table) {
    Scope *old_scope = table->current;
    table->current = old_scope->father;
    freeScope(old_scope);
}

void insertSymbol(SymbolTable *table, char identifier[], int type_descriptor) {
    Scope *current_scope = table->current;
    current_scope->symbols[current_scope->size] = makeSymbol(identifier, type_descriptor);
    current_scope->size += 1;
}

/**
 * Insert a definition of type to symbol table.
 */
int insertType(SymbolTable *table, SymbolType *type){
    SymbolType **typeDefined = table->typeDefined;
    for(int i = 0; i < table->type_size; i++){
        if (equalSymbolType(typeDefined[i], type))
        {
            return -1;
        }
        
    }
    typeDefined[table->type_size] = type;
    table->type_size ++;
    return  table->type_size -1;
}

int lookupSymbol(SymbolTable *table, char identifier[]) {
    Scope *cur = table->current;
    while (cur != NULL) {
        for (size_t i = 0; i < cur -> size; i++) {
            if (STR_EQUAL(cur->symbols[i]->identifier, identifier)) {
                return cur->symbols[i]->type_descriptor;
            }
        }
        cur = cur->father;
    }
    return -1;
}

void printSymbol(SymbolTable *table, Symbol *s) {
    if(s->type_descriptor == -1){
        printf("| %s : %s |\n", s->identifier, "unknown structure");
        return;
    }
    SymbolType *st = table-> typeDefined[s->type_descriptor];
    printf("| %s : %s |\n", s->identifier, st->name);
}

void printScope(SymbolTable *t, Scope *s) {
    if (s->father != NULL) {
        printScope(t, s->father);
    }
    printf("------------ Scope --------------\n");
    printf("scope size: %d\n", s->size);
    for (int i = 0; i < s->size; i++) {
        Symbol *sym = s->symbols[i];
        printSymbol(t, sym);
    }
    printf("------------  End  --------------\n");
}

void printSymbolTable(SymbolTable *t) {
    printf("Symbol Table\n");
    printScope(t, t->current);
}
