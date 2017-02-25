################################################################################
# Makefile                                                                     #
#                                                                              #
# Description: This file contains the make rules for Recitation 1.             #
#                                                                              #
# Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                          #
#          Wolf Richter <wolf@cs.cmu.edu>                                      #
#                                                                              #
################################################################################

# default: echo_server echo_client

# echo_server:
# 	@gcc echo_server.c -o echo_server -Wall -Werror

# echo_client:
# 	@gcc echo_client.c -o echo_client -Wall -Werror

# clean:
# 	@rm echo_server echo_client

CC=gcc
CFLAGS=-I.
DEPS = parse.h y.tab.h
OBJ = y.tab.o lex.yy.o parse.o select_server.o
FLAGS = -g -Wall


default: select_server echo_client echo_server

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y
	yacc -d $^

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

select_server: $(OBJ)
	#@gcc select_server.c -o select_server -Wall -Werror
	$(CC) -o $@ $^ $(CFLAGS)

echo_server:
	@gcc echo_server.c -o echo_server -Wall -Werror

echo_client:
	@gcc echo_client.c -o echo_client -Wall -Werror

clean:
	#@rm select_server echo_client
	rm -f *~ *.o select_server echo_client lex.yy.c y.tab.c y.tab.h
