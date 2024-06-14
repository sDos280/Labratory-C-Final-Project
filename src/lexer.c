/*
 * the lexer
*/

#include "../include/consts.h"
#include "../include/lexer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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
    static bool wasNewLineLastChar = false;

    if (is_char_in_char_pointer(lexer->ch, "\n")) 
    {
        /* a new line is started only after the \n char */
        lexer->line++;
        wasNewLineLastChar = true;
    }else{
        if (wasNewLineLastChar == true){
            lexer->indexInLine = 0;
            wasNewLineLastChar = false;
        }
        lexer->indexInLine++;
    }

    lexer->index++;
    lexer->ch = string_get_char(lexer->string, lexer->index);
}

void lexer_peek_comment(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int line_index = lexer->indexInLine;

    Token token;
    token.kind = COMMENT;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    while(!is_char_in_char_pointer(lexer->ch, "\0")){
        if (is_char_in_char_pointer(lexer->ch, "\n")) break; /* the end of the comment token */

        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
    }
    
    add_token(lexer, token);
}

void lexer_peek_next_line(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int line_index = lexer->indexInLine;

    Token token;
    token.kind = EOL;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init_with_data("\n");

    lexer_peek_char(lexer);

    add_token(lexer, token);
}

void lexer_peek_separator(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int line_index = lexer->indexInLine;

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
    token.line_index = line_index;
    token.line = line;
    token.string = string_init_with_data(&lexer->ch);

    lexer_peek_char(lexer);

    add_token(lexer, token);
}

void lexer_peek_number(Lexer * lexer){
    int index = lexer->index;
    int line = lexer->line;
    int line_index = lexer->indexInLine;
    int i;

    Token token;
    token.kind = NUMBER;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !is_char_in_char_pointer(lexer->ch, "\0") && (is_char_numeric(lexer->ch) || (i == 0 && is_char_in_char_pointer(lexer->ch, "+-"))); i++){
        string_add_char(&token.string, lexer->ch);
        lexer_peek_char(lexer);
    }
    /* check if we only got +/ without any numerical numbers after that */
    if (token.string.index == 1) {
        if (string_get_char(token.string, 0) == '+' || string_get_char(token.string, 0) == '-'){
            LexerCharError error;

            error.ch = string_get_char(token.string, 0);
            error.index = token.index;
            error.indexInLine = token.line_index;
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
    int line_index = lexer->indexInLine;
    int i;

    Token token;
    token.kind = STRING;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !is_char_in_char_pointer(lexer->ch, "\0") && ((i == 0 /* for the first " char */) || (lexer->ch != '\"')); i++){
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
    int line_index = lexer->indexInLine;
    int i;

    LexerTokenError error;

    Token token;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    
    for (i = 0; !is_char_in_char_pointer(lexer->ch, "\0") && ((i == 0 /* for the . char */) || (is_char_identifier_starter(lexer->ch) /* only alpha no numeric*/)); i++){
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
    int line_index = lexer->indexInLine;

    Token token;
    token.index = index;
    token.line_index = line_index;
    token.line = line;
    token.string = string_init();

    string_add_char(&token.string, lexer->ch); /* the first char is always an alfa char so it has to be part of the identifier*/
    lexer_peek_char(lexer);
    
    while (!is_char_in_char_pointer(lexer->ch, '\0')){
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

void lexer_lex(Lexer * lexer){
    while (!is_char_in_char_pointer(lexer->ch, "\0"))
    {
        if (is_char_whitespace(lexer->ch)){
            lexer_peek_char(lexer); /* we simply move over whitespaces*/
        } else if (is_char_in_char_pointer(lexer->ch, ";")){
            lexer_peek_comment(lexer);
        } else if (is_char_in_char_pointer(lexer->ch, "\n")){
            lexer_peek_next_line(lexer);
        }else if (is_char_in_char_pointer(lexer->ch, ",:#*")){
            lexer_peek_separator(lexer);
        } else if (is_char_numeric(lexer->ch) || is_char_in_char_pointer(lexer->ch, "+-")){
            lexer_peek_number(lexer);
        } else if (is_char_in_char_pointer(lexer->ch, "\"")){
            lexer_peek_string(lexer);
        } else if (is_char_in_char_pointer(lexer->ch, ".")){
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

            lexer_peek_char(lexer); /* we would continue to lex in any case, to find more lexer errors*/
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
            printf("%s : Lexer Error : %s\n", PROJECT_NAME, current->error.tokenError.message.data);
            printf("    %u | ", line);

            while (string_get_char(lexer->string, index) != '\0' && string_get_char(lexer->string, index) != '\n'){ 
                putchar(string_get_char(lexer->string, index));
                index++;
            }

            printf("\n");

            /* print token highlight */
            printf("    ");
            
            for (i = 0; i < countDigits(line); i++){
                printf(" ");
            }

            printf(" | ");

            for (i = 0; i < current->error.tokenError.token.line_index; i++){
                printf(" ");
            }

            /* print to token highligh itself*/
            for (i = 0; i < current->error.tokenError.token.string.index; i++){
                if (i == 0) printf("^");
                else printf("~");
            }

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

            /* print the line the error has occurred */
            printf("%s : Lexer Error : %s : '%c' \n", PROJECT_NAME, current->error.charError.message.data, current->error.charError.ch);
            printf("    %u | ", line);
            
            while (string_get_char(lexer->string, index) != '\0' && string_get_char(lexer->string, index) != '\n'){ 
                putchar(string_get_char(lexer->string, index));
                index++;
            }

            printf("\n");

            /* print token highlight */
            printf("    ");
            
            for (i = 0; i < countDigits(line); i++){
                printf(" ");
            }

            printf(" | ");

            for (i = 0; i < current->error.tokenError.token.line_index; i++){
                printf(" ");
            }

            /* print to token highligh itself*/
            printf("^\n");

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