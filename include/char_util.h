#ifndef LABRATORY_C_MAMAN22_CHAR_UTIL
#define LABRATORY_C_MAMAN22_CHAR_UTIL

/*
 * The Char util:
 * this Char util provide helper functions for easy use of chars in c.
*/

#include <stdbool.h>
#include <ctype.h>


/**
 * Checks if a character is present in a given character pointer.
 * 
 * @param ch The character to search for.
 * @param string The character pointer to search in.
 * @return true if the character is found, false otherwise.
 * @note The string parameter must be null-terminated.
 */
bool is_char_in_char_pointer(char ch, char *string);

/**
 * Checks if a character is whitespace.
 * 
 * @param ch The character to check.
 * @return true if the character is whitespace, false otherwise.
 * @note Whitespace characters include space, tab, newline, carriage return, vertical tab, and form feed.
 */
bool is_char_whitespace(char ch);

/**
 * Checks if a character is numeric (0-9).
 * 
 * @param ch The character to check.
 * @return true if the character is a digit, false otherwise.
 */
bool is_char_numeric(char ch);

/**
 * Checks if a character is a valid identifier starter character.
 * 
 * @param ch The character to check.
 * @return true if the character is a valid identifier starter, false otherwise.
 * @note Identifier starter characters include letters (a-z, A-Z) and underscores (_).
 */
bool is_char_identifier_starter(char ch);

/**
 * Checks if a character is a valid identifier character.
 * 
 * @param ch The character to check.
 * @return true if the character is a valid identifier character, false otherwise.
 * @note Identifier characters include letters (a-z, A-Z), digits (0-9), and underscores (_).
 */
bool is_char_identifier(char ch);

#endif /* LABRATORY_C_MAMAN22_CHAR_UTIL */