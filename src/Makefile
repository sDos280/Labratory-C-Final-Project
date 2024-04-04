CC = gcc
CXXFLAGS = -Wall -ansi -pedantic -g -I ../include
LDFLAGS = 

APPNAME = ac # ac = AsemblyCompiler

.PHONY: clean main

main: lexer.o main.o
	$(CC) $(LDFLAGS) -o $(APPNAME) $^

lexer.o: lexer.c
	$(CC) $(CXXFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APPNAME)
