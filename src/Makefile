CC = gcc
CXXFLAGS = -Wall -ansi -pedantic -g -I ../../../include
LDFLAGS = 

APPNAME = assembler # ac = AsemblyCompiler

.PHONY: clean main valgrind

# compiler targets
main: error_handler.o safe_allocations.o ast_checker.o lexer.o preprocessor.o parser.o emitter.o error_handler.o main.o string_util.o _char_util.o
	$(CC) $(LDFLAGS) -o $(APPNAME) $^

safe_allocations.o: safe_allocations.c
	$(CC) $(CXXFLAGS) -c $< -o $@

error_handler.o: error_handler.c
	$(CC) $(CXXFLAGS) -c $< -o $@

lexer.o: lexer.c
	$(CC) $(CXXFLAGS) -c $< -o $@

preprocessor.o: preprocessor.c
	$(CC) $(CXXFLAGS) -c $< -o $@

parser.o: parser.c
	$(CC) $(CXXFLAGS) -c $< -o $@

ast_checker.o: ast_checker.c
	$(CC) $(CXXFLAGS) -c $< -o $@

emitter.o: emitter.c
	$(CC) $(CXXFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CXXFLAGS) -c $< -o $@

# utils targets
string_util.o: ../utils/string_util.c
	$(CC) $(CXXFLAGS) -c $< -o $@

_char_util.o: ../utils/char_util.c
	$(CC) $(CXXFLAGS) -c $< -o $@

# valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind.out.txt ./$(APPNAME) $(ARGS)

clean:
	rm -f ./*.o ../utils/*.o valgrind.out.txt $(APPNAME)
