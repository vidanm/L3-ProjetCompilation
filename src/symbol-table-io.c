#include "symbol-table.h"

void symbolTypeToString(SymbolType *type, char buf[], SymbolTable *table) {
    switch (type->type) {
        case TYPE_VOID: {
            strcpy(buf, "void");
            break;
        }
        case TYPE_INT: {
            strcpy(buf, "int");
            break;
        }
        case TYPE_CHAR: {
            strcpy(buf, "char");
            break;
        }
        case TYPE_STRUCT: {
            sprintf(buf, "struct %s", type->name);
            break;
        }
        case TYPE_FUNC: {
            int ret_td = type->member[0]->type_descriptor;
            symbolTypeToString(table->typeDefined[ret_td], buf, table);
            int offset = strlen(buf);
            buf[offset] = ' ';
            strcpy(buf + offset + 1, type->name);

            // parametres
            if (type->memberSize == 1) {
                strcat(buf, "()");
            } else {
                strcat(buf, "(");
                char param_buf[256];
                for (int i = 1; i < type->memberSize; i++) {
                    int td = type->member[i]->type_descriptor;
                    symbolTypeToString(table->typeDefined[td], param_buf,
                                       table);
                    strcat(buf, param_buf);
                    strcat(buf, ", ");
                }
                strcpy(buf + strlen(buf) - 2, ")");
            }
            break;
        }
        default:
            sprintf(buf, "unknown type: %d", type->type);
            break;
    }
}

void printParametres(Symbol *params[], SymbolTable *table) { ; }

void printSymbolType(SymbolTable *table, int type_descriptor) {
    /* static int depth = 0;  // depth of current node */
    char buf[256];
    symbolTypeToString(table->typeDefined[type_descriptor], buf, table);
    printf("%s", buf);
}

void printSymbol(SymbolTable *table, Symbol *s) {
    printSymbolType(table, s->type_descriptor);
    if (table->typeDefined[s->type_descriptor]->type != TYPE_FUNC) {
        putchar(' ');
        printf("%s\n", s->identifier);
    } else {
        putchar('\n');
    }
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
