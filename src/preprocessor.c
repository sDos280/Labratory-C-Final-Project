#include "../include/preprocessor.h"


void preprocessor_init(Preprocessor * preprocessor, Lexer lexer){
    preprocessor->string = (String){};
    preprocessor->macroList = NULL;
    preprocessor->errorList = NULL;
    preprocessor->tokens = lexer.tokens;
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

void add_error_to_preprocessor_macro_list(Preprocessor * preprocessor, PreprocessorTokenError error){
    PreprocessorTokenErrorList * toAdd = malloc(sizeof(PreprocessorTokenErrorList));
    toAdd->error = error;
    toAdd->next = NULL;

    if (preprocessor->errorList == NULL){
        /* we haven't added a token yet */
        preprocessor->errorList = toAdd;
    }else{
        PreprocessorTokenErrorList * last = preprocessor->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void generate_macro_list(Preprocessor * preprocessor, Lexer lexer){
    String expansion;
    PreprocessorTokenError error;
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

                add_error_to_preprocessor_macro_list(preprocessor, error);
                return; /* there is truly no need to continue to look for other macroes*/
            }

            /* move over the IDENTIFIER token */
            currentToken = currentToken->next;

            if (currentToken->token.kind != EOL){
                error.message = string_init_with_data("A new line is needed after a macro identifier");
                error.token = currentToken->token;

                add_error_to_preprocessor_macro_list(preprocessor, error);
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

                        add_error_to_preprocessor_macro_list(preprocessor, error);
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