/* abstract-tree.c */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstract-tree.h"
extern int lineno;       /* from lexer */

static const char *StringFromKind[] = {
  "Program",
  "VarDeclList",
  /* declaration */
  "GlobeVar",
  "TypeSimp",
  "TypeStruct",
  "DeclVar",
  "DeclChamp",
  "DefStruct",
  "DefFunct",
  "DefFunctHead",
  "DefFunctCorps",
  "ParaTypVar",
  /* instructions */
  "Move",
  "InstrIF",
  "InstrIFELSE",
  "InstrWhile",
  "InstrReadE",
  "InstrReadC",
  "InstrPrint",
  "Instructions",
  "ReturnVoid",
  "ReturnValue",
  "Print",
  /* expression */
  "Identifier",
  "IntLiteral",
  "CharLiteral",
  "DivStar",
  "Order",
  "AddSub",
  "UnaryAddSub",
  "Eq",
  "Or",
  "And",
  "Negation",
  "Struct",
  "Type",
  "Void",
<<<<<<< HEAD
  "Call"
  /* and all other node labels */
=======
  "Func",
  "VarDeclaration",
  "DeclStruct",
  "Parameter",
  "ReturnType",
  "Corps",
  "End",
  "StructIdentifier"
  
	  /* and all other node labels */
>>>>>>> 6fb22cea9ec82d805f98b5db4113475c849ec33f
  /* The list must coincide with the enum in abstract-tree.h */
  /* To avoid listing them twice, see https://stackoverflow.com/a/10966395 */
};

static const char *StringFromType[] = {
	"void",
	"int",
	"char",
	"struct"
};

Node *makeNode(Kind kind) {
  Node *node = malloc(sizeof(Node));
  if (!node) {
    printf("Run out of memory\n");
    exit(1);
  }
  node->kind = kind;
  node-> firstChild = node->nextSibling = NULL;
  node->lineno=lineno;
  return node;
}

void addSibling(Node *node, Node *sibling) {
  Node *curr = node;
  while (curr->nextSibling != NULL) {
    curr = curr->nextSibling;
  }
  curr->nextSibling = sibling;
}

void addChild(Node *parent, Node *child) {
  if (parent->firstChild == NULL) {
    parent->firstChild = child;
  }
  else {
    addSibling(parent->firstChild, child);
  }
}

void deleteTree(Node *node) {
  if (node->firstChild) {
    deleteTree(node->firstChild);
  }
  if (node->nextSibling) {
    deleteTree(node->nextSibling);
  }
  free(node);
}

void set_identifier(Node *node, char *identifier){
  strcpy(node->u.identifier,identifier);
}

void printTree(Node *node) {
  static bool rightmost[128]; // current node is rightmost sibling
  static int depth = 0;       // depth of current node
  for (int i = 1; i < depth; i++) { // 2502 = vertical line
    printf(rightmost[i] ? "    " : "\u2502   ");
  }
  if (depth > 0) { // 2514 = up and right; 2500 = horiz; 251c = vertical and right 
    printf(rightmost[depth] ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
  }
  printf("%s", StringFromKind[node->kind]);
  switch (node->kind) {
    case VarDeclaration: printf(": %s",node->u.identifier);break;
    case Type: printf(": %s",StringFromType[node->u.integer]);break;
    case ReturnType: printf(": %s",StringFromType[node->u.integer]);break;
    case IntLiteral: printf(": %d", node->u.integer); break;
    case CharLiteral: printf(": '%c'", node->u.character); break;
    case Identifier: printf(": %s", node->u.identifier); break;
<<<<<<< HEAD
    case TypeSimp: printf(": %s", node->u.identifier); break;
    case DefFunctHead: printf(": %s", node->u.identifier); break;
    case Call: printf(": %s", node->u.identifier); break;
=======
    case DeclStruct: printf(": %s",node->u.identifier); break;
    case StructIdentifier: printf(": %s",node->u.identifier); break;
>>>>>>> 6fb22cea9ec82d805f98b5db4113475c849ec33f
    default: break;
  }
  printf("\n");
  depth++;
  for (Node *child = node->firstChild; child != NULL; child = child->nextSibling) {
    rightmost[depth] = (child->nextSibling) ? false : true;
    printTree(child);
  }
  depth--;
}
