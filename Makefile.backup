CC=gcc
CFLAGS=-I.
DEPS = parse.h y.tab.h log.h process_request.h
OBJ = y.tab.o lex.yy.o parse.o lisod.o log.o process_request.o
FLAGS = -g -Wall

default: lisod

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y
	yacc -d $^

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

lisod: $(OBJ)
	#@gcc select_server.c -o select_server -Wall -Werror
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	#@rm select_server echo_client
	rm -f *~ *.o lisod lex.yy.c y.tab.c y.tab.h