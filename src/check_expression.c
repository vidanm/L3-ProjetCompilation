#include "check_expression.h"

static Kind boolNodes[] = {Negation, Or, And, Eq, Order};
static int boolNodeSize = 5;

static Kind arithmeticNodes[] = {UnaryAddSub, AddSub, DivStar};
static int arithmeticNodeSize = 3;

static Kind expNodes[] = {Negation, Or,          And,    Eq,
                          Order,    UnaryAddSub, AddSub, DivStar};
static int expNodeSize = 8;

int containsKind(Kind kinds[], int size, Kind k);

int isExpNode(Node *n) { return containsKind(expNodes, expNodeSize, n->kind); }

void typeMismatchError(int left, int right, SymbolTable *t, char *operation,
                       int line) {
    SymbolType *leftType = t->typeDefined[left];
    SymbolType *rightType = t->typeDefined[right];

    fprintf(
        stderr,
        "%serror%s: at line %d, %s operation is not permitted between %s and "
        "%s\n",
        RED_BEGIN, RED_END, line, operation, leftType->name, rightType->name);

    exit(SEMERR_EXIT);
}

int undefinedError(Node *identifier) {
    fprintf(stderr, "%serror%s: ‘%s’ undeclared (first use at line %d)\n",
            RED_BEGIN, RED_END, identifier->u.identifier, identifier->lineno);
    exit(SEMERR_EXIT);
}

int containsKind(Kind kinds[], int size, Kind k) {
    for (int i = 0; i < size; i++) {
        if (kinds[i] == k) {
            return TRUE;
        }
    }
    return FALSE;
}

int containDescriptor(int tds[], int size, int descriptor) {
    for (int i = 0; i < size; i++) {
        if (tds[i] == descriptor) {
            return TRUE;
        }
    }
    return FALSE;
}

void existenceCheck(int td, Node *identifier) {
    if (td == -1) {
        undefinedError(FIRSTCHILD(identifier));
    }
    if (td == -2) {
        undefinedError(identifier);
    }
}

int isBool(int type_descriptor) {
    static int boolTd[] = {1, 2};
    return containDescriptor(boolTd, 2, type_descriptor);
}

int typeOfLogic(Node *logExp, SymbolTable *t) {
    if (logExp->kind == Negation) {
        if (isBool(typeOfExp(FIRSTCHILD(logExp), t))) {
            return 1;
        }
    }
    int left = typeOfExp(FIRSTCHILD(logExp), t);
    int right = typeOfExp(SECONDCHILD(logExp), t);
    if (isBool(left) && isBool(right)) {
        return 1;
    } else {
        typeMismatchError(right, left, t, "logical", logExp->lineno);
    }
    return 0;
}

int isArithmetic(int type_descriptor) {
    static int arith[] = {1, 2};
    return containDescriptor(arith, 2, type_descriptor);
}

int typeOfArithmetic(Node *ariExp, SymbolTable *t) {
    if (ariExp->kind == UnaryAddSub) {
        if (isArithmetic(typeOfExp(FIRSTCHILD(ariExp), t))) {
            return 1;
        }
    }
    int left = typeOfExp(FIRSTCHILD(ariExp), t);
    int right = typeOfExp(SECONDCHILD(ariExp), t);
    if (isArithmetic(left) && isArithmetic(right)) {
        return 1;
    } else {
        typeMismatchError(right, left, t, "arithmetical", ariExp->lineno);
        // never reached
        return -1;
    }
}

/**
 * Get type descriptor of a variable.
 *
 * @return type of descriptor in case of success, -1 if variable
 * does not exist or the member of struct doesn't exist, -2 if struct itself
 * doesn't exist
 */
int typeOfVariable(Node *identifier, SymbolTable *t) {
    int res;
    // case of simple type variable
    if (FIRSTCHILD(identifier) == NULL) {
        res = lookupSymbol(t, identifier->u.identifier);
    } else {
        res = lookupStructSymbol(t, identifier->u.identifier,
                                 FIRSTCHILD(identifier)->u.identifier);
    }

    existenceCheck(res, identifier);
    return res;
}

/**
 * This function shouldn't be called on a non-exp node.
 */
int typeOfExp(Node *exp, SymbolTable *t) {
    switch (exp->kind) {
        case IntLiteral: {
            return 1;
        }
        case CharLiteral: {
            return 2;
        }
        case Identifier: {
            return typeOfVariable(exp, t);
        }
        default: {
            if (containsKind(boolNodes, boolNodeSize, exp->kind)) {
                printf("line no: %d\n", exp->lineno);
                return typeOfLogic(exp, t);

            } else if (containsKind(arithmeticNodes, arithmeticNodeSize,
                                    exp->kind)) {
                printf("line no: %d\n", exp->lineno);
                return typeOfArithmetic(exp, t);
            } else
                return 0;
        }
    }
}
