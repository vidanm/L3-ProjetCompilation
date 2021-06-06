#include "check_instruction.h"

void checkMove(Node *move, SymbolTable *table);
void checkReadE(Node *readE, SymbolTable *table);
void checkReadC(Node *node, SymbolTable *table);
void checkPrint(Node *node, SymbolTable *table);
void checkIf(Node *node, SymbolTable *table);
void checkIfElse(Node *node, SymbolTable *table);
void checkWhile(Node *node, SymbolTable *table);
void checkExp(Node *node, SymbolTable *table);
void checkReturnExp(Node *node, SymbolTable *table);
void checkReturnVoid(Node *node, SymbolTable *table);

void typeMismatchWarning(int left, int right, SymbolTable *t, int line,
                         char *operation) {
    SymbolType *leftType = t->typeDefined[left];
    SymbolType *rightType = t->typeDefined[right];

    fprintf(stdout,
            "%swarning%s: at line %d, %s operation is performed between %s and "
            "%s\n",
            RED_BEGIN, RED_END, line, operation, leftType->name,
            rightType->name);
}

void unexpectedWarning(int expected, int actuel, SymbolTable *t, int line) {
    SymbolType *expectedType = t->typeDefined[expected];
    SymbolType *actuelType = t->typeDefined[actuel];

    fprintf(stdout, "%swarning%s: at line %d, expected: %s, got: %s\n",
            RED_BEGIN, RED_END, line, expectedType->name, actuelType->name);
}

void unexpectedError(int expected, int actuel, SymbolTable *t, int line) {
    SymbolType *expectedType = t->typeDefined[expected];
    SymbolType *actuelType = t->typeDefined[actuel];

    fprintf(stderr, "%swarning%s: at line %d, expected: %s, got: %s\n",
            RED_BEGIN, RED_END, line, expectedType->name, actuelType->name);
    exit(SEMERR_EXIT);
}

void checkMove(Node *move, SymbolTable *table) {
    int left = typeOfExp(FIRSTCHILD(move), table);
    int right = typeOfExp(SECONDCHILD(move), table);
    if (left == right) {
        return;
    } else {
        // pass value between int and char.
        if (left + right == 3) {
            typeMismatchWarning(left, right, table, move->lineno, "Evaluation");
        } else {
            typeMismatchError(left, right, table, "Evaluation", move->lineno);
        }
    }
}

void checkReadE(Node *readE, SymbolTable *table) {
    int td = typeOfExp(readE->firstChild, table);
    if (td == 2) {
        unexpectedWarning(1, 2, table, readE->lineno);
    } else if (td > 2) {
        unexpectedError(1, td, table, readE->lineno);
    }
}

void checkReadC(Node *node, SymbolTable *table) {
    int td = typeOfExp(node->firstChild, table);
    if (td == 1) {
        unexpectedWarning(2, 1, table, node->lineno);
    } else if (td > 2) {
        unexpectedError(2, td, table, node->lineno);
    }
}

void checkPrint(Node *node, SymbolTable *table) {
    int td = typeOfExp(node->firstChild, table);
    if (td > 2) {
        unexpectedError(2, td, table, node->lineno);
    }
}

void checkIf(Node *node, SymbolTable *table) {
    int td = typeOfExp(FIRSTCHILD(node), table);
    // condition isn't int nor char
    if (td > 2) {
        unexpectedError(1, td, table, node->lineno);
    }
    // warning in case of char
    if (td == 2) {
        unexpectedWarning(1, 2, table, node->lineno);
    }
    checkInstr(SECONDCHILD(node), table);
}

void checkIfElse(Node *node, SymbolTable *table) {
    int td = typeOfExp(FIRSTCHILD(node), table);
    // condition isn't int nor char
    if (td > 2) {
        unexpectedError(1, td, table, node->lineno);
    }
    // warning in case of char
    if (td == 2) {
        unexpectedWarning(1, 2, table, node->lineno);
    }
    checkInstr(SECONDCHILD(node), table);
    checkInstr(THIRDCHILD(node), table);
}
void checkWhile(Node *node, SymbolTable *table) {
    int td = typeOfExp(FIRSTCHILD(node), table);
    // condition isn't int nor char
    if (td > 2) {
        unexpectedError(1, td, table, node->lineno);
    }
    // warning in case of char
    if (td == 2) {
        unexpectedWarning(1, 2, table, node->lineno);
    }
    checkInstr(SECONDCHILD(node), table);
}

void checkExp(Node *node, SymbolTable *table) { typeOfExp(node, table); }

void checkReturnExp(Node *node, SymbolTable *table) {
    ;  // TODO
}

void checkReturnVoid(Node *node, SymbolTable *table) {
    ;  // TODO
}

void checkInstr(Node *node, SymbolTable *table) {
    // empty instr
    if (node == NULL) {
        return;
    }
    switch (node->kind) {
        case Move: {
            checkMove(node, table);
            break;
        }
        case InstrReadE: {
            checkReadE(node, table);
            break;
        }
        case InstrReadC: {
            checkReadC(node, table);
            break;
        }
        case Print: {
            checkPrint(node, table);
            break;
        }
        case InstrIF: {
            checkIf(node, table);
            break;
        }
        case InstrIFELSE: {
            checkIfElse(node, table);
            break;
        }
        case InstrWhile: {
            checkWhile(node, table);
            break;
        }
        default: {
            if (isExpNode(node)) {
                checkExp(node, table);
            }
        }
    }
}
