CC = gcc
CXXFLAGS = -Wstrict-prototypes -Wold-style-declaration -Wall -ansi -pedantic -g -I ../../../include
LDFLAGS = 
current_dir := $(notdir $(CURDIR))

APPNAME = $(current_dir)

.PHONY: clean main

# compiler targets
$(current_dir): ../../../src/lexer.c ../../../utils/string_util.c
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $(APPNAME) $^ $(current_dir).c

clean:
	rm -f $(APPNAME)

#valgrind
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(APPNAME)
