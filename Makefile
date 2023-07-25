
obj = util.o node.o lexer.o htable.o test_lexer.o
CC=gcc
CFLAGS= -g -ggdb -Wall -Wextra -O2

build: $(obj)
	$(CC) output/*.o $(CFLAGS) -o bin/test_lexer

lexer.o: src/lexer.c src/lexer.h src/util.h src/node.h
	$(CC) src/lexer.c $(CFLAGS)  -c -o output/lexer.o 

node.o: src/node.c src/node.h src/util.h
	$(CC) src/node.c $(CFLAGS) -c -o output/node.o

util.o: src/util.c src/util.h
	$(CC) src/util.c $(CFLAGS) -c -o output/util.o

htable.o: src/htable.c src/htable.h src/util.c src/util.h \
	src/node.h src/node.c 
	$(CC) src/htable.c $(CFLAGS)  -c -o output/htable.o

test_lexer.o: src/util.c src/util.h src/node.c src/node.h \
	src/lexer.c src/lexer.h tests/test_lexer.c src/htable.c \
	src/htable.h
	$(CC) tests/test_lexer.c $(CFLAGS)  -c -o output/test_lexer.o

clean:
	rm -f output/*
	rm -f bin/*

