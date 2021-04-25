#include "symbol-table.h"


char *StringFromType[] = {"void", "int", "char", "struct"};

STentry *symbolTable[MAX_TABLES]; /* Tableau d'adresses vers les tables
                                     globales/locales */

int STmax = MAX_SYMBOLS;
int STSize[MAX_TABLES]; /* Nombres de symboles dans each sub-table */
int STNumber = 0;       /* Nomber of sub-tables */
int current_type = 0;

/**
 * Append a new sub-table to symbole table
 */
void appendST() {
    STentry *table;
    table = (STentry *)malloc(MAX_SYMBOLS * sizeof(STentry));
    symbolTable[STNumber] = table;
    STSize[STNumber] = 0;
    STNumber += 1;
}

/**
 * Pop up current sub-table, all content will be cleared
 */
void popST() {
    symbolTable[STNumber] = NULL;
    STSize[STNumber] = 0;
    STNumber -= 1;
}

/**
 * Add a entry into current scope's sub-table, record its name and type
 */
void addVar(const char name[], int type) {
    int i, j;
    for (i = 0; i < STNumber; i++) {
        for (j = 0; j < STSize[STNumber]; j++) {
            if (!strcmp(symbolTable[i][j].name, name)) {
                printf("Redefinition de la variable %s pres de la ligne\n",
                       name);
                return;
            }
        }
    }

    if (STNumber == 0) {
        appendST();
    }

    if (++(STSize[STNumber - 1]) > STmax) {
        printf("too many variables near line \n");
        exit(1);
    }

    strcpy(symbolTable[STNumber - 1][(STSize[STNumber - 1]) - 1].name, name);
    symbolTable[STNumber - 1][(STSize[STNumber - 1]) - 1].type = type;
}

void createTable(Node *node) {
    while (node != NULL) {
        switch (node->kind) {
            case Type:
                current_type = node->u.integer;
                break;
            case Identifier:
                isInTable(node->u.identifier);
            default:
                break;
        }

        if (node->firstChild != NULL) {
            createTable(node->firstChild);
        }

        node = node->nextSibling;
    }
}

void isInTable(const char name[]) {
    int i, j, declared = 0;
    for (i = 0; i < STNumber; i++) {
        for (j = 0; j < STSize[i]; j++) {
            if (!strcmp(symbolTable[i][j].name, name)) {
                declared = 1;
                break;
            }
        }
        if (declared == 1) break;
    }

    if (!declared)
        printf(
            "\x1b[31mUtilisation de la variable %s sans l'avoir déclaré\x1b[0m "
            "\n",
            name);
}

void printTable() {
    int i, j;
    printf("\x1b[34mTABLE DES SYMBOLES\n=============\x1b[0m\n");
    for (i = 0; i < STNumber; i++) {
        if (i == 0)
            printf("TABLE GLOBALE \n");
        else
            printf("TABLE LOCALE %d \n", i);
        for (j = 0; j < STSize[i]; j++) {
            printf("%s : %s\n", StringFromType[symbolTable[i][j].type],
                   symbolTable[i][j].name);
        }
        printf("\n");
    }
    printf("\n\n");
}
