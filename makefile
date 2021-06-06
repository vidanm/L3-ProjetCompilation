CC=gcc
CFLAGS=-Wall -g -I src/
LDFLAGS=-Wall -lfl -ly 
EXEC=bin/tpcc

LEXICAL = parser_lexical
SYNTACTIC = parser_syntactic

# all .c files without path
SRC = $(SYNTACTIC).c $(LEXICAL).c $(shell (cd src && ls *c))
# all .o files
objects = $(addsuffix .o, $(addprefix obj/, $(basename $(SRC))))

TEST_SCRIPT = tests.sh
#############################################
# create directory obj and bin if not exist
OBJ = $(firstword $(wildcard obj))
BIN = $(firstword $(wildcard bin))
ifeq (,$(OBJ))
  $(shell mkdir obj)
endif
ifeq (,$(BIN))
  $(shell mkdir bin)
endif
#############################################3

all : $(EXEC)

$(EXEC): $(objects)
	$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS)

src/$(SYNTACTIC).c : src/$(SYNTACTIC).y
	bison -d $< -o $@ -ly

src/$(LEXICAL).c: src/$(LEXICAL).flex
	flex -o $@ $<

# regular source c file at src with header
obj/%.o: src/%.c src/%.h
	$(CC) $< -o $@ $(CFLAGS) -c

# regular source c file at src without header
obj/%.o: src/%.c
	$(CC) $< -o $@ $(CFLAGS) -c

clean :
	rm -f obj/*
	rm -f $(EXEC) src/$(SYNTACTIC).c src/$(SYNTACTIC).h src/$(LEXICAL).c

test: $(EXEC)
	bash $(TEST_SCRIPT)
