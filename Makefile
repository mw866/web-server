
define build-cmd
$(CC) $(CFLAGS) -c  $< -o $@
endef

CC=gcc
CFLAGS=-Wall -O2 -Wno-unused-function

OBJ_DIR=objs
PARSE_DIR=parse
SELECT_DIR=select
PARSER_DEPS = $(PARSE_DIR)/parse.h $(PARSE_DIR)/y.tab.h
OBJ+=$(OBJ_DIR)/y.tab.o
OBJ+=$(OBJ_DIR)/lex.yy.o
OBJ+=$(OBJ_DIR)/parse.o
OBJ+=$(OBJ_DIR)/selectserver.o

all: prepare server

server: $(OBJ)
	$(CC) $(CFLAGS) -lcrypto -lssl $^ -o $@

prepare:
	mkdir -p $(OBJ_DIR)


# ==Make Select==
$(OBJ_DIR)/selectserver.o: $(SELECT_DIR)/selectserver.c 
	$(build-cmd)

# ==Make Parse==
$(PARSE_DIR)/lex.yy.c: $(PARSE_DIR)/lexer.l
	flex -o $@ $^

$(PARSE_DIR)/y.tab.c: $(PARSE_DIR)/parser.y
	yacc -d $^
	mv y.tab.* $(PARSE_DIR)/

$(OBJ_DIR)/%.o: $(PARSE_DIR)/%.c $(PARSER_DEPS)
	$(build-cmd)

$(OBJ_DIR)/lex.yy.o: $(PARSE_DIR)/lex.yy.c $(PARSER_DEPS)
	$(build-cmd)

$(OBJ_DIR)/y.tab.o: $(PARSE_DIR)/y.tab.c $(PARSER_DEPS)
	$(build-cmd)

# $(OBJ_DIR)/liso.o: $(SOURCE_DIR)/liso.c $(SOURCE_DIR)/liso.h
# 	$(build-cmd)

clean:
	rm $(PARSE_DIR)/y.tab.* $(PARSE_DIR)/lex.yy.c
	rm $(OBJ_DIR)/*.o
	rm server