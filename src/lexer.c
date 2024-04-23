/*
 * the lexer
*/

#include "consts.h"
#include "../include/lexer.h"
#include "string_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

static Lexer lexer;
static LexerErrorList * errorList;

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
        string_free(&current->token.string); /* free the string of each token */
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
            printf("number: %d, line_index: %d\n", atoi(tokens->token.string.data), tokens->token.line_index);
            break;
        
        case STRING:
            printf("string: %s\n", tokens->token.string.data);
            break;

        case DATA_INS:
            printf("non operative instructions: %s\n", tokens->token.string.data);
            break;
        
        case STRING_INS:
            printf("non operative instructions: %s\n", tokens->token.string.data);
            break;
        
        case ENTRY_INS:
            printf("non operative instructions: %s\n", tokens->token.string.data);
            break;
        
        case EXTERN_INS:
            printf("non operative instructions: %s\n", tokens->token.string.data);
            break;
        
        case TokenError:
            printf("error token: %s\n", tokens->token.string.data);
            break;
        
        case REGISTER:
            printf("register: %s\n", tokens->token.string.data);
            break;
        
        case MACR:
            printf("macro start token: %s\n", tokens->token.string.data);
            break;
        
        case ENDMACR:
            printf("macro end token: %s\n", tokens->token.string.data);
            break;
        
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
        case RTS:
        case STOP:
            printf("operative instruction: %s\n", tokens->token.string.data);
            break;

        case IDENTIFIER:
            printf("identifier: %s\n", tokens->token.string.data);
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
    if (is_char('\n')) 
    {
        /* a new line is started only after the \n char */
        lexer.currentLine++;
        lexer.line_index = 0;
    }else{
        lexer.line_index++;
    }

    lexer.index++;
    lexer.currentChar = string_get_char(lexer.string, lexer.index);
}

/*static void peek_drop(){
    lexer.index--;
    lexer.currentChar = string_get_char(lexer.string, lexer.index);
}*/

void peek_comment(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.kind = COMMENT;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    while(!is_char('\0')){
        if (is_char('\n')) break; /* the end of the comment token */

        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }
    
    add_token(token);
}

void peek_next_line(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.kind = EOL;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init_with_data("\n");

    peek_char();

    add_token(token);
}

void peek_separator(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

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
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init_with_data(&lexer.currentChar);

    peek_char();

    add_token(token);
}

void peek_number(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.kind = NUMBER;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    int i;
    for (i = 0; !is_char('\0') && (is_char_numeric() || (i == 0 && is_char_in("+-"))); i++){
        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }

    add_token(token);
}

void peek_string(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.kind = STRING;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    int i;
    for (i = 0; !is_char('\0') && ((i == 0 /* for the first " char */) || (lexer.currentChar != '\"')); i++){
        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }

    string_add_char(&token.string, lexer.currentChar);
    peek_char(); /* peek the last " char */

    add_token(token);
}

void peek_non_op_instruction(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    int i;
    for (i = 0; !is_char('\0') && ((i == 0 /* for the . char */) || (is_char_identifier_starter() /* only alpha no numeric*/)); i++){
        string_add_char(&token.string, lexer.currentChar);
        peek_char();
    }

    if (string_equals_char_pointer(token.string, ".data")){
        token.kind = DATA_INS;
    } else if (string_equals_char_pointer(token.string, ".string")){
        token.kind = STRING_INS;
    } else if (string_equals_char_pointer(token.string, ".entry")){
        token.kind = ENTRY_INS;
    } else if (string_equals_char_pointer(token.string, ".extern")){
        token.kind = EXTERN_INS;
    }else {
        /* there isn't a known non operative instruction that match the one that we know, so we raise an error */
        
        token.kind = TokenError;

        LexerTokenError error;
        error.token = token;
        error.message = string_init_with_data("unknown non operative instruction");

        push_lexer_token_error(error);
    }

    add_token(token);
}


