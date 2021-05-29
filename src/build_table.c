#include "build_table.h"

int handleType(Node *type){
    if (type->kind == TypeStruct){
        return TYPE_STRUCT;
    }
    return type->u.integer;
}


void handleGlobeVar(Node *globeVar, SymbolTable *table) {
    int sym_type = handleType(globeVar->firstChild);
    Node *identifiers = globeVar->firstChild->nextSibling;
    for (Node *sibling = identifiers; sibling != NULL;
         sibling = sibling->nextSibling) {
        insertSymbol(table, sibling->u.identifier, sym_type);
    }
}


/**
 * Handle the node "DefFunctHead" of AST, it creates a new scope in the
 * symbol table for following locaux symbols and insert 
 * 
 * 
 */
void handleEnTeteFunct(Node *defFunctHead, SymbolTable *table) {
    // add a scope to table
    pushScope(table);
    Node *param = SECONDCHILD(defFunctHead);
    if (param->kind == Void) {
        return;
    }
    while (param != NULL) {
        int sym_type = handleType(param->firstChild);
        insertSymbol(table, param->u.identifier, sym_type);
        param = param->nextSibling;
    }
}

/**
 * Handle the node "DefFunctCorps" of AST, it insert all local symbols
 * to the symbol table, and // TODO does type checking on following instructions.
 * 
 * @param defCorps, the pointer to AST "DefFunctCorps" Node.
 * @param t, the pointer to the symbol table.
 */
void handleDefFunctCorps(Node *defCorps, SymbolTable *t) {
    Node *declVar = defCorps->firstChild;
    while (declVar != NULL && declVar->kind == DeclVar) {
        int type = handleType(declVar->firstChild);
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

/**
 * Construct symbol table from a AST.
 * 
 * Return:
 *  A pointer to a symbol table.
 */
SymbolTable *makeTableFromAST(Node *tree) {
    SymbolTable *table = makeEmptySymbolTable();
    handleNodeAndScope(tree, table);
    return table;
}