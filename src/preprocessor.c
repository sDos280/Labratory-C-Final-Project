#include "../include/safe_allocations.h"
#include "../include/preprocessor.h"

#define RED_COLOR   "\x1B[1;91m"
#define GRN_COLOR   "\x1B[32m"
#define YEL_COLOR   "\x1B[33m"
#define BLU_COLOR   "\x1B[34m"
#define MAG_COLOR   "\x1B[35m"
#define CYN_COLOR   "\x1B[36m"
#define WHT_COLOR   "\x1B[37m"
#define RESET_COLOR "\x1B[0m"

static void writeDataToFile(FILE *file, String data) {
    int i = 0;

    while (!(string_get_char(data, i) == EOF || string_get_char(data, i) == '\0')) {
        fputc(string_get_char(data, i), file);
        i++;
    }
}

void preprocessor_init(Preprocessor * preprocessor, Lexer lexer, char * filePath){
    /* curate the file path string */
    char * filePathCurated = safe_calloc((strlen(filePath) + 3) + 1, sizeof(char));
    strcpy(filePathCurated, filePath);
    strcat(filePathCurated, ".am");
    
    preprocessor->string = string_init();
    preprocessor->macroList = NULL;
    /* preprocessor->errorHandler = (ErrorHandler){0}; */
    preprocessor->tokens = lexer.tokens;

    error_handler_init(&preprocessor->errorHandler, lexer.string, filePathCurated);
}

void preprocessor_free(Preprocessor * preprocessor){
    MacroList * current = preprocessor->macroList;
    string_free(preprocessor->string);

    error_handler_free_error_list(&preprocessor->errorHandler);

    /* free the macro list */
    while (preprocessor->macroList != NULL){
        current = preprocessor->macroList;

        /* free the macro expansion */
        string_free(current->macro.expansion);

        /* move over to the next macro and save a "copy" of the current */
        preprocessor->macroList = preprocessor->macroList->next;

        /* free the current macro */
        free(current);
    }

    free(preprocessor->errorHandler.filePath); /* the filepath in the error handler doens't free itself, so we free it here*/
}

void preprocessor_generate_macro(Preprocessor * preprocessor, String source){
    Macro macro;
    TokenError error;

    int expansion_start = 0;
    int expansion_end = 0;
    
    int start = 0;
    int end = 0;

    MacroList * macroList = NULL;
    TokenList * copy = NULL;

    bool wasGoodEndMacroFound = false;
    bool wasEndMacroFound = false;

    if (preprocessor->tokens->token.kind != MACR){
        error.message = string_init_with_data("A (none operative) identifier wasn't found after an macro token");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes */
    }

    start = preprocessor->tokens->token.index;

    /* move over the MACR token */
    preprocessor->tokens = preprocessor->tokens->next;

    if (preprocessor->tokens->token.kind != IDENTIFIER){
        error.message = string_init_with_data("A (none operative) identifier wasn't found after an macro token");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes */
    }

    macro.identifier = preprocessor->tokens->token;

    macroList = preprocessor->macroList;

    /* check if the macro identifier is already used in another macro */
    while (macroList != NULL){
        if (string_equals(macro.identifier.string, macroList->macro.identifier.string)){
            error.message = string_init_with_data("That identifer was already used in another macro, please change the macro identifier name");
            error.token = macro.identifier;

            error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        }

        macroList = macroList->next;
    }

    /* move over the IDENTIFIER token */
    preprocessor->tokens = preprocessor->tokens->next;

    if (preprocessor->tokens->token.kind != EOL){
        error.message = string_init_with_data("A new line is needed after a macro identifier");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
        return; /* there is truly no need to continue to look for other macroes*/
    }

    expansion_start = preprocessor->tokens->token.index + 1;

    /* move over the EOL token */
    preprocessor->tokens = preprocessor->tokens->next;

    copy = preprocessor->tokens;

    /* look for '\n' endmacr '\n' */
    while ((copy != NULL && copy->token.kind != EOFT) && 
           (copy->next != NULL && copy->next->token.kind != EOFT) &&
           (copy->next->next != NULL)){

        if (copy->token.kind == EOL && 
            copy->next->token.kind == ENDMACR &&
            (copy->next->next->token.kind == EOL || copy->next->next->token.kind == EOFT)) {

            expansion_end = copy->token.index;
            end = copy->next->next->token.index;
            wasGoodEndMacroFound = true;
            break;
        } 
        else if (copy->token.kind == ENDMACR){
            wasEndMacroFound = true;
            break;
        }

        copy = copy->next;
    }

    if (wasGoodEndMacroFound == true){
        preprocessor->tokens = copy;
    } else {
        while (preprocessor->tokens->token.kind != EOFT) {
            if (preprocessor->tokens->token.kind == ENDMACR){
                wasEndMacroFound = true;
                break;
            }

            preprocessor->tokens = preprocessor->tokens->next;
        }
    }

    if ((wasGoodEndMacroFound == false) && (wasEndMacroFound == true)){
        error.message = string_init_with_data("Although an end of macro was found, the grammer isn't correct");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
    } else if ((wasGoodEndMacroFound == false) && (wasEndMacroFound == false)){
        error.message = string_init_with_data("No end of macro was found");
        error.token = preprocessor->tokens->token;

        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
    }

    macro.expansion = string_get_slice(source, expansion_start, expansion_end);
    macro.start = start;
    macro.end = end;

    preprocessor_add_macro_to_macro_list(preprocessor, macro);
}

