CC = gcc
CXXFLAGS = -Wall -ansi -pedantic -g -I ../include
LDFLAGS = 

APPNAME = ac # ac = AsemblyCompiler

.PHONY: clean main

# compiler targets
main: lexer.o main.o string_util.o
	$(CC) $(LDFLAGS) -o $(APPNAME) $^

lexer.o: lexer.c
	$(CC) $(CXXFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CXXFLAGS) -c $< -o $@

# utils targets
string_util.o: ../utils/string_util.c
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f ./*.o ../utils/*.o $(APPNAME)