void peek_identifier(){
    int index = lexer.index;
    int line = lexer.currentLine;
    int line_index = lexer.line_index;

    Token token;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    string_add_char(&token.string, lexer.currentChar); /* the first char is always an alfa char so it has to be part of the identifier*/
    peek_char();
    
    while (!is_char('\0')){
        if (is_char_identifier()){
            string_add_char(&token.string, lexer.currentChar);
            peek_char();
        }
        else 
        {
            break;
        }
    }

    /* classify idenifiers if needed */
    /* check for regirster */
    if (string_equals_char_pointer(token.string, "r0") ||
        string_equals_char_pointer(token.string, "r1") ||
        string_equals_char_pointer(token.string, "r2") ||
        string_equals_char_pointer(token.string, "r3") ||
        string_equals_char_pointer(token.string, "r4") ||
        string_equals_char_pointer(token.string, "r5") ||
        string_equals_char_pointer(token.string, "r6") ||
        string_equals_char_pointer(token.string, "r7"))
        token.kind = REGISTER;
    /* check for macro token*/
    else if (string_equals_char_pointer(token.string, "macr") ||
        string_equals_char_pointer(token.string, "endmacr"))
        token.kind = string_equals_char_pointer(token.string, "macr")? MACR : ENDMACR;
    /* check for operative instruction*/
    else if (string_equals_char_pointer(token.string, "mov"))
        token.kind = MOV;
    else if (string_equals_char_pointer(token.string, "cmp"))
        token.kind = CMP;
    else if (string_equals_char_pointer(token.string, "add"))
        token.kind = ADD;
    else if (string_equals_char_pointer(token.string, "sub"))
        token.kind = SUB;
    else if (string_equals_char_pointer(token.string, "lea"))
        token.kind = LEA;
    else if (string_equals_char_pointer(token.string, "clr"))
        token.kind = CLR;
    else if (string_equals_char_pointer(token.string, "not"))
        token.kind = NOT;
    else if (string_equals_char_pointer(token.string, "inc"))
        token.kind = INC;
    else if (string_equals_char_pointer(token.string, "dec"))
        token.kind = DEC;
    else if (string_equals_char_pointer(token.string, "jmp"))
        token.kind = JMP;
    else if (string_equals_char_pointer(token.string, "bne"))
        token.kind = BNE;
    else if (string_equals_char_pointer(token.string, "red"))
        token.kind = RED;
    else if (string_equals_char_pointer(token.string, "prn"))
        token.kind = PRN;
    else if (string_equals_char_pointer(token.string, "jsr"))
        token.kind = JSR;
    else if (string_equals_char_pointer(token.string, "rts"))
        token.kind = RTS;
    else if (string_equals_char_pointer(token.string, "stop"))
        token.kind = STOP;
    else
        token.kind = IDENTIFIER; 

    add_token(token);      
}

void push_lexer_token_error(LexerTokenError error){
    LexerErrorList * toAdd = malloc(sizeof(LexerErrorList));
    toAdd->tokenError = error;
    toAdd->kind = LexerTokenErrorKind;
    toAdd->next = NULL;

    if (errorList == NULL){
        /* we haven't added an error yet */
        errorList = toAdd;
    }else{
        LexerErrorList * last = errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void flush_lexer_error_list(){
    LexerErrorList * current = errorList;
    while (current != NULL)
    {
        switch (current->kind)
        {
        case LexerTokenErrorKind:
            unsigned int index = 0;
            unsigned int line = 1; /* line index */

            /* move index to the first index in the line that the token is inside */
            while (string_get_char(lexer.string, index) != '\0' && line != current->tokenError.token.line){
                if (index != 0)
                    if (string_get_char(lexer.string, index - 1) == '\n') line++;
                    else index++;
                else index++;
            }

            printf("%s : Lexer Error : %s\n", PROJECT_NAME, current->tokenError.message.data);
            printf("    %u | ", line);
            
            while (string_get_char(lexer.string, index) != '\0' && string_get_char(lexer.string, index) != '\n'){ 
                putchar(string_get_char(lexer.string, index));
                index++;
            }

            printf("\n");

            break;
        
        default:
            break;
        }

        current = current->next;
    }
    
}