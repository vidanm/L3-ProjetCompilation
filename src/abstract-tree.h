/* abstract-tree.h */

// include guard
#ifndef AST_H
#define AST_H

typedef enum {
  Program,
  VarDeclList,
  /* declaration */
  GlobeVar,
  TypeSimp,
  TypeStruct,
  DeclVar,
  DeclChamp,
  DefStruct,
  DefFunct,
  DefFunctHead,
  DefFunctCorps,
  ParaTypVar,
  /* instructions */
  Move,
  InstrIF,
  InstrIFELSE,
  InstrWhile,
  InstrReadE,
  InstrReadC,
  InstrPrint,
  Instructions,
  ReturnVoid,
  ReturnValue,
  Print,
  /* expression */
  Identifier,
  IntLiteral,
  CharLiteral,
  DivStar,
  Order,
  AddSub,
  UnaryAddSub,
  Eq,
  Or,
  And,
  Negation,
  Struct,
  Type,
  Void,
  Call,
  GlobVarsSection,
  FuncSection
  /* and allother node labels */
  /* The list must coincide with the strings in abstract-tree.c */
  /* To avoid listing them twice, see https://stackoverflow.com/a/10966395 */
} Kind;

typedef struct Node {
  Kind kind;
  union {
    int integer;
    char character;
    char identifier[64];
  } u;
  struct Node *firstChild, *nextSibling;
  int lineno;
} Node;

/**
 * Given the kind of node, makes a node of abstract tree.
 */
Node *makeNode(Kind kind);

/**
 * Given 2 nodes, add the second as sibling for the first one.
 */
void addSibling(Node *node, Node *sibling);

/**
 * Add a child for another node.
 */
void addChild(Node *parent, Node *child);

/**
 * Set a string value for the node in the parametre.
 * Normally the string is identifier.
 */
void set_identifier(Node *node, char *identifier);

/**
 * Delete the tree, and its childs recursively.
 */ 
void deleteTree(Node*node);

/**
 * Print the node and its childs to the stdout recursively.
 */
void printTree(Node *node);

/**
 * Macro to access the first child.
 */
#define FIRSTCHILD(node) node->firstChild

/**
 * Macro to access the second child.
 */
#define SECONDCHILD(node) node->firstChild->nextSibling

/**
 * Macro to access the third child.
 */
#define THIRDCHILD(node) node->firstChild->nextSibling->nextSibling

#endif
