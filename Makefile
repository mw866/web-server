CC=gcc
CFLAGS=-I.
DEPS = parse.h y.tab.h
OBJ = y.tab.o lex.yy.o parse.o select_server.o
FLAGS = -g -Wall


default: select_server

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y
	yacc -d $^

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

select_server: $(OBJ)
	#@gcc select_server.c -o select_server -Wall -Werror
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	#@rm select_server echo_client
	rm -f *~ *.o select_server echo_client lex.yy.c y.tab.c y.tab.h