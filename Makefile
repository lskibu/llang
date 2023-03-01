build: util.o node.o lexer.o htable.o test_lexer.o
	gcc output/*.o -O4 -Wall -o bin/test_lexer

lexer.o: src/lexer.c src/lexer.h src/util.h src/node.h
	gcc src/lexer.c -c -o output/lexer.o

node.o: src/node.c src/node.h src/util.h
	gcc src/node.c -c -o output/node.o

util.o: src/util.c src/util.h
	gcc src/util.c -c -o output/util.o

htable.o: src/htable.c src/htable.h src/util.c src/util.h \
	src/node.h src/node.c 
	gcc src/htable.c -c -o output/htable.o

test_lexer.o: src/util.c src/util.h src/node.c src/node.h \
	src/lexer.c src/lexer.h tests/test_lexer.c src/htable.c \
	src/htable.h
	gcc tests/test_lexer.c -c -o output/test_lexer.o

clean:
	rm -f output/*
	rm -f bin/*