void preprocessor_generate_macro_list(Preprocessor * preprocessor, String source){
    TokenList * copy = preprocessor->tokens;

    while (preprocessor->tokens != NULL && preprocessor->tokens->token.kind != EOFT){
        if (preprocessor->tokens->token.kind == MACR){
            preprocessor_generate_macro(preprocessor, source);
        }

        /* advance to the next token */
        preprocessor->tokens = preprocessor->tokens->next;
    }

    preprocessor->tokens = copy; /* make sure the preprocessor will still point to the tokens */
}

void preprocessor_add_macro_to_macro_list(Preprocessor * preprocessor, Macro macro){
    MacroList * toAdd = safe_malloc(sizeof(MacroList));
    toAdd->macro = macro;
    toAdd->next = NULL;

    if (preprocessor->macroList == NULL){
        /* we haven't added a token yet */
        preprocessor->macroList = toAdd;
    }else{
        MacroList * last = preprocessor->macroList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void preprocessor_print_macro_list(Preprocessor * preprocessor){
    MacroList * current = preprocessor->macroList;

    while (current != NULL){
        printf("macro expansion identifier: %s\n", current->macro.identifier.string.data);
        printf("macro expansion start:\n%s\n", current->macro.expansion.data);
        printf("macro expansion end!\n");

        current = current->next;
    }
}

void preprocessor_preprocess_to_source(Preprocessor * preprocessor, String source){
    bool isTokenMacroIdentifier = false;
    MacroList * macroListCopy = preprocessor->macroList;
    MacroList * currentMacro = preprocessor->macroList;
    TokenList * currentToken = preprocessor->tokens;
    unsigned int i = 0; /* the current char index */

    for (i = 0; i < string_length(source);){
        if (currentMacro != NULL && i >= currentMacro->macro.start && i <= currentMacro->macro.end){
            /* check if the current index is inside the current macro */

            /* move the currentToken to the first token after the macro */
            while (currentToken != NULL && currentToken->token.index <= currentMacro->macro.end) currentToken = currentToken->next;

            /* set the current char index to the char after the macro end */
            i = currentMacro->macro.end + 1;
            currentMacro = currentMacro->next;
        } else if (currentToken != NULL && i >= currentToken->token.index && i <= currentToken->token.index + string_length(currentToken->token.string) - 1){
            /* check if the current index is inside the current tooken */

            macroListCopy = preprocessor->macroList;
            isTokenMacroIdentifier = false;

            /* check if the identifier is a macro identifier */
            while (macroListCopy != NULL){
                if (string_equals(macroListCopy->macro.identifier.string, currentToken->token.string)) {
                    isTokenMacroIdentifier = true;
                    break; /* break so we save the macro identifier match */
                }

                macroListCopy = macroListCopy->next;
            }

            if (isTokenMacroIdentifier == true){
                string_add_string(&preprocessor->string, macroListCopy->macro.expansion);
            }else {
                string_add_string(&preprocessor->string, currentToken->token.string);
            }
            
            /* set the current char index to the char after the current char */
            i = currentToken->token.index + string_length(currentToken->token.string);
            currentToken = currentToken->next;
            /* check if the token after the macro identifier there is an EOL, if there is we wouldn't add it */
            if (isTokenMacroIdentifier == true && currentToken != NULL && currentToken->token.kind == EOL){
                currentToken = currentToken->next;
                i++;
            }
        } else {
            string_add_char(&preprocessor->string, string_get_char(source, i));
            i++;
        }
    }
}

void preprocessor_preprocess(Preprocessor * preprocessor, String source){
    FILE *file;
    
    preprocessor_generate_macro_list(preprocessor, source);

    /* make the preprocess part only if there were on errors */
    if (preprocessor->errorHandler.errorList != NULL) return;

    preprocessor_preprocess_to_source(preprocessor, source);

    file = fopen(preprocessor->errorHandler.filePath, "w");
    if (file == NULL) {
        printf("%sPreprocessor Error:%s couldn't open \"%s\".\n", RED_COLOR, RESET_COLOR, preprocessor->errorHandler.filePath);
    }

    writeDataToFile(file, preprocessor->string);

    fclose(file);

    preprocessor->errorHandler.string = preprocessor->string; /* update string data because size may have changed */
}