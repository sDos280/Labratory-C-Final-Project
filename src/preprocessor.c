#include "../include/preprocessor.h"


void preprocessor_init(Preprocessor * preprocessor, Lexer lexer){
    preprocessor->string = lexer.string;
    preprocessor->macroList = NULL;
    /* preprocessor->errorHandler = (ErrorHandler){0}; */
    preprocessor->tokens = lexer.tokens;

    error_handler_init(&preprocessor->errorHandler, lexer.string, lexer.filePath);
}

void preprocessor_generate_macro(Preprocessor * preprocessor, String source){
    Macro macro;
    TokenError error;

    String expansion;
    int expansion_start;
    int expansion_end;
    
    int start;
    int end;

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

    /* look for '\n' endmacr '\n' */
    while (preprocessor->tokens->token.kind != EOFT && 
           preprocessor->tokens->next->token.kind != EOFT &&
           preprocessor->tokens->next->next->token.kind != EOFT){

        if (preprocessor->tokens->token.kind == EOL && 
            preprocessor->tokens->next->token.kind == ENDMACR &&
            preprocessor->tokens->next->next->token.kind == EOL) {

            expansion_end = preprocessor->tokens->token.index;
            end = preprocessor->tokens->next->next->token.index;
            wasGoodEndMacroFound = true;
            break;
        } 
        else if (preprocessor->tokens->token.kind == ENDMACR){
            wasEndMacroFound = true;
            break;
        }

        preprocessor->tokens = preprocessor->tokens->next;
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

    add_macro_to_preprocessor_macro_list(preprocessor, macro);
}


void add_macro_to_preprocessor_macro_list(Preprocessor * preprocessor, Macro macro){
    MacroList * toAdd = malloc(sizeof(MacroList));
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

void generate_macro_list(Preprocessor * preprocessor, Lexer lexer){
    String expansion;
    TokenError error;
    Macro currentMacro;
    TokenList * currentToken = lexer.tokens;
    bool wasMacroCloserFound = false;
    int expansion_start;
    int expansion_end;
    int start;
    int end;

    while (currentToken->token.kind != EOFT){
        if (currentToken->token.kind == MACR){
            start = currentToken->token.index;

            /* move over the MACR token */
            currentToken = currentToken->next;

            if (currentToken->token.kind != IDENTIFIER){
                error.message = string_init_with_data("A (none operative) identifier wasn't found after an macro token");
                error.token = currentToken->token;

                error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
                return; /* there is truly no need to continue to look for other macroes*/
            }

            /* move over the IDENTIFIER token */
            currentToken = currentToken->next;

            if (currentToken->token.kind != EOL){
                error.message = string_init_with_data("A new line is needed after a macro identifier");
                error.token = currentToken->token;

                error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
                return; /* there is truly no need to continue to look for other macroes*/
            }

            expansion_start = currentToken->token.index + 1;

            /* move over the EOL token */
            currentToken = currentToken->next;

            while (currentToken->token.kind != EOFT){
                if (currentToken->token.kind == ENDMACR){
                    end = currentToken->token.index + string_length(currentToken->token.string);
                    
                    if (currentToken->token.kind != EOL){
                        error.message = string_init_with_data("new line is needed after a macro ender");
                        error.token = currentToken->token;

                        error_handler_push_token_error(&preprocessor->errorHandler, PreprocessorErrorKind, error);
                        return; /* there is truly no need to continue to look for other macroes*/
                    }

                    break; /* every this has been done current so to look for more macroes*/
                }
            }
        }

        /* advance to the next token */
        currentToken = currentToken->next;
    }
}