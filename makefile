CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wall -lfl -ly
EXEC=as

LEXICAL = src/lexical_parser
SYNTACTIC = src/syntactic_parser

all : $(EXEC) clean

$(EXEC): $(LEXICAL).c $(SYNTACTIC).c src/abstract-tree.c
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

$(SYNTACTIC).c : $(SYNTACTIC).y
	bison -d $< -o $@ -ly

$(LEXICAL).c: $(LEXICAL).flex
	flex -o $@ $<

clean :
	rm -f src/*.tab.c src/*.tab.h src/lex.yy.c

test: $(EXEC)
	bash ./src/tests.sh
