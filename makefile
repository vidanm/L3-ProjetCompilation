CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wall -lfl -ly
EXEC=as

LEXICAL = src/lexical_parser
SYNTACTIC = src/syntactic_parser

TEST_SCRIPT = tests.sh

all : $(EXEC) clean

$(EXEC): $(LEXICAL).c $(SYNTACTIC).c src/abstract-tree.c src/symbol-table.c
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

$(SYNTACTIC).c : $(SYNTACTIC).y
	bison -d $< -o $@ -ly

$(LEXICAL).c: $(LEXICAL).flex
	flex -o $@ $<

clean :
	rm -f src/*_parser.c src/*_parser.h

test: $(EXEC)
	bash $(TEST_SCRIPT)
