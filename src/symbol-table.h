#define MAXSYMBOLS 256
#define MAXNAME 64

typedef struct {
	char name[MAXNAME];
	int type;
} STentry;

void addVar(const char name[],int type);
void printTable();
void createTable(Node *node);
