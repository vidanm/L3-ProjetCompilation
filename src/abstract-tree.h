/* abstract-tree.h */

typedef enum {
  Program,
  VarDeclList,
  IntLiteral,
  Identifier,
  UnaryAddSub,
  CharLiteral,
  Negation,
  UnaryDivStar,
  UnaryOrder,
  Eq,
  Or,
  And,
  Type,
  Assign,
  Struct,
  Reade,
  Readc,
  Print,
  If,
  While,
  FunctionCalling,
  Return,
  Void,
  Else
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

Node *makeNode(Kind kind);
void addSibling(Node *node, Node *sibling);
void addChild(Node *parent, Node *child);
void deleteTree(Node*node);
void printTree(Node *node);

#define FIRSTCHILD(node) node->firstChild
#define SECONDCHILD(node) node->firstChild->nextSibling
#define THIRDCHILD(node) node->firstChild->nextSibling->nextSibling
