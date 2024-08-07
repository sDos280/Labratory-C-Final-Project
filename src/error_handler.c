#include "../include/error_handler.h"

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

void error_handler_init(ErrorHandler * handler, String string, char * filePath){
    handler->string = string;
    handler->filePath = filePath;
    handler->errorList = NULL;
}

void error_handler_push_token_error(ErrorHandler * handler, ErrorCaller caller, TokenError error){
    ErrorList * toAdd = malloc(sizeof(ErrorList));
    toAdd->error.tokenError = error;
    toAdd->kind = TokenErrorKind;
    toAdd->caller = caller;
    toAdd->next = NULL;

    if (handler->errorList == NULL){
        /* we haven't added an error yet */
        handler->errorList = toAdd;
    }else{
        ErrorList * last = handler->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void error_handler_push_char_error(ErrorHandler * handler, ErrorCaller caller, CharError error){
    ErrorList * toAdd = malloc(sizeof(ErrorList));
    toAdd->error.charError = error;
    toAdd->kind = CharErrorKind;
    toAdd->caller = caller;
    toAdd->next = NULL;

    if (handler->errorList == NULL){
        /* we haven't added an error yet */
        handler->errorList = toAdd;
    }else{
        ErrorList * last = handler->errorList;
        while (last->next != NULL){
            last = last->next;
        }

        last->next = toAdd;
    }
}

void error_handler_flush_error_list(ErrorHandler * handler){
    ErrorList * current = handler->errorList;
    unsigned int index = 0;
    unsigned int line = 1; /* line index */
    unsigned int i;
    char * caller_string;

    while (current != NULL)
    {
        switch (current->caller)
        {
        case LexerErrorKind:
            caller_string = "Lexer Error";
            break;
        
        case PreprocessorErrorKind:
            caller_string = "Preprocessor Error";
            break;
        
        case ParserErrorKind:
            caller_string = "Parser Error";
            break;

        case AstCheckerErrorKind:
            caller_string = "AST Checker Error";
            break;
        
        case EmitterErrorKind:
            caller_string = "Emitter Error";
            break;
        
        default:
            caller_string = "";
            break;
        }

        switch (current->kind)
        {
        case TokenErrorKind:
            index = 0;
            line = 1;

            /* move index to the first index in the line that the token is inside */
            while (string_get_char(handler->string, index) != '\0' && line != current->error.tokenError.token.line){
                if (index != 0 && string_get_char(handler->string, index - 1) == '\n'){
                    line++;
                    if (line == current->error.tokenError.token.line)
                        break; /* we break now because we already got the wanted index so we don't want index++ to happen*/
                }
                index++;
            }

            /* print the line the error has occurred */
            printf("%s:%d:%d: %s%s%s: %s\n", handler->filePath, current->error.tokenError.token.line, current->error.tokenError.token.indexInLine + 1, RED_COLOR, caller_string, RESET_COLOR, current->error.tokenError.message.data);
            printf("    %u | ", line);

            while (string_get_char(handler->string, index) != '\0' && string_get_char(handler->string, index) != '\n'){ 
                if (index == current->error.tokenError.token.index) printf("%s", RED_COLOR);
                if (index == current->error.tokenError.token.index + string_length(current->error.tokenError.token.string)) printf("%s", RESET_COLOR);
                putchar(string_get_char(handler->string, index));
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
        case CharErrorKind:
            index = 0;
            line = 1;

            /* move index to the first index in the line that the char is inside */
            while (string_get_char(handler->string, index) != '\0' && line != current->error.charError.line){
                if (index != 0 && string_get_char(handler->string, index - 1) == '\n'){
                    line++;
                    if (line == current->error.charError.line)
                        break; /* we break now because we already got the wanted index so we don't want index++ to happen*/
                }
                index++;
            }
            printf("%s", RESET_COLOR);

            /* print the line the error has occurred */
            printf("%s:%d:%d: %s%s%s: %s\n", handler->filePath, current->error.tokenError.token.line, current->error.tokenError.token.indexInLine + 1, RED_COLOR, caller_string, RESET_COLOR, current->error.charError.message.data);
            printf("    %u | ", line);
            
            while (string_get_char(handler->string, index) != '\0' && string_get_char(handler->string, index) != '\n'){
                if (index == current->error.tokenError.token.index) printf("%s", RED_COLOR);
                putchar(string_get_char(handler->string, index));
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

void error_handler_free_error_list(ErrorHandler * handler){
    ErrorList * temp;
    ErrorList * current = handler->errorList;
    
    while (current != NULL)
    {
        switch (current->kind)
        {
        case TokenErrorKind:
            string_free(current->error.tokenError.message);
            break;
        case CharErrorKind:
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

