#include "../include/char_util.h"

bool is_char_in_char_pointer(char ch, char * string){
    int i;

    if (string[0] == ch) return true; /* in case string = "\0" */
    
    for (i = 0; string[i] != '\0'; i++){
        if (ch == string[i]) return true;
    }

    return false;
}

bool are_chars_equal(char ch1, char ch2){
    return (char)ch1 == (char)ch2;
}

bool is_char_whitespace(char ch){
    return is_char_in_char_pointer(ch, "\t\r ");
}

bool is_char_numeric(char ch){
    return isdigit(ch);
}

bool is_char_identifier_starter(char ch){
    return isalpha(ch);
}

bool is_char_identifier(char ch){
    return isalpha(ch) || isdigit(ch);
}