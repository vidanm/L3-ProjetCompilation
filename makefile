CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wall -lfl -ly
EXEC=as

LEXICAL = src/parser_lexical
SYNTACTIC = src/parser_syntactic

TEST_SCRIPT = tests.sh

all : $(EXEC) clean

$(EXEC): $(LEXICAL).c $(SYNTACTIC).c src/abstract-tree.c src/symbol-table.c
	$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS)

$(SYNTACTIC).c : $(SYNTACTIC).y
	bison -d $< -o $@ -ly

$(LEXICAL).c: $(LEXICAL).flex
	flex -o $@ $<

clean :
	rm -f src/parser_*.c src/parser_*.h

test: $(EXEC)
	bash $(TEST_SCRIPT)
