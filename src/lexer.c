/*
 * the lexer
*/

#include "../include/lexer.h"
#include "string_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static Lexer lexer;

void lexer_init(char * sourceString){
    lexer.string = string_init_with_data(sourceString);
    lexer.index = 0;
    lexer.currentChar = string_get_char(lexer.string, lexer.index);
    lexer.currentLine = 1;
}

void lexer_free(){
    /* reset the lexer, free the token list (lexer.tokens) and string, and other field */
    
    /* free the token list*/
    TokenList * current = lexer.tokens;
    TokenList * temp;

    while (current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
    
    /* free the source string*/
    string_free(&lexer.string);

    /* reset other fields */
    lexer.index = 0;
    lexer.currentLine = 1;
}

void print_token_list(){
    TokenList * tokens = lexer.tokens;

    while (tokens != NULL){
        switch (tokens->token.kind)
        {
        case COMMENT:
            printf("comment: %s\n", tokens->token.string.data);
            break;
        
        case EOL:
            printf("end of line: \\n\n");
            break;
        
        case COMMA:
            printf("comma: ,\n");
            break;
        
        case COLON:
            printf("colon: :\n");
            break;

        case HASHTAG:
            printf("hashtag: #\n");
            break;

        case STAR:
            printf("star: *\n");
            break;
        
        case NUMBER:
            printf("number: %d\n", atoi(tokens->token.string.data));
            break;

        default:
            break;
        }

        tokens = tokens->next;
    }
}

static void add_token(Token token){
    TokenList * toAdd = malloc(sizeof(TokenList));
    toAdd->token = token;
    toAdd->next = NULL;

    if (lexer.tokens == NULL){
        /* we haven't added a token yet */
        lexer.tokens = toAdd;
    }else{
        TokenList * last = lexer.tokens;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

static bool is_char(char ch){
    return lexer.currentChar == ch;
}

static bool is_char_in(char * string){
    /* (string should be null terminated) */
    int i;
    for (i = 0; string[i] != '\0'; i++){
        if (lexer.currentChar == string[i]) return true;
    }

    return false;
}

static bool is_char_whitespace(){
    return is_char_in("\t\r ");
}

static bool is_char_numeric(){
    return isdigit(lexer.currentChar);
}

static bool is_char_identifier_starter(){
    return isalpha(lexer.currentChar);
}

static bool is_char_identifier(){
    return isalpha(lexer.currentChar) || isdigit(lexer.currentChar);
}

static bool is_special_char(){
    return isalpha(lexer.currentChar) || isdigit(lexer.currentChar);
}

static void peek_char(){
    if (is_char('\n')) lexer.currentLine++; /* a new line is started only after the \n char */
    lexer.index++;
    lexer.currentChar = string_get_char(lexer.string, lexer.index);
}

static void peek_drop(){
    lexer.index--;
    lexer.currentChar = string_get_char(lexer.string, lexer.index);
}

void peek_comment(){
    int index = lexer.index;
    int line = lexer.currentLine;

    peek_char();  /* peek ; */
    Token token;
    token.kind = COMMENT;
    token.start = index;
    token.line = line;
    token.string = string_init();

    while(!is_char(EOF)){
        if (is_char('\n')) break; /* the end of the comment token */

        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }
    
    add_token(token);
}

void peek_next_line(){
    int index = lexer.index;
    int line = lexer.currentLine;

    Token token;
    token.kind = EOL;
    token.start = index;
    token.line = line;
    token.string = string_init_with_data("\n");

    peek_char();

    add_token(token);
}

void peek_separator(){
    int index = lexer.index;
    int line = lexer.currentLine;

    Token token;
    switch (lexer.currentChar)
    {
    case ',':
        token.kind = COMMA;
        break;

    case ':':
        token.kind = COLON;
        break;
    case '#':
        token.kind = HASHTAG;
        break;
    case '*':
        token.kind = STAR;
        break;
    
    
    default:
        break;
    }
    token.start = index;
    token.line = line;
    token.string = string_init_with_data(&lexer.currentChar);

    peek_char();

    add_token(token);
}

void peek_number(){
    int index = lexer.index;
    int line = lexer.currentLine;

    Token token;
    token.kind = NUMBER;
    token.start = index;
    token.line = line;
    token.string = string_init();

    int i;
    for (i = 0; is_char_numeric() || (i == 0 && is_char_in("+-")); i++){
        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }

    add_token(token);
}