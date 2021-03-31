CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall -lfl -ly
EXEC=as

all : $(EXEC) clean

$(EXEC): src/lex.yy.c src/$(EXEC).tab.c src/abstract-tree.c
	$(CC) src/abstract-tree.c src/lex.yy.c src/$(EXEC).tab.c $(LDFLAGS) -o $(EXEC)

src/$(EXEC).tab.c : src/tpc-2020-2021.y
	bison -d src/tpc-2020-2021.y -o src/$(EXEC).tab.c -ly

src/lex.yy.c: src/$(EXEC).flex
	flex -o src/lex.yy.c src/$(EXEC).flex

clean :
	rm -f src/*.tab.c src/*.tab.h src/lex.yy.c

test:
	bash ./src/tests.sh
