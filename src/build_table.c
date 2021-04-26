#include "build_table.h"

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