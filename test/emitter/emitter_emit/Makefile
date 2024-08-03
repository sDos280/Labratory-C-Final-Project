CC = gcc
CXXFLAGS = -Wall -ansi -pedantic -g -I ../../../include
LDFLAGS = 
current_dir := $(notdir $(CURDIR))

APPNAME = $(current_dir)

.PHONY: clean main valgrind

# compiler targets
$(current_dir): ../../../src/error_handler.c ../../../src/lexer.c ../../../src/preprocessor.c ../../../src/parser.c ../../../src/ast_checker.c ../../../src/emitter.c ../../../utils/string_util.c ../../../utils/char_util.c
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $(APPNAME) $^ $(current_dir).c

clean:
	rm -f $(APPNAME)

# valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(APPNAME)
