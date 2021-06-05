#include "symbol-table.h"

void printSymbol(SymbolTable *table, Symbol *s);

SymbolType *makeSymbolType(int type, char *name, Symbol *member[],
                           int memberSize) {
    SymbolType *st = (SymbolType *)malloc(sizeof(SymbolType));
    st->type = type;
    strcpy(st->name, name);
    for (int i = 0; i < memberSize; i++) {
        st->member[i] = member[i];
    }
    st->memberSize = memberSize;
    return st;
}

/**
 * If two symbol types have the same type and name, they are equal.
 */
int equalSymbolType(SymbolType *st1, SymbolType *st2) {
    return st1->type == st2->type && (strcmp(st1->name, st2->name) == 0);
}

Symbol *makeSymbol(char *identifier, int type_descriptor) {
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    strcpy(s->identifier, identifier);
    s->type_descriptor = type_descriptor;
    s->stack_address = actual_stack_size;
    return s;
}

Scope *makeScope(Scope *father, char *name) {
    Scope *s = (Scope *)malloc(sizeof(Scope));
    s->size = 0;
    s->father = father;
    strcpy(s->name, name);
    return s;
}

SymbolTable *makeEmptySymbolTable() {
    SymbolTable *t = (SymbolTable *)malloc(sizeof(SymbolTable));
    t->current = NULL;

    SymbolType *the_void =
        makeSymbolType(TYPE_VOID, typeToString(TYPE_VOID), NULL, 0);
    SymbolType *the_int =
        makeSymbolType(TYPE_INT, typeToString(TYPE_INT), NULL, 0);
    SymbolType *the_char =
        makeSymbolType(TYPE_CHAR, typeToString(TYPE_CHAR), NULL, 0);

    t->typeDefined[0] = the_void;
    t->typeDefined[1] = the_int;
    t->typeDefined[2] = the_char;
    t->type_size = 3;
    return t;
}

// TODO finish it
/* not implemented, wait OS to do its job */
void freeScope(Scope *s) { ; }

void pushScope(SymbolTable *table, char *name) {
    Scope *cur = makeScope(table->current, name);
    table->current = cur;
}

void popScope(SymbolTable *table) {
    Scope *old_scope = table->current;
    table->current = old_scope->father;
    freeScope(old_scope);
}

void insertSymbol(SymbolTable *table, char identifier[], int type_descriptor) {
    Scope *current_scope = table->current;
    current_scope->symbols[current_scope->size] =
        makeSymbol(identifier, type_descriptor);
    current_scope->size += 1;

    actual_stack_size += 8; /* update stack index,
			       all elements are qwords for testing */ 
}

int hasType(SymbolTable *table, SymbolType *type){
    SymbolType **typeDefined = table->typeDefined;
    for (int i = 0; i < table->type_size; i++) {
        if (equalSymbolType(typeDefined[i], type)) {
            return i;
        }
    }
    return -1;
}


/**
 * Insert a definition of type to symbol table.
 */
int insertType(SymbolTable *table, SymbolType *type) {
    if(hasType(table, type) != -1)
        return -1;
    table->typeDefined[table->type_size] = type;
    table->type_size++;
    return table->type_size - 1;
}

int lookupSymbol(SymbolTable *table, char identifier[]) {
    Scope *cur = table->current;
    while (cur != NULL) {
        for (size_t i = 0; i < cur->size; i++) {
            if (STR_EQUAL(cur->symbols[i]->identifier, identifier)) {
                return cur->symbols[i]->type_descriptor;
            }
        }
        cur = cur->father;
    }
    return -1;
}

int getSymbolAddress(SymbolTable *table, char identifier[]){
    Scope *cur = table->current;
    while (cur != NULL) {
        for (size_t i = 0; i < cur->size; i++) {
            if (STR_EQUAL(cur->symbols[i]->identifier, identifier)) {
                return actual_stack_size - cur->symbols[i]->stack_address;
            }
        }
        cur = cur->father;
    }
    return -1;
}
