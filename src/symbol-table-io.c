#include "symbol-table.h"

void symbolToString(SymbolTable *table, Symbol *s, char buf[]);

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
            sprintf(buf, "struct %s {", type->name);
            char mem_buf[256];
            for (int i = 0; i < type->memberSize; i++) {
                Symbol *member = type->member[i];

                if (equalSymbolType(table->typeDefined[member->type_descriptor], type)) {
                    sprintf(mem_buf, "struct %s, ", type->name);
                    strcat(buf, mem_buf);
                } else {
                    symbolToString(table, member, mem_buf);
                    strcat(buf, mem_buf);
                    strcat(buf, ", ");
                }
            }
            strcpy(buf + strlen(buf) - 2, "}");

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
                    symbolToString(table, type->member[i], param_buf);
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

void symbolToString(SymbolTable *table, Symbol *s, char buf[]) {
    char type_buf[256];
    symbolTypeToString(table->typeDefined[s->type_descriptor], type_buf, table);
    if (table->typeDefined[s->type_descriptor]->type != TYPE_FUNC) {
        strcat(type_buf, " ");
        strcat(type_buf, s->identifier);
    }
    strcpy(buf, type_buf);
}

void printSymbolType(SymbolTable *table, int type_descriptor) {
    char buf[256];
    symbolTypeToString(table->typeDefined[type_descriptor], buf, table);
    printf("%s", buf);
}

void printSymbol(SymbolTable *table, Symbol *s) {
    char buf[256];
    symbolToString(table, s, buf);
    printf("%s\n", buf);
    // printSymbolType(table, s->type_descriptor);
    // if (table->typeDefined[s->type_descriptor]->type != TYPE_FUNC) {
    //     putchar(' ');
    //     printf("%s\n", s->identifier);
    // } else {
    //     putchar('\n');
    // }
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

void printDefinedType(SymbolTable *t) {
    printf("----------Defined Type-----------\n");
    for (int i = 0; i < t->type_size; i++) {
        printSymbolType(t, i);
        putchar('\n');
    }
    printf("---------------------------------\n");

}

void printSymbolTable(SymbolTable *t) {
    printf("Symbol Table\n");
    printDefinedType(t);
    printScope(t, t->current);
}
