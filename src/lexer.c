//
// the lexer
//

#include "include/lexer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ctype.h"
static Lexer lexer;

void lexer_init(unsigned char * sourceString){
    lexer.string = sourceString;
    lexer.index = 0;
    lexer.currentChar = lexer.string[lexer.index];
}

/*static void init_token(Token* token, TokenKind kind, int start, unsigned char * string){
    // the memory for that token should have allocated by the caller
    token->kind = kind;
    token->start = start;
    token->string = string;
}*/

static void add_token(Token token){
    TokenList * toAdd = malloc(sizeof TokenList);
    toAdd->token = token;
    toAdd->next = NULL;

    if (lexer.tokens == NULL){
        // we haven't added a token yet
        lexer.tokens = toAdd;
    }else{
        TokenList * last = lexer.tokens;
        while (last.next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}
static void peek_char(){
    lexer.index++;
    lexer.currentChar = lexer.string[lexer.index];
}

static void peek_drop(){
    lexer.index--;
    lexer.currentChar = lexer.string[lexer.index];
}

static bool is_char_in(unsigned char * string){
    // (string should be null terminated)
    for (int i = 0; string[i] != EOF; i++){
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

static bool is_char_numeric(){
    return isdigit(lexer.currentChar);
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

static void peek_comment(){
    int index = lexer.index;

    peek_char();  // peek ;

    unsigned int size;  // the size of the comment (not including the ';' char and the EOL char)
    for (size = 0; !(lexer.string[index + 1 + size] == '\0' || lexer.string[index + 1 + size] == '\n'); ++size);

    unsigned char * string = malloc(size + 1);  // plus 1 for \0
    for (int i = 0; i < size; ++i) {
        string[i] = lexer.string[index + 1 + i];
    }

    string[size] = '\0';

    Token token;
    token.kind = COMMENT;
    token.start = index;
    token.string = string;

    add_token(token);
}