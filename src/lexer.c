/*
 * the lexer
*/

#include "../include/consts.h"
#include "../include/lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define RED_COLOR   "\x1B[1;91m"
#define GRN_COLOR   "\x1B[32m"
#define YEL_COLOR   "\x1B[33m"
#define BLU_COLOR   "\x1B[34m"
#define MAG_COLOR   "\x1B[35m"
#define CYN_COLOR   "\x1B[36m"
#define WHT_COLOR   "\x1B[37m"
#define RESET_COLOR "\x1B[0m"

/* some local utils */
int countDigits(int value)
{
    int result = 0;
    while(value != 0) {
       value /= 10;
       result++;
    }
    return result;
}

void lexer_init(Lexer * lexer, char * sourceString){
    lexer->string = string_init_with_data(sourceString);
    lexer->index = 0;
    lexer->indexInLine = 0;
    lexer->ch = string_get_char(lexer->string, lexer->index);
    lexer->line = 1;

    lexer->filePath = "from_string.txt";

    lexer->errorList = NULL;
    lexer->tokens = NULL;

    /* add EOF to the end of the buffer so it would follow the convention */
    string_add_char(&lexer->string, EOF);
}

void lexer_free(Lexer * lexer){
    /* reset the lexer, free the token list (lexer->tokens) and string, and other field */
    
    /* free the token list*/
    TokenList * current = lexer->tokens;
    TokenList * temp;

    while (current != NULL){
        string_free(current->token.string); /* free the string of each token */
        temp = current;
        current = current->next;
        free(temp);
    }
    
    /* free the source string*/
    string_free(lexer->string);

    /* reset other fields */
    lexer->index = 0;
    lexer->line = 1;
}

