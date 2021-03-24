CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall -lfl -ly
EXEC=as

all : $(EXEC) clean

$(EXEC): src/lex.yy.c src/$(EXEC).tab.c src/abstract-tree.c
	$(CC) src/lex.yy.c src/$(EXEC).tab.c src/abstract-tree.c $(LDFLAGS) -o $(EXEC)

src/$(EXEC).tab.c : src/tpc-2020-2021.y
	bison -d src/tpc-2020-2021.y -o src/$(EXEC).tab.c -ly

src/lex.yy.c: src/$(EXEC).lex
	flex -o src/lex.yy.c src/$(EXEC).lex

clean :
	rm src/*.tab.c src/*.tab.h src/lex.yy.c
