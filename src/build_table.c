#include "build_table.h"

extern FILE *file;

/**
 * Check validation of AST type node.
 * @p
 * This function is potential exit, when meeting a undefined struct, this
 * function exit program with SMEERR_EXIT.
 * @p
 * If its a void or simple type(int, char), return its type descriptor;
 * @p
 * If its a struct, check whether it has been defined, if not print error
 * information to stderr and exit with semantical error code, otherwise return
 * its type descriptor.
 * @p
 * In case that the node type's kind is unknown,
 * exit program with OTHERF_EXIT, this is a runtime error,
 * node Type shouldn't be other kind than Void, TypeSimp or TypeStruct.
 *
 * @return the correspodant type descriptor
 *
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
            SymbolType *structType =
                makeSymbolType(TYPE_STRUCT, type->u.identifier, NULL, 0);
            int td = hasType(table, structType);
            if (td == -1) {
                fprintf(stderr, "struct not defined: %s\n", type->u.identifier);
                exit(SEMERR_EXIT);
            }
            return td;
        }
        default: {
            fprintf(stderr,
                    "Runtime error, type node is neither void, int, char, nor "
                    "a struct\n");
            exit(OTHERF_EXIT);
        }
    }
}

void handleGlobeVar(Node *globeVar, SymbolTable *table) {
    int td = handleType(globeVar->firstChild, table);
    Node *identifiers = globeVar->firstChild->nextSibling;
    for (Node *sibling = identifiers; sibling != NULL;
         sibling = sibling->nextSibling) {
        reserveGlobalVarAdress(file, sibling->u.identifier, td);
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
    // !! Global scope part
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
 * @p
 * Handle the node "DefFunctCorps" of AST, it inserts all local symbols
 * to the symbol table, and // TODO does type checking on following
 * instructions.
 * @p
 * defCorps is a linked list of node, each node contains 2 children,
 * the first is node Type, the seconde is a linked list of node Identifier.
 *
 * This function performs a sementic check, the variable type can not be void.
 *
 * @param defCorps, the pointer to AST "DefFunctCorps" Node.
 * @param t, the pointer to the symbol table.
 */
void handleDefFunctCorps(Node *defCorps, SymbolTable *t) {
    Node *declVar = defCorps->firstChild;
    while (declVar != NULL && declVar->kind == DeclVar) {
        int td = handleType(declVar->firstChild, t);
        // by definition of grammar, void is not a type,
        // no need to check
        for (Node *sibling = SECONDCHILD(declVar); sibling != NULL;
             sibling = sibling->nextSibling) {
            insertSymbol(t, sibling->u.identifier, td);
        }
        declVar = declVar->nextSibling;
    }
    // TODO perform semantic analyse in expressions

    if (f_table) {
        printCurrentScope(t);
    }
    popScope(t);
}

/**
 * Converst a list of struct member into list of symbols,
 * type checking will be performed during the transformation.
 *
 * @return  in case of invalid type in the member, return -1,
 * otherwise, the number of members will be returned.
 */
int handleStructMembers(Node *members, Symbol *member_symbols[],
                        SymbolTable *table) {
    Node *begin = members;
    int i = 0;
    while (begin != NULL && begin->kind == DeclChamp) {
        int td = handleType(begin->firstChild, table);
        if (td == -1) {
            return -1;
        }
        Node *identifier = begin->firstChild->nextSibling;
        while (identifier != NULL && identifier->kind == Identifier) {
            Symbol *s = makeSymbol(identifier->u.identifier, td);
            member_symbols[i] = s;
            i++;
            identifier = identifier->nextSibling;
        }
        begin = begin->nextSibling;
    }
    return i;
}

/**
 * Handle struct definition.
 * This function is a potentiel exit, when meeting a duplication
 * of structure definition, it will exit with SEMERR_EXIT.
 *
 */
void handleStructDef(Node *structDef, SymbolTable *table) {
    Symbol **struct_symbols =
        (Symbol **)malloc(sizeof(Symbol *) * MAX_MEMBER_NUMBER);

    int size = 0;
    // combine them as function type
    SymbolType *structType = makeSymbolType(
        TYPE_STRUCT, structDef->u.identifier, struct_symbols, size);

    /**
     * Insert this struct as a type to symbol table.
     * Insert the type before parsing its member
     * to enable self containing member.
     */
    int td = insertType(table, structType);
    if (td == -1) {
        fprintf(stderr, "Duplicated definition of structure %s\n",
                structDef->u.identifier);
        exit(SEMERR_EXIT);
    }
    // parser struct member as array of symbols
    size = handleStructMembers(FIRSTCHILD(structDef), struct_symbols, table);
    // record member information
    table->typeDefined[td]->memberSize = size;
    for (int i = 0; i < size; i++) {
        table->typeDefined[td]->member[i] = struct_symbols[i];
    }
}

void handleNodeAndScope(Node *node, SymbolTable *t) {

    

    switch (node->kind){ /* Set variable in symbolTable */ 
	case Program:
		pushScope(t);
		break;
	case GlobeVar:
		handleGlobeVar(node,t);
		break;
	case DefFunctHead:
		handleEnTeteFunct(node, t);
		break;
	case DefFunctCorps:
		handleDefFunctCorps(node, t);
		break;
	case DefStruct:
		handleStructDef(node, t);
		break;
	case FuncSection:
		write_main_section();
		break;
	case GlobVarsSection:
		write_bss_section();
		break;
	default:
		break;
	}

    for (Node *child = node->firstChild; child != NULL;
        child = child->nextSibling) {
	handleNodeAndScope(child, t);
    }

    switch (node->kind){
	case DeclVar:
		ldc(0);
		break;
	case Move:
		/* evaluate Expr and store on the stack */
		iload(t,node->firstChild->u.identifier); /* pop the stack and store the value */
		break;
	case UnaryAddSub:
		iadd();
		break;
	case DivStar:
		imul();
		break;
	case IntLiteral:
		ldc(node->u.integer);
		break;
	default:
		break;

    }
    // deep first search
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
