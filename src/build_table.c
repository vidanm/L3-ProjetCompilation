#include "build_table.h"


/**
 *  Return the descriptor of the type in the node, or -1 in case of not existe.
 */
int handleType(Node *type, SymbolTable *table){
    switch(type->kind){
        case TypeSimp:{
            // by definition of constant, its value is just the descriptor
            return type->u.integer;
        }case TypeStruct:{
            return lookupSymbol(table, type->u.identifier);
        }
        default:{
            return -1;
        }
    }
}


void handleGlobeVar(Node *globeVar, SymbolTable *table) {
    int td = handleType(globeVar->firstChild, table);
    Node *identifiers = globeVar->firstChild->nextSibling;
    for (Node *sibling = identifiers; sibling != NULL;
         sibling = sibling->nextSibling) {
        insertSymbol(table, sibling->u.identifier, td);
    }
}

void handleEnTeteFunct(Node *defFunctHead, SymbolTable *table) {
    pushScope(table);
    Node *param = SECONDCHILD(defFunctHead);
    if (param->kind == Void) {
        return;
    }
    while (param != NULL) {
        int sym_type = handleType(param->firstChild, table);
        insertSymbol(table, param->u.identifier, sym_type);
        param = param->nextSibling;
    }
}

void handleDefFunctCorps(Node *defCorps, SymbolTable *t) {
    Node *declVar = defCorps->firstChild;
    while (declVar != NULL && declVar->kind == DeclVar) {
        int td = handleType(declVar->firstChild, t);
        // TODO after insert type check td, in case of -1, error undefine type.
        for (Node *sibling = SECONDCHILD(declVar); sibling != NULL;
            sibling = sibling->nextSibling) {
            insertSymbol(t, sibling->u.identifier, td);
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