void lexer_print_token_list(Lexer * lexer){
    TokenList * tokens = lexer->tokens;

    while (tokens != NULL){
        switch (tokens->token.kind)
        {
        case COMMENT:
            printf("comment: %s\n", tokens->token.string.data);
            break;
        
        case EOL:
            printf("end of line: \\n\n");
            break;
        
        case EOFT:
            printf("end of file: (-1)\n");
            break;

        case COMMA:
            printf("comma: ','\n");
            break;
        
        case COLON:
            printf("colon: ':'\n");
            break;

        case HASHTAG:
            printf("hashtag: '#'\n");
            break;

        case STAR:
            printf("star: '*'\n");
            break;
        
        case NUMBER:
            printf("number: %d\n", atoi(tokens->token.string.data));
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

static void add_token(Lexer * lexer, Token token){
    TokenList * toAdd = malloc(sizeof(TokenList));
    toAdd->token = token;
    toAdd->next = NULL;

    if (lexer->tokens == NULL){
        /* we haven't added a token yet */
        lexer->tokens = toAdd;
    }else{
        TokenList * last = lexer->tokens;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void lexer_peek_char(Lexer * lexer){
    lexer->index++;
    lexer->ch = string_get_char(lexer->string, lexer->index);

    if (lexer->index != 0 && string_get_char(lexer->string, lexer->index - 1) == '\n'){
        lexer->line++;
        lexer->indexInLine = 0;
    } else {
        lexer->indexInLine++;
    }
}

void lexer_peek_comment(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;

    Token token;
    token.kind = COMMENT;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init();

    while(!(are_chars_equal(lexer->ch, '\0') || are_chars_equal(lexer->ch, EOF))){
        if (are_chars_equal(lexer->ch, '\n')) break; /* the end of the comment token */

        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
    }
    
    add_token(lexer, token);
}

void lexer_peek_next_line(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;

    Token token;
    token.kind = EOL;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init_with_data("\n");

    lexer_peek_char(lexer);

    add_token(lexer, token);
}

void lexer_peek_separator(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;

    Token token;
    switch (lexer->ch)
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
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init_with_data(&lexer->ch);

    lexer_peek_char(lexer);

    add_token(lexer, token);
}

void lexer_peek_number(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;
    int i;

    Token token;
    token.kind = NUMBER;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !(are_chars_equal(lexer->ch, EOF) || are_chars_equal(lexer->ch, '\0')) && (is_char_numeric(lexer->ch) || (i == 0 && is_char_in_char_pointer(lexer->ch, "+-"))); i++){
        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
    }
    /* check if we only got +/ without any numerical numbers after that */
    if (string_length(token.string) == 1) {
        if (string_get_char(token.string, 0) == '+' || string_get_char(token.string, 0) == '-'){
            LexerCharError error;

            error.ch = string_get_char(token.string, 0);
            error.index = token.index;
            error.indexInLine = token.indexInLine;
            error.line = token.line;
            error.message = string_init_with_data("it seems that you have a number sign but not any numerical chars after it");

            lexer_push_lexer_char_error(lexer, error);

            string_free(token.string);

            return; /* there is no need to add the tokens since it's invalid*/
        }
    }

    add_token(lexer, token);
}

void lexer_peek_string(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;
    int i;

    Token token;
    token.kind = STRING;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !(are_chars_equal(lexer->ch, EOF) || are_chars_equal(lexer->ch, '\0')) && ((i == 0 /* for the first " char */) || (lexer->ch != '\"')); i++){
        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
    }

    string_add_char(&token.string, lexer->ch);
    lexer_peek_char(lexer); /* peek the last " char */

    add_token(lexer, token);
}

void lexer_peek_non_op_instruction(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;
    int i;

    LexerTokenError error;

    Token token;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !(are_chars_equal(lexer->ch, EOF) || are_chars_equal(lexer->ch, '\0')) && ((i == 0 /* for the . char */) || (is_char_identifier_starter(lexer->ch) /* only alpha no numeric*/)); i++){
        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
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

        error.token = token;
        error.message = string_init_with_data("unknown non operative instruction");

        lexer_push_lexer_token_error(lexer, error);
    }

    add_token(lexer, token);
}


void lexer_peek_identifier(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;

    Token token;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init();

    string_add_char(&token.string, lexer->ch); /* the first char is always an alfa char so it has to be part of the identifier*/
    lexer_peek_char(lexer);
    
    while (!(are_chars_equal(lexer->ch, EOF) || are_chars_equal(lexer->ch, '\0'))){
        if (is_char_identifier(lexer->ch)){
            string_add_char(&token.string, lexer->ch);
            lexer_peek_char(lexer);
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

    add_token(lexer, token);      
}

void lexer_peek_end_of_file(Lexer *lexer){
    int index = lexer->index;
    int line = lexer->line;
    int indexInLine = lexer->indexInLine;
    Token token;

    char *temp = (char *)malloc(2 * sizeof(char));

    temp[0] = (char)EOF;
    temp[1] = '\0';

    token.kind = EOFT;
    token.index = index;
    token.indexInLine = indexInLine;
    token.line = line;
    token.string = string_init_with_data(temp);

    lexer_peek_char(lexer);

    add_token(lexer, token);

    free(temp);
}

void lexer_lex(Lexer * lexer){
    while (!are_chars_equal(lexer->ch, '\0'))
    {
        if (is_char_whitespace(lexer->ch)){
            lexer_peek_char(lexer); /* we simply move over whitespaces */
        } else if (are_chars_equal(lexer->ch, ';')){
            lexer_peek_comment(lexer);
        } else if (are_chars_equal(lexer->ch, '\n')){
            lexer_peek_next_line(lexer);
        }else if (are_chars_equal(lexer->ch, EOF)){
            lexer_peek_end_of_file(lexer);
        }else if (is_char_in_char_pointer(lexer->ch, ",:#*")){
            lexer_peek_separator(lexer);
        } else if (is_char_numeric(lexer->ch) || is_char_in_char_pointer(lexer->ch, "+-")){
            lexer_peek_number(lexer);
        } else if (are_chars_equal(lexer->ch, '\"')){
            lexer_peek_string(lexer);
        } else if (are_chars_equal(lexer->ch, '.')){
            lexer_peek_non_op_instruction(lexer);
        } else if (is_char_identifier_starter(lexer->ch)){
            lexer_peek_identifier(lexer);
        }else {
            LexerCharError error;

            error.ch = lexer->ch;
            error.index = lexer->index;
            error.indexInLine = lexer->indexInLine;
            error.line = lexer->line;
            error.message = string_init_with_data("unkonwn char");

            lexer_push_lexer_char_error(lexer, error);

            lexer_peek_char(lexer); /* we would continue to lex in any case, to find more lexer errors */
        }
    }
}

void lexer_push_lexer_token_error(Lexer * lexer, LexerTokenError error){
    LexerErrorList * toAdd = malloc(sizeof(LexerErrorList));
    toAdd->error.tokenError = error;
    toAdd->kind = LexerTokenErrorKind;
    toAdd->next = NULL;

    if (lexer->errorList == NULL){
        /* we haven't added an error yet */
        lexer->errorList = toAdd;
    }else{
        LexerErrorList * last = lexer->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void lexer_push_lexer_char_error(Lexer * lexer, LexerCharError error){
    LexerErrorList * toAdd = malloc(sizeof(LexerErrorList));
    toAdd->error.charError = error;
    toAdd->kind = LexerCharErrorKind;
    toAdd->next = NULL;

    if (lexer->errorList == NULL){
        /* we haven't added an error yet */
        lexer->errorList = toAdd;
    }else{
        LexerErrorList * last = lexer->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void lexer_flush_lexer_error_list(Lexer * lexer){
    LexerErrorList * current = lexer->errorList;
    unsigned int index = 0;
    unsigned int line = 1; /* line index */
    unsigned int i;

    while (current != NULL)
    {
        switch (current->kind)
        {
        case LexerTokenErrorKind:
            index = 0;
            line = 1;

            /* move index to the first index in the line that the token is inside */
            while (string_get_char(lexer->string, index) != '\0' && line != current->error.tokenError.token.line){
                if (index != 0 && string_get_char(lexer->string, index - 1) == '\n'){
                    line++;
                    if (line == current->error.tokenError.token.line)
                        break; /* we break now because we already got the wanted index so we don't want index++ to happen*/
                }
                index++;
            }

            /* print the line the error has occurred */
            printf("%s:%d:%d: %sLexer Error%s: %s\n", lexer->filePath, current->error.tokenError.token.line, current->error.tokenError.token.indexInLine + 1, RED_COLOR, RESET_COLOR, current->error.tokenError.message.data);
            printf("    %u | ", line);

            while (string_get_char(lexer->string, index) != '\0' && string_get_char(lexer->string, index) != '\n'){ 
                if (index == current->error.tokenError.token.index) printf("%s", RED_COLOR);
                if (index == current->error.tokenError.token.index + string_length(current->error.tokenError.token.string)) printf("%s", RESET_COLOR);
                putchar(string_get_char(lexer->string, index));
                index++;
            }
            printf("%s", RESET_COLOR);

            printf("\n");

            /* print token highlight */
            printf("    ");
            
            for (i = 0; i < countDigits(line); i++){
                printf(" ");
            }

            printf(" | ");

            for (i = 0; i < current->error.tokenError.token.indexInLine; i++){
                printf(" ");
            }

            /* print to token highligh itself */
            printf("%s", RED_COLOR);
            for (i = 0; i < string_length(current->error.tokenError.token.string); i++){
                if (i == 0) printf("^");
                else printf("~");
            }
            printf("%s", RESET_COLOR);

            printf("\n");

            break;
        case LexerCharErrorKind:
            index = 0;
            line = 1;

            /* move index to the first index in the line that the char is inside */
            while (string_get_char(lexer->string, index) != '\0' && line != current->error.charError.line){
                if (index != 0 && string_get_char(lexer->string, index - 1) == '\n'){
                    line++;
                    if (line == current->error.charError.line)
                        break; /* we break now because we already got the wanted index so we don't want index++ to happen*/
                }
                index++;
            }
            printf("%s", RESET_COLOR);

            /* print the line the error has occurred */
            printf("%s:%d:%d: %sLexer Error%s: %s\n", lexer->filePath, current->error.tokenError.token.line, current->error.tokenError.token.indexInLine + 1, RED_COLOR, RESET_COLOR, current->error.charError.message.data);
            printf("    %u | ", line);
            
            while (string_get_char(lexer->string, index) != '\0' && string_get_char(lexer->string, index) != '\n'){
                if (index == current->error.tokenError.token.index) printf("%s", RED_COLOR);
                putchar(string_get_char(lexer->string, index));
                if (index == current->error.tokenError.token.index) printf("%s", RESET_COLOR);
                index++;
            }

            printf("\n");

            /* print token highlight */
            printf("    ");
            
            for (i = 0; i < countDigits(line); i++){
                printf(" ");
            }

            printf(" | ");

            for (i = 0; i < current->error.tokenError.token.indexInLine; i++){
                printf(" ");
            }
            
            printf("%s", RED_COLOR);
            /* print to token highligh itself */
            printf("^\n");
            printf("%s", RESET_COLOR);

            break;

        default:
            break;
        }

        current = current->next;
    }
}

void lexer_free_lexer_error_list(Lexer * lexer){
    LexerErrorList * temp;
    LexerErrorList * current = lexer->errorList;
    
    while (current != NULL)
    {
        switch (current->kind)
        {
        case LexerTokenErrorKind:
            string_free(current->error.tokenError.message);
            break;
        case LexerCharErrorKind:
            string_free(current->error.charError.message);
            break;

        default:
            break;
        }

        /* free the current error's memory */
        temp = current;
        current = current->next;
        free(temp);
    }
}