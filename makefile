CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wall -lfl -ly -I src/
EXEC=bin/tpcc

LEXICAL = parser_lexical
SYNTACTIC = parser_syntactic

OBJS = obj/$(LEXICAL).c obj/$(SYNTACTIC).c $(shell ls src/*.c)

TEST_SCRIPT = tests.sh

all : $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS)

obj/$(SYNTACTIC).c : src/$(SYNTACTIC).y
	bison -d $< -o $@ -ly

obj/$(LEXICAL).c: src/$(LEXICAL).flex
	flex -o $@ $<

clean :
	rm -f obj/*
	rm -f $(EXEC)

test: $(EXEC)
	bash $(TEST_SCRIPT)
