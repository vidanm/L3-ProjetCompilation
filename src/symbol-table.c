#include "symbol-table.h"

char *StringFromType[] = {"void", "int", "char", "struct"};

Symbol *makeSymbol(char *identifier, int type) {
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    strcpy(s->identifier, identifier);
    s->type = type;
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

void insertSymbol(SymbolTable *table, char identifier[], int type) {
    Scope *current_scope = table->current;
    current_scope->symbols[current_scope->size] = makeSymbol(identifier, type);
    current_scope->size += 1;
}

int lookupSymbol(SymbolTable *table, char identifier[]) {
    Scope *cur = table->current;
    while (cur != NULL) {
        for (size_t i = 0; i < cur->size; i++) {
            if (STR_EQUAL(cur->symbols[i]->identifier, identifier)) {
                return cur->symbols[i]->type;
            }
        }
        cur = cur->father;
    }
    return -1;
}

int typeStrToInt(char *strType) {
    if (STR_EQUAL(strType, "int")) {
        return TYPE_INT;
    } else if (STR_EQUAL(strType, "char")) {
        return TYPE_CHAR;
    } else {
        return TYPE_STRUCT;
    }
}

void handleGlobeVar(Node *globeVar, SymbolTable *table) {
    int sym_type = typeStrToInt(globeVar->firstChild->u.identifier);
    Node *identifiers = globeVar->firstChild->nextSibling;
    for (Node *sibling = identifiers; sibling != NULL;
         sibling = sibling->nextSibling) {
        insertSymbol(table, sibling->u.identifier, sym_type);
    }
}

void handleEnTeteFunct(Node *defFunctHead, SymbolTable *table) {
    pushScope(table);
    Node *param = SECONDCHILD(defFunctHead);
    if (param->kind == Void) {
        return;
    }
    while (param != NULL) {
        int sym_type = typeStrToInt(param->firstChild->u.identifier);
        insertSymbol(table, param->u.identifier, sym_type);
        param = param->nextSibling;
    }
}

void handleDefFunctCorps(Node *defCorps, SymbolTable *t) {
    Node *declVar = defCorps->firstChild;
    while (declVar != NULL && declVar->kind == DeclVar) {
        int type = typeStrToInt(declVar->firstChild->u.identifier);
        for (Node *sibling = SECONDCHILD(declVar); sibling != NULL;
            sibling = sibling->nextSibling) {
            insertSymbol(t, sibling->u.identifier, type);
        }
        declVar = declVar->nextSibling;
    }
}

void handleNodeAndScope(Node *node, SymbolTable *t) {
    if (node->kind == Program) {
        pushScope(t);
    } else if (node->kind == GlobeVar) {
        handleGlobeVar(node, t);
    } else if (node->kind == DefFunctHead) {
        handleEnTeteFunct(node, t);
    } else if (node->kind == DefFunctCorps) {
        handleDefFunctCorps(node, t);
    }

    for (Node *child = node->firstChild; child != NULL;
         child = child->nextSibling) {
        handleNodeAndScope(child, t);
    }
}

SymbolTable *makeTableFromAST(Node *tree) {
    SymbolTable *table = makeEmptySymbolTable();
    handleNodeAndScope(tree, table);
    return table;
}

void printSymbol(Symbol *s) {
    printf("| %s : %s |\n", s->identifier, StringFromType[s->type]);
}

void printScope(Scope *s) {
    if (s->father != NULL) {
        printScope(s->father);
    }
    printf("------------ Scope --------------\n");
    for (int i = 0; i < s->size; i++) {
        Symbol *sym = s->symbols[i];
        printSymbol(sym);
    }
    printf("------------  End  --------------\n");
}

void printSymbolTable(SymbolTable *t) {
    printf("Symbol Table\n");
    printScope(t->current);
}

// STentry *symbolTable[MAX_TABLES]; /* Tableau d'adresses vers les tables
//  globales/locales */
//
// int STmax = MAX_SYMBOL_NUM;
// int STSize[MAX_TABLES]; /* Nombres de symboles dans each sub-table */
// int STNumber = 0;       /* Nomber of sub-tables */
// int current_type = 0;
//
// /**
//  * Append a new sub-table to symbole table
//  */
// void appendST() {
// STentry *table;
// table = (STentry *)malloc(MAX_SYMBOL_NUM * sizeof(STentry));
// symbolTable[STNumber] = table;
// STSize[STNumber] = 0;
// STNumber += 1;
// }
//
// /**
//  * Pop up current sub-table, all content will be cleared
//  */
// void popST() {
// free(symbolTable[STNumber]);
// symbolTable[STNumber] = NULL;
// STSize[STNumber] = 0;
// STNumber -= 1;
// }
//
// /**
//  * Add a entry into current scope's sub-table, record its name and type
//  */
// void addVar(const char name[], int type) {
// STentry *current_table = symbolTable[STNumber-1];
//
//
// int i, j;
// for (i = 0; i < STNumber; i++) {
// for (j = 0; j < STSize[STNumber]; j++) {
// if (!strcmp(symbolTable[i][j].name, name)) {
// printf("Redefinition de la variable %s", name);
// return;
// }
// }
// }
//
// if (STNumber == 0) {
// appendST();
// }
//
// if (++(STSize[STNumber - 1]) > STmax) {
// printf("too many variables near line \n");
// exit(1);
// }
//
// strcpy(symbolTable[STNumber - 1][(STSize[STNumber - 1]) - 1].name, name);
// symbolTable[STNumber - 1][(STSize[STNumber - 1]) - 1].type = type;
// }
//
// void createTable(Node *node) {
// while (node != NULL) {
// switch (node->kind) {
// case Type:
// current_type = node->u.integer;
// break;
// case Identifier:
// isInTable(node->u.identifier);
// default:
// break;
// }
//
// if (node->firstChild != NULL) {
// createTable(node->firstChild);
// }
//
// node = node->nextSibling;
// }
// }
//
// void isInTable(const char name[]) {
// int i, j, declared = 0;
// for (i = 0; i < STNumber; i++) {
// for (j = 0; j < STSize[i]; j++) {
// if (!strcmp(symbolTable[i][j].name, name)) {
// declared = 1;
// break;
// }
// }
// if (declared == 1) break;
// }
//
// if (!declared)
// printf(
// "\x1b[31mUtilisation de la variable %s sans l'avoir déclaré\x1b[0m "
// "\n",
// name);
// }
//
// void printTable() {
// int i, j;
// printf("\x1b[34mTABLE DES SYMBOLES\n=============\x1b[0m\n");
// for (i = 0; i < STNumber; i++) {
// if (i == 0)
// printf("TABLE GLOBALE \n");
// else
// printf("TABLE LOCALE %d \n", i);
// for (j = 0; j < STSize[i]; j++) {
// printf("%s : %s\n", StringFromType[symbolTable[i][j].type],
//    symbolTable[i][j].name);
// }
// printf("\n");
// }
// printf("\n\n");
// }
