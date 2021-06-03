#include "build_table.h"
#include "asm_generation.h"
extern FILE* file;

/**
 *  Return the descriptor of the type in the node, or -1 in case of not existe.
 */
int handleType(Node *type, SymbolTable *table) {
    switch (type->kind) {
        case Void: {
            return 0;
        }
        case TypeSimp: {
            // by definition of constant, its value is just the descriptor
            return type->u.integer;
        }
        case TypeStruct: {
            SymbolType *structType = makeSymbolType(TYPE_STRUCT, type->u.identifier, NULL, 0);
            int td = hasType(table, structType);
            if (td == -1) {
                fprintf(stderr, "struct not defined: %s\n", type->u.identifier);
                exit(SMEERR_EXIT);
            }
            return td;
        }
        default: {
            fprintf(stderr, "unknown type type: %d\n", type->kind);
            return type->kind;
            //exit(-1);
        }
    }
}

void handleGlobeVar(Node *globeVar, SymbolTable *table) {
    int td = handleType(globeVar->firstChild, table);
    Node *identifiers = globeVar->firstChild->nextSibling;
    for (Node *sibling = identifiers; sibling != NULL;
         sibling = sibling->nextSibling) {
            reserveGlobalVarAdress(file,sibling->u.identifier,td);
	    insertSymbol(table, sibling->u.identifier, td);
    }
}

/**
 * Converst a list of params into list of symbols,
 * type checking will be performed during the transformation.
 *
 * @return  in case of invalid type in the parametres, return -1,
 * otherwise, the number of parametres will be returned.
 */
int handleParametres(Node *params, Symbol *params_symbols[],
                     SymbolTable *table) {
    if (params->kind == Void) {
        return 0;
    }
    Node *begin = params;
    int i = 0;
    while (begin != NULL && begin->kind == ParaTypVar) {
        int td = handleType(begin->firstChild, table);
        if (td == -1) {
            return -1;
        }
        Symbol *s = makeSymbol(begin->u.identifier, td);
        params_symbols[i] = s;
        i++;
        begin = begin->nextSibling;
    }
    return i;
}

/**
 * Handle the node "DefFunctHead" of AST, insert itself as a symbol,
 * then it creates a new scope in the symbol table, inserts its parametres as
 * local symbol.
 *
 */
void handleEnTeteFunct(Node *defFunctHead, SymbolTable *table) {
    Symbol **func_symbols =
        (Symbol **)malloc(sizeof(Symbol *) * MAX_MEMBER_NUMBER);
    // parser function return type
    Node *type = defFunctHead->firstChild;
    int ret_td = handleType(type, table);
    func_symbols[0] = makeSymbol("func ret", ret_td);
    // parser function params as array of symbols
    int size =
        handleParametres(SECONDCHILD(defFunctHead), func_symbols + 1, table);

    // combine them as function type
    SymbolType *functionType = makeSymbolType(
        TYPE_FUNC, defFunctHead->u.identifier, func_symbols, size + 1);
    // insert this type to symbol table
    int td = insertType(table, functionType);

    // insert function as a symbol
    insertSymbol(table, defFunctHead->u.identifier, td);
    // add a scope to table
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

/**
 * Handle the node "DefFunctCorps" of AST, it insert all local symbols
 * to the symbol table, and // TODO does type checking on following
 * instructions.
 *
 * @param defCorps, the pointer to AST "DefFunctCorps" Node.
 * @param t, the pointer to the symbol table.
 */
void handleDefFunctCorps(Node *defCorps, SymbolTable *t) {
    Node *declVar = defCorps->firstChild;
    while (declVar != NULL && declVar->kind == DeclVar) {
        printTree(declVar);
        printTree(declVar->firstChild);
        int td = handleType(declVar->firstChild, t);
        printf("function corp:%d\n", td);
        // TODO after insert type check td, in case of -1, error undefine type.
        for (Node *sibling = SECONDCHILD(declVar); sibling != NULL;
             sibling = sibling->nextSibling) {
            insertSymbol(t, sibling->u.identifier, td);
        }
        declVar = declVar->nextSibling;
    }
}

void handleStructDef(Node *structDef, SymbolTable *table) {
    printf("We have a struct def\n");
    Symbol **struct_symbols =
        (Symbol **)malloc(sizeof(Symbol *) * MAX_MEMBER_NUMBER);

    // parser struct member as array of symbols
    int size = handleParametres(FIRSTCHILD(structDef), struct_symbols, table);
    printf("member size: %d\n", size);

    // combine them as function type
    SymbolType *structType = makeSymbolType(
        TYPE_STRUCT, structDef->u.identifier, struct_symbols, size);
    // insert this type to symbol table
    int td = insertType(table, structType);
    printf("inserted at %d\n", td);
    if (td == -1) {
        fprintf(stderr, "Duplicated definition of structure %s\n",
                structDef->u.identifier);
        exit(SMEERR_EXIT);
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
    } else if (node->kind == DefStruct) {
        handleStructDef(node, t);
    }
    // deep first search
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